#include "i2s.h"

DMA_HandleTypeDef 	I2S_Tx_DmaHandle;
I2S_HandleTypeDef 	I2S_HandleStructure;

void I2S_NVIC_Configuration(void);
void I2S_LowLevel_Init(uint32_t SD_I2S_AudioFreq);
void I2S_DMA_Init(void);

void Audio_MAL_Play(uint16_t * Addr, uint32_t Size)
{
	uint32_t pcm_size = Size/2;
    /* Configure the DMA Stream with the new parameters */
	I2S_HandleStructure.hdmatx			= &I2S_Tx_DmaHandle;
	
	/* DMA Tx Complete Interrupt Enable */
	__HAL_DMA_ENABLE_IT(&I2S_Tx_DmaHandle, DMA_IT_TC);
	HAL_I2S_Transmit_DMA(&I2S_HandleStructure, (uint16_t *)Addr, pcm_size);
}

void I2S_Configuration(uint32_t SD_I2S_AudioFreq)
{
	I2S_NVIC_Configuration();
	I2S_LowLevel_Init(SD_I2S_AudioFreq);
	I2S_DMA_Init();
}

void I2S_NVIC_Configuration(void)
{
	/* Configure the NVIC Preemption Priority Bits */
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}

void I2S_LowLevel_Init(uint32_t SD_I2S_AudioFreq)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	__HAL_RCC_I2S_CONFIG(RCC_I2SCLKSOURCE_PLLI2S);
	__HAL_RCC_PLLI2S_CONFIG(PLLI2S_N, PLLI2S_R);
	__HAL_RCC_PLLI2S_ENABLE();
	
	/* Wait till PLLI2S is ready */
	while((RCC->CR & RCC_CR_PLLI2SRDY) == 0);
	
	/* GPIOC and GPIOB Periph clock enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* Configure PB.08 pins: WS pins */
	GPIO_InitStructure.Pin 			= GPIO_PIN_9 ;
	GPIO_InitStructure.Mode 		= GPIO_MODE_AF_PP;		// push-pull alternate
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_HIGH;	// 50 MHz
	GPIO_InitStructure.Alternate 	= GPIO_AF5_SPI2;
	GPIO_InitStructure.Pull  		= GPIO_NOPULL;			// no pull
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure PB.10 SCK line */
	GPIO_InitStructure.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure PC.3 SD line */
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure I2S */
	/* Enable the I2S APB1 Clock */
	__HAL_I2S_DISABLE_IT(&I2S_HandleStructure, I2S_IT_TXE);
	
	HAL_I2S_DeInit(&I2S_HandleStructure);
	I2S_HandleStructure.Instance 				= SPI2;
	I2S_HandleStructure.Init.Mode 			= I2S_MODE_MASTER_TX;
	I2S_HandleStructure.Init.Standard 			= I2S_STANDARD_PHILIPS;
	I2S_HandleStructure.Init.DataFormat 		= I2S_DATAFORMAT_16B;
	I2S_HandleStructure.Init.MCLKOutput 		= I2S_MCLKOUTPUT_DISABLE;
	I2S_HandleStructure.Init.AudioFreq 		= SD_I2S_AudioFreq;
	I2S_HandleStructure.Init.CPOL 			= I2S_CPOL_LOW;
	I2S_HandleStructure.Init.ClockSource 		= I2S_CLOCK_PLL;
	I2S_HandleStructure.Init.FullDuplexMode 	= I2S_FULLDUPLEXMODE_DISABLE;	
	
	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_I2S_ENABLE(&I2S_HandleStructure);
	HAL_I2S_Init(&I2S_HandleStructure);
}

void I2S_DMA_Init(void)
{
	/* Enable the DMA1 Clock */
	__HAL_RCC_DMA1_CLK_ENABLE();
	
	I2S_Tx_DmaHandle.Instance 		= AUDIO_MAL_DMA_STREAM;
	
	__HAL_DMA_CLEAR_FLAG(&I2S_Tx_DmaHandle, I2S_DMA_FLAG_FEIF);
	__HAL_DMA_CLEAR_FLAG(&I2S_Tx_DmaHandle, I2S_DMA_FLAG_DMEIF);
	__HAL_DMA_CLEAR_FLAG(&I2S_Tx_DmaHandle, I2S_DMA_FLAG_TEIF);
	__HAL_DMA_CLEAR_FLAG(&I2S_Tx_DmaHandle, I2S_DMA_FLAG_HTIF);
	__HAL_DMA_CLEAR_FLAG(&I2S_Tx_DmaHandle, I2S_DMA_FLAG_TCIF);
	
	/* DMA2 Stream3  or Stream6 disable */
	__HAL_DMA_DISABLE(&I2S_Tx_DmaHandle);
	HAL_DMA_DeInit(&I2S_Tx_DmaHandle);

	I2S_Tx_DmaHandle.Parent			= &I2S_HandleStructure;
	I2S_Tx_DmaHandle.Init.Channel 	= DMA_CHANNEL_0;
	I2S_Tx_DmaHandle.Init.Direction	= DMA_MEMORY_TO_PERIPH;
	I2S_Tx_DmaHandle.Init.PeriphInc	= DMA_PINC_DISABLE;
	I2S_Tx_DmaHandle.Init.MemInc	= DMA_MINC_ENABLE;
	I2S_Tx_DmaHandle.Init.PeriphDataAlignment	= DMA_PDATAALIGN_HALFWORD;
	I2S_Tx_DmaHandle.Init.MemDataAlignment		= DMA_MDATAALIGN_HALFWORD;
	
	I2S_Tx_DmaHandle.Init.Mode		= DMA_NORMAL;
	I2S_Tx_DmaHandle.Init.Priority		= DMA_PRIORITY_HIGH;
	I2S_Tx_DmaHandle.Init.FIFOMode	= DMA_FIFOMODE_ENABLE;
	I2S_Tx_DmaHandle.Init.FIFOThreshold	= DMA_FIFO_THRESHOLD_FULL;
	I2S_Tx_DmaHandle.Init.MemBurst		= DMA_MBURST_SINGLE;
	I2S_Tx_DmaHandle.Init.PeriphBurst	= DMA_PBURST_SINGLE;
	
	/* DMA Config */
	HAL_DMA_Init(&I2S_Tx_DmaHandle);
}

void DMA1_Stream4_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&I2S_Tx_DmaHandle);
}

