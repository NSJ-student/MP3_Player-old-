#ifndef __I2S_H
#define __I2S_H

#include <stm32f4xx_hal.h>

#define PLLI2S_N   258
#define PLLI2S_R   3

 #define I2S_DMA_FLAG_FEIF         	DMA_FLAG_FEIF3_7
 #define I2S_DMA_FLAG_DMEIF    		DMA_FLAG_DMEIF3_7
 #define I2S_DMA_FLAG_TEIF         	DMA_FLAG_TEIF3_7
 #define I2S_DMA_FLAG_HTIF         	DMA_FLAG_HTIF3_7
 #define I2S_DMA_FLAG_TCIF         	DMA_FLAG_TCIF3_7
 
/* I2S DMA Stream definitions */
#define AUDIO_MAL_DMA_CLOCK			RCC_AHB1Periph_DMA1
#define AUDIO_MAL_DMA_STREAM		DMA1_Stream4
#define AUDIO_MAL_DMA_CHANNEL		DMA_CHANNEL_0
#define AUDIO_MAL_DMA_IRQ              	DMA1_Stream4_IRQn
#define AUDIO_MAL_DMA_FLAG_TC      	DMA_FLAG_TCIF4
#define AUDIO_MAL_DMA_FLAG_HT		DMA_FLAG_HTIF4
#define AUDIO_MAL_DMA_FLAG_FE		DMA_FLAG_FEIF4
#define AUDIO_MAL_DMA_FLAG_TE		DMA_FLAG_TEIF4
#define AUDIO_MAL_DMA_FLAG_DME		DMA_FLAG_DMEIF4
#define AUDIO_MAL_DMA_PERIPH_DATA_SIZE	DMA_PeripheralDataSize_HalfWord
#define AUDIO_MAL_DMA_MEM_DATA_SIZE		DMA_MemoryDataSize_HalfWord
#define DMA_MAX_SZE		0xFFFF

#define Audio_MAL_IRQHandler           DMA1_Stream4_IRQHandler

void I2S_Configuration(uint32_t SD_I2S_AudioFreq);
void Audio_MAL_Play(uint16_t * Addr, uint32_t Size);

#endif
