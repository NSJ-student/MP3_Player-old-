#include "i2c.h"

I2C_HandleTypeDef 	I2C_HandleStructure;
	
void CODEC_I2C_Configuration(void)
{
	GPIO_InitTypeDef  		GPIO_InitStructure;

	/* GPIOB Periph clock enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	/* Configure PB.06, 7 pins: WS pins */
	GPIO_InitStructure.Pin 			= I2C_x_SDA_PIN | I2C_x_SCL_PIN;
	GPIO_InitStructure.Mode 		= GPIO_MODE_AF_OD;		// open-drain alternate
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_HIGH;	// 50 MHz
	GPIO_InitStructure.Alternate 	= GPIO_AF4_I2C1;
	GPIO_InitStructure.Pull  		= GPIO_NOPULL;			// no pull
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* I2C Periph clock enable */
	__HAL_RCC_I2C1_CLK_ENABLE();
	
	HAL_I2C_DeInit(&I2C_HandleStructure);
	
	I2C_HandleStructure.Instance 	= I2C1;
	I2C_HandleStructure.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
	I2C_HandleStructure.Init.ClockSpeed 			= I2Cx_SPEED;
	I2C_HandleStructure.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
	I2C_HandleStructure.Init.DutyCycle 			= I2C_DUTYCYCLE_2;
	I2C_HandleStructure.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
	I2C_HandleStructure.Init.NoStretchMode 		= I2C_NOSTRETCH_DISABLE;
	I2C_HandleStructure.Init.OwnAddress1 		= I2Cx_SLAVE_ADDRESS7;
	I2C_HandleStructure.Mode					= HAL_I2C_MODE_MASTER;
	
	HAL_I2C_Init(&I2C_HandleStructure);
}

HAL_StatusTypeDef I2C_Write(uint16_t DevAddress, uint8_t *pData, uint16_t Size)
{
	return HAL_I2C_Master_Transmit(&I2C_HandleStructure, 
								DevAddress, pData, Size, 0xFFFF);
}
