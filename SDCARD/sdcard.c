#include "sdcard.h"



static uint32_t CardType =  SDIO_STD_CAPACITY_SD_CARD_V1_1;

SD_HandleTypeDef 		SDIO_HandleStructure;
DMA_HandleTypeDef 		Rx_DmaHandle;
DMA_HandleTypeDef 		Tx_DmaHandle;
HAL_SD_CardInfoTypedef	SDCardInfo;

void SDIO_IRQHandler(void)
{
	HAL_SD_IRQHandler(&SDIO_HandleStructure);
}   

void SD_SDIO_DMA_IRQHANDLER(void)
{
	if(DMA2->HISR & SD_SDIO_DMA_FLAG_TCIF)
	{
		if(SD_SDIO_DMA_STREAM->CR & DMA_SxCR_DIR_0)
		{
			HAL_DMA_IRQHandler(&Tx_DmaHandle);
		}
		else
		{
			HAL_DMA_IRQHandler(&Rx_DmaHandle);
		}
	}
}

/**
  * @brief  Configures SDIO IRQ channel.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
	/* Configure the NVIC Preemption Priority Bits */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);
	HAL_NVIC_SetPriority(SDIO_IRQn, 1, 2);
	HAL_NVIC_EnableIRQ(SDIO_IRQn);
	
	HAL_NVIC_SetPriority(SD_SDIO_DMA_IRQn, 2, 2);
	HAL_NVIC_EnableIRQ(SD_SDIO_DMA_IRQn);
}

void SD_LowLevel_DeInit(void)
{
	/* Reset the SDIO APB2 */
	__HAL_RCC_SDIO_FORCE_RESET();
	__HAL_RCC_SDIO_RELEASE_RESET();
	/*
	GPIO_InitTypeDef GPIO_InitStructure; 
  
	//!< Disable SDIO Clock 
	//SDIO_ClockCmd(DISABLE);
  
	//!< Set Power State to OFF 
	//!< DeInitializes the SDIO peripheral 
	HAL_SD_DeInit(&SDIO_HandleStructure);
  
	// Disable the SDIO APB2 Clock 
	__HAL_RCC_SDIO_CLK_DISABLE();

  // Configure PC.08, PC.09, PC.10, PC.11 pins: D0, D1, D2, D3 pins 
	GPIO_InitStructure.Pin 			= GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	GPIO_InitStructure.Mode 		= GPIO_MODE_AF_PP;		// push-pull alternate
	GPIO_InitStructure.Alternate 	= GPIO_AF0_MCO;
	GPIO_InitStructure.Pull  		= GPIO_NOPULL;			// no pull
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// Configure PD.02 CMD line 
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  // Configure PC.12 pin: CLK pin 
	GPIO_InitStructure.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	*/
}

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for 
  *         data transfer).
  * @param  None
  * @retval None
  */
void SD_LowLevel_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* GPIOC and GPIOD Periph clock enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

  /* Configure PC.08, PC.09, PC.10, PC.11 pins: D0, D1, D2, D3 pins */
	GPIO_InitStructure.Pin 			= GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	GPIO_InitStructure.Mode 		= GPIO_MODE_AF_PP;		// push-pull alternate
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_MEDIUM;	// 25 MHz
	GPIO_InitStructure.Alternate 	= GPIO_AF12_SDIO;
	GPIO_InitStructure.Pull  		= GPIO_PULLUP;			// pull up
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  /* Configure PD.02 CMD line */
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  /* Configure PC.12 pin: CLK pin */
	GPIO_InitStructure.Pin 	= GPIO_PIN_12;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;			// no pull
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Enable the SDIO APB2 Clock */
	__HAL_RCC_SDIO_CLK_ENABLE();

	/* Enable the DMA2 Clock */
	__HAL_RCC_DMA2_CLK_ENABLE();
}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO Tx request.
  * @param  BufferSRC: pointer to the source buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
void SD_LowLevel_DMA_TxConfig(void)
{
	Tx_DmaHandle.Instance 		= SD_SDIO_DMA_STREAM;
	
	__HAL_DMA_CLEAR_FLAG(&Tx_DmaHandle, SD_SDIO_DMA_FLAG_FEIF);
	__HAL_DMA_CLEAR_FLAG(&Tx_DmaHandle, SD_SDIO_DMA_FLAG_DMEIF);
	__HAL_DMA_CLEAR_FLAG(&Tx_DmaHandle, SD_SDIO_DMA_FLAG_TEIF);
	__HAL_DMA_CLEAR_FLAG(&Tx_DmaHandle, SD_SDIO_DMA_FLAG_HTIF);
	__HAL_DMA_CLEAR_FLAG(&Tx_DmaHandle, SD_SDIO_DMA_FLAG_TCIF);
	
	/* DMA2 Stream3  or Stream6 disable */
	__HAL_DMA_DISABLE(&Tx_DmaHandle);
	HAL_DMA_DeInit(&Tx_DmaHandle);

	Tx_DmaHandle.Parent			= &SDIO_HandleStructure;
	Tx_DmaHandle.Init.Channel 	= SD_SDIO_DMA_CHANNEL;
	Tx_DmaHandle.Init.Direction	= DMA_MEMORY_TO_PERIPH;
	Tx_DmaHandle.Init.FIFOMode	= DMA_FIFOMODE_ENABLE;
	Tx_DmaHandle.Init.PeriphInc	= DMA_PINC_DISABLE;
	Tx_DmaHandle.Init.MemInc	= DMA_MINC_ENABLE;
	Tx_DmaHandle.Init.PeriphDataAlignment	= DMA_PDATAALIGN_WORD;
	Tx_DmaHandle.Init.MemDataAlignment		= DMA_MDATAALIGN_WORD;
	Tx_DmaHandle.Init.Mode		= DMA_SxCR_PFCTRL;
	Tx_DmaHandle.Init.Priority		= DMA_PRIORITY_VERY_HIGH;
	Tx_DmaHandle.Init.FIFOMode	= DMA_FIFOMODE_ENABLE;
	Tx_DmaHandle.Init.FIFOThreshold	= DMA_FIFO_THRESHOLD_FULL;
	Tx_DmaHandle.Init.MemBurst		= DMA_MBURST_INC4;
	Tx_DmaHandle.Init.PeriphBurst	= DMA_PBURST_INC4;
	
	/* DMA2 Stream3  or Stream6 Config */
	HAL_DMA_Init(&Tx_DmaHandle);

	/* DMA2 Stream3  or Stream6 enable */
	//__HAL_DMA_ENABLE_IT(&Tx_DmaHandle, DMA_IT_TC);
	//__HAL_DMA_ENABLE(&Tx_DmaHandle);
}

/**
  * @brief  Configures the DMA2 Channel4 for SDIO Rx request.
  * @param  BufferDST: pointer to the destination buffer
  * @param  BufferSize: buffer size
  * @retval None
  */
HAL_StatusTypeDef SD_LowLevel_DMA_RxConfig(uint32_t dst_addr)
{
	Rx_DmaHandle.Instance 		= SD_SDIO_DMA_STREAM;
	
	__HAL_DMA_CLEAR_FLAG(&Rx_DmaHandle, SD_SDIO_DMA_FLAG_FEIF);
	__HAL_DMA_CLEAR_FLAG(&Rx_DmaHandle, SD_SDIO_DMA_FLAG_DMEIF);
	__HAL_DMA_CLEAR_FLAG(&Rx_DmaHandle, SD_SDIO_DMA_FLAG_TEIF);
	__HAL_DMA_CLEAR_FLAG(&Rx_DmaHandle, SD_SDIO_DMA_FLAG_HTIF);
	__HAL_DMA_CLEAR_FLAG(&Rx_DmaHandle, SD_SDIO_DMA_FLAG_TCIF);
	
	/* DMA2 Stream3  or Stream6 disable */
	__HAL_DMA_DISABLE(&Rx_DmaHandle);
	HAL_DMA_DeInit(&Rx_DmaHandle);

	Rx_DmaHandle.Parent			= &SDIO_HandleStructure;
	Rx_DmaHandle.Init.Channel 	= SD_SDIO_DMA_CHANNEL;
	Rx_DmaHandle.Init.Direction	= DMA_PERIPH_TO_MEMORY;
	Rx_DmaHandle.Init.FIFOMode	= DMA_FIFOMODE_ENABLE;
	Rx_DmaHandle.Init.PeriphInc	= DMA_PINC_DISABLE;
	Rx_DmaHandle.Init.MemInc	= DMA_MINC_ENABLE;
	Rx_DmaHandle.Init.PeriphDataAlignment	= DMA_PDATAALIGN_WORD;
	Rx_DmaHandle.Init.MemDataAlignment	= DMA_MDATAALIGN_WORD;
	Rx_DmaHandle.Init.Mode		= DMA_SxCR_PFCTRL;
	Rx_DmaHandle.Init.Priority		= DMA_PRIORITY_VERY_HIGH;
	Rx_DmaHandle.Init.FIFOMode	= DMA_FIFOMODE_ENABLE;
	Rx_DmaHandle.Init.FIFOThreshold	= DMA_FIFO_THRESHOLD_FULL;
	Rx_DmaHandle.Init.MemBurst		= DMA_MBURST_INC4;
	Rx_DmaHandle.Init.PeriphBurst	= DMA_PBURST_INC4;
	
	/* DMA2 Stream3  or Stream6 Config */
	return HAL_DMA_Init(&Rx_DmaHandle);

	/* DMA2 Stream3  or Stream6 enable */
	//__HAL_DMA_ENABLE_IT(&Rx_DmaHandle, DMA_IT_TC);
	//__HAL_DMA_ENABLE(&Rx_DmaHandle);
}

/**
  * @brief  DeInitializes the SDIO interface.
  * @param  None
  * @retval None
  */
void SD_DeInit(void)
{ 
	SD_LowLevel_DeInit();
}

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for data 
  *         transfer).
  * @param  None
  * @retval HAL_SD_ErrorTypedef: SD Card Error code.
  */
HAL_SD_ErrorTypedef SD_Init(void)
{
	/* SDIO Peripheral Low Level Init */
	NVIC_Configuration();

	SD_LowLevel_Init();

	SD_DeInit();
	
	SDIO_HandleStructure.Instance 	= SDIO;
	SDIO_HandleStructure.CardType 	= CardType;
	SDIO_HandleStructure.hdmarx	= &Rx_DmaHandle;
	SDIO_HandleStructure.hdmatx		= &Tx_DmaHandle;
	SDIO_HandleStructure.Init.ClockEdge           	= SDIO_CLOCK_EDGE_RISING;
	SDIO_HandleStructure.Init.ClockBypass         	= SDIO_CLOCK_BYPASS_DISABLE;
	SDIO_HandleStructure.Init.ClockPowerSave     = SDIO_CLOCK_POWER_SAVE_DISABLE;
	SDIO_HandleStructure.Init.BusWide             	= SDIO_BUS_WIDE_1B;
	SDIO_HandleStructure.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
	SDIO_HandleStructure.Init.ClockDiv            	= SDIO_INIT_CLK_DIV;
	
	return HAL_SD_Init(&SDIO_HandleStructure, &SDCardInfo);
}

/**
  * @brief  Converts the number of bytes in power of two and returns the power.
  * @param  NumberOfBytes: number of bytes.
  * @retval None
  */
uint8_t convert_from_bytes_to_power_of_two(uint16_t NumberOfBytes)
{
  uint8_t count = 0;

  while (NumberOfBytes != 1)
  {
    NumberOfBytes >>= 1;
    count++;
  }
  return(count);
}
