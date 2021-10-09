#ifndef __I2C_H
#define __I2C_H

#include <stm32f4xx_hal.h>


/* Private define ------------------------------------------------------------*/
#define I2C_PAGESIZE	4


/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define I2C_x_FLAG_TIMEOUT         				10000


#define I2Cx_SPEED               					30000
#define I2Cx_SLAVE_ADDRESS7      					0x33

#define I2C_x                        		I2C1
#define I2C_x_CLK                    	RCC_APB1Periph_I2C1

#define I2C_x_SDA_PIN                 	GPIO_PIN_7
#define I2C_x_SDA_GPIO_PORT     GPIOB
#define I2C_x_SDA_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define I2C_x_SDA_SOURCE          GPIO_PinSource7
#define I2C_x_SDA_AF                  	GPIO_AF_I2C1

#define I2C_x_SCL_PIN                 	GPIO_PIN_6
#define I2C_x_SCL_GPIO_PORT     GPIOB
#define I2C_x_SCL_GPIO_CLK        RCC_AHB1Periph_GPIOB
#define I2C_x_SCL_SOURCE          GPIO_PinSource6
#define I2C_x_SCL_AF                  	GPIO_AF_I2C1

void CODEC_I2C_Configuration(void);
HAL_StatusTypeDef I2C_Write(uint16_t DevAddress, uint8_t *pData, uint16_t Size);

#endif
