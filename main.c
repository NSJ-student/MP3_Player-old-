#include "main.h"

 /*
	System Clock Configuration
      System Clock source            = PLL (HSE)
      SYSCLK(Hz)                     = 168000000
      HCLK(Hz)                       = 2168000000
      AHB Prescaler                  = 1
      APB1 Prescaler                 = 4
      APB2 Prescaler                 = 2
      HSE Frequency(Hz)              = +000000
      PLL_M                          = 8
      PLL_N                          = 336
      PLL_P                          = 2
      PLL_Q                          = 7
      VDD(V)                         = 3.3
      Main regulator output voltage  = Scale1 mode
      Flash Latency(WS)              = 7
  */
static void SystemClock_Config (void) 
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	/* Enable Power Control clock */
	__PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the
	device is clocked below the maximum system frequency (see datasheet). */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType 	= RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState 		= RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState 	= RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM 		= 8;
	RCC_OscInitStruct.PLL.PLLN 		= 336;
	RCC_OscInitStruct.PLL.PLLP 		= RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ 		= 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	clocks dividers */
	RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource 	= RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}   
/*
void TIM2_Init(void)
{
	TIM_HandleTypeDef TIM_HandleStruct;
	TIM_ClockConfigTypeDef TIM_Config;
	
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	TIM_HandleStruct.Instance 	= TIM2_BASE;
	TIM_Config.ClockSource 		= TIM_CLOCKSOURCE_INTERNAL;
	
	HAL_TIM_ConfigClockSource(&TIM_HandleStruct, &TIM_Config);
	
	TIM_HandleStruct.Instance 	= TIM2_BASE;
	TIM_HandleStruct.Channel 	= ;
	TIM_HandleStruct.hdma 		= ;
	
	TIM_HandleStruct.Init.ClockDivision 	= ;
	TIM_HandleStruct.Init.CounterMode 	= ;
	TIM_HandleStruct.Init.Period 		= ;
	TIM_HandleStruct.Init.Prescaler 	= ;
	TIM_HandleStruct.Init.RepetitionCounter = ;
	TIM_CLOCKSOURCE_INTERNAL
	TIM_HandleStruct.Lock 		= ;
	TIM_HandleStruct.State		= HAL_TIM_STATE_RESET;
	HAL_TIM_Base_Init(&TIM_HandleStruct);
}
*/
volatile TOUCH_STATE touch_state = TOUCH_NONE;
volatile PLAY_STATE play_state = PLAY_NONE;
volatile uint32_t play_diaplay_cnt = 0;
uint32_t audio_total_size = 0;
uint32_t audio_play_size = 0;
uint8_t play_str[20] = {0};

uint8_t	volume = 100;
int8_t	volume_str[20]={0};

uint32_t FreadByte = 0;
uint8_t audio_buffer1[512];
uint8_t audio_buffer2[512];
uint8_t image_buffer[900*2];

int PlayMp3(FIL * file)
{
//	uint32_t index, cnt;	  
//	uint32_t filesize=file->fsize;
		
	while(  MP3_DREQ ==0 );   
	f_read (file, audio_buffer1, 512, &FreadByte);
	audio_play_size += FreadByte;
	VS1003_WriteData_DMA((uint8_t *)audio_buffer1, FreadByte);
	f_read (file, audio_buffer2, 512, &FreadByte);
	audio_play_size += FreadByte;
	
//	filesize -= cnt;
//	while(filesize > 0)
//	{
//		f_read (file, audio_buffer1, 512, &cnt);
//		filesize -= cnt;
//		 for( index = 0; index < cnt; index += 32 )
//		 {
//			 while(  MP3_DREQ ==0 );    
//			 VS1003_WriteData( (void*)( audio_buffer1 + index ) );   
//		 }
//	}
	
	return -1;
}

int main(void)
{
	int x, y;
	uint16_t * p_image;
	uint32_t index, cnt;	   
	uint16_t validx=0,validy=0;
	FIL fileR;      // file objects
	FILINFO *	info;
	MP3_ID3v2 * p_id3 = (MP3_ID3v2 *)audio_buffer1;
	

	HAL_Init();
	SystemClock_Config();
	
	VS1003_Init();
	VS1003_Reset();
	VS1003_SoftReset();
	VS1003_SineTest();
	SPI3_DMA_Init();
	sprintf(volume_str, "vol: %3d", volume);
	VS1003_WriteReg(SPI_VOL,  (volume | (volume<<8))  );
	
//	f_read (file, buffer, 512, &cnt);
//	VS1003_WriteData_DMA((uint8_t *)MP3_DATA, sizeof(MP3_DATA));
//	while(VS1003_Cplt==0);   
//	 for( index = 0; index < sizeof(MP3_DATA); index += 32 )
//	 {	 
//		 while(  MP3_DREQ ==0 );     /* 된덤왕國 */
//		 VS1003_WriteData( (void*)( MP3_DATA + index ) );   
// 	 }	

	TP_Init();
	LCD_Initializtion();
	LCD_Clear(Black);
  	TouchPanel_Calibrate();
	FileSystem_Init();
	DisplayCurrentDir();
	
	while(1)
	{
		if(play_state > PLAY_NONE)
		{
			play_diaplay_cnt++;
			if(play_diaplay_cnt>0xFFF)
			{
				play_diaplay_cnt = 0;
				sprintf(play_str, "Comp: %3d per", 100*audio_play_size/audio_total_size);
				GUI_Text(PROGRESS_POS_X,PROGRESS_POS_Y,(uint8_t*)play_str,Red,Black);
			}
			if(VS1003_WriteData_DMA_ING() == 0)
			{
				if(play_state == PLAY_ON1)
				{
					play_state = PLAY_ON2;
					VS1003_WriteData_DMA((uint8_t *)audio_buffer2, FreadByte);
					f_read (&fileR, audio_buffer1, 512, &FreadByte);
					audio_play_size += FreadByte;
				}
				else
				{
					play_state = PLAY_ON1;
					VS1003_WriteData_DMA((uint8_t *)audio_buffer1, FreadByte);
					f_read (&fileR, audio_buffer2, 512, &FreadByte);
					audio_play_size += FreadByte;
				}
				
				if(FreadByte == 0)
				{
					play_state = PLAY_NONE;
					play_diaplay_cnt = 0;
					audio_play_size = 0;
					GUI_Text(10,10,"Stop Play.....",Green,Black);
				}
			}
		}
		
		if(!TP_INT_IN)	// 터치 IC에서 인터럽트 신호가 있을 때
		{
			getDisplayPoint(&display, Read_Ads7846(), &matrix );
			if((display.y <= MAX_Y)&&(display.x <= MAX_X))	// 터치값이 화면범위 안에 있을 때
			{
				touch_state = TOUCH_ON;		// 나중에 터치를 처리하도록 하는 플래그
				validx = display.x;	// 좌표값 저장
				validy = display.y;
			}
		}
		else if(touch_state == TOUCH_ON)	// 터치 인터럽트 신호가 없으면서 
		{									// 아직 처리하지 않은 터치 값이 존재할 때
			
			touch_state = TOUCH_OFF;	// 터치 플래그를 끈다(터치를 처리한 후 같은 값에 대해 다시 실행되지 않도록)
				
//			sprintf(volume_str, "X:%3d,Y:%3d", tempx, validy);
//			GUI_Text(10,10,(uint8_t*)volume_str,Red,Black);
//			TP_DrawPoint(validx, validy);
			info = PosToFInfo(validx, validy);
			if(info != (FILINFO *)-1)
			{
				if(info == (FILINFO *)0)
				{
					if(ToUpperDir() == FR_OK)
					{
						DisplayCurrentDir();
					}
				}
				if(info == (FILINFO *)1)
				{
					DisplayCurrentDir();
				}
				if(info->fattrib == AM_ARC)
				{
					if( CheckExtension(info->fname, "MP3") )
					{
						if(f_open(&fileR, GetFilePath(info->fname), FA_READ) ==FR_OK)
						{
							GUI_Text(10,10,(uint8_t*)GetFilePath(info->fname),Red,Black);
							play_state = PLAY_ON1;
							
							f_read (&fileR, audio_buffer1, 512, &FreadByte);
							cnt = 10;
							cnt += audio_buffer1[ID3v2_SIZE_POS+3];
							cnt += audio_buffer1[ID3v2_SIZE_POS+2]<<7;
							cnt += audio_buffer1[ID3v2_SIZE_POS+1]<<14;
							cnt += audio_buffer1[ID3v2_SIZE_POS]<<21;
							if((audio_buffer1[ID3v2_FLAGS_POS])&FOOTER_PRESEND_FLAG)
							{
								cnt += 10;
							}
							audio_total_size = fileR.fsize - cnt;
							f_lseek(&fileR, cnt);
							PlayMp3(&fileR);
							sprintf(play_str, "Comp: %3d per", 100*audio_play_size/audio_total_size);
							GUI_Text(PROGRESS_POS_X,PROGRESS_POS_Y,(uint8_t*)play_str,Red,Black);
							play_diaplay_cnt = 0;
							audio_play_size = 0;

						}
					}
					else if( CheckExtension(info->fname, "WAV") )
					{
						if(f_open(&fileR, GetFilePath(info->fname), FA_READ) ==FR_OK)
						{
							GUI_Text(10,10,(uint8_t*)GetFilePath(info->fname),Red,Black);
							play_state = PLAY_ON1;
							
							PlayMp3(&fileR);
							sprintf(play_str, "Comp: %3d per", 100*audio_play_size/audio_total_size);
							GUI_Text(PROGRESS_POS_X,PROGRESS_POS_Y,(uint8_t*)play_str,Red,Black);
							audio_total_size = fileR.fsize;
							play_diaplay_cnt = 0;
							audio_play_size = 0;
						}
					}
					else if(CheckExtension(info->fname, "BIN"))
					{
	
						if(f_open(&fileR,GetFilePath(info->fname), FA_READ) ==FR_OK)
						{
							if(f_read(&fileR, image_buffer, 30*30*2+2, &FreadByte) == FR_OK)
							{
								p_image = (uint16_t *)(&image_buffer[2]);
								for(y=0; y<image_buffer[1]; y++)
								{
									for(x=0; x<image_buffer[0]; x++)
									{
										LCD_SetPoint(x, y, p_image[x + y*image_buffer[0]]);
									}
								}
							}
						}
					}
//					if(f_open(&fileR, GetFilePath(info->fname), FA_READ) ==FR_OK)
//					{
//						GUI_Text(10,210,"Now Playing...",Blue,Black);
//						GUI_Text(10,10,(uint8_t*)GetFilePath(info->fname),Red,Black);
//						PlayWav(&fileR);
//						TP_Init();
//						GUI_Text(10,210,"Stop Play.....",Green,Black);
//					}
				}
				if(info->fattrib == AM_DIR)
				{
					if(ToDownDir(info->fname) == FR_OK)
					{
						DisplayCurrentDir();
//						delay_ms(100);
					}
				}
			}
			else
			{
				index = PosToCtrl(validx, validy);
				if(index == 1)
				{
					if(volume < 240)
					{
						volume -= 10;
						sprintf(volume_str, "vol: %3d", volume);
						GUI_Text(VOLUME_POS_X,VOLUME_POS_Y,(uint8_t*)volume_str,Red,Black);
						VS1003_WriteReg(SPI_VOL,  (volume | (volume<<8))  );
					}
				}
				else if(index == 2)
				{
					if(volume > 0)
					{
						volume += 10;
						sprintf(volume_str, "vol: %3d", volume);
						GUI_Text(VOLUME_POS_X,VOLUME_POS_Y,(uint8_t*)volume_str,Red,Black);
						VS1003_WriteReg(SPI_VOL,  (volume | (volume<<8))  );
					}
				}
				else if(index == 3)
				{
					if(play_state != PLAY_NONE)
					{
						play_state = PLAY_NONE;
						play_diaplay_cnt = 0;
						audio_play_size = 0;
						VS1003_Reset();
						VS1003_WriteReg(SPI_VOL,  (volume | (volume<<8))  );
//						VS1003_StopPlay();
						GUI_Text(10,10,"Stop Play.....",Green,Black);
						delay_ms(100);
					}
				}
			}
		}
	}
	
	return 0;
}
