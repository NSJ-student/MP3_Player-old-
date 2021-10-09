/*********************************************************************************************************
*
* File                : TouchPanel.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.20
* Version             : V1.0
* By                  : 
*
*                                  (c) Copyright 2005-2011, WaveShare
*                                       http://www.waveshare.net
*                                          All Rights Reserved
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "TouchPanel/TouchPanel.h"
//#include "TouchPanel/config.h"
#include "LCD/LCD.h"

/* Private variables ---------------------------------------------------------*/
Matrix matrix ;
Coordinate  display ;
SPI_HandleTypeDef SPI_HandleStruct;


Coordinate ScreenSample[3];

Coordinate DisplaySample[3] = {
					{ 50,  45 },
					{ 290, 100},
					{ 160,210}
			};

/* Private define ------------------------------------------------------------*/
#define THRESHOLD 2


/*******************************************************************************
* Function Name  : ADS7843_SPI_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void ADS7843_SPI_Init(void) 
{ 
	__HAL_RCC_SPI2_CLK_ENABLE();

	//SPI_I2S_DeInit(Open_SPI);
	
	SPI_HandleStruct.Instance		= SPI2;
	SPI_HandleStruct.State			= HAL_SPI_STATE_RESET;
	/* Open_SPI Config -------------------------------------------------------------*/ 
	SPI_HandleStruct.Init.Direction 	= SPI_DIRECTION_2LINES; 
	SPI_HandleStruct.Init.Mode 		= SPI_MODE_MASTER; 
	SPI_HandleStruct.Init.DataSize 	= SPI_DATASIZE_8BIT; 
	SPI_HandleStruct.Init.CLKPolarity 	= SPI_POLARITY_LOW; 
	SPI_HandleStruct.Init.CLKPhase 	= SPI_PHASE_1EDGE; 
	SPI_HandleStruct.Init.NSS 			= SPI_NSS_SOFT; 
	SPI_HandleStruct.Init.BaudRatePrescaler 	= SPI_BAUDRATEPRESCALER_32; 
	SPI_HandleStruct.Init.FirstBit 		= SPI_FIRSTBIT_MSB; 
	SPI_HandleStruct.Init.TIMode		= SPI_TIMODE_DISABLE;
	SPI_HandleStruct.Init.CRCCalculation	= SPI_CRCCALCULATION_DISABLE;
	SPI_HandleStruct.Init.CRCPolynomial 	= 7; 

	HAL_SPI_Init(&SPI_HandleStruct);
	__HAL_SPI_ENABLE(&SPI_HandleStruct);
} 

/*******************************************************************************
* Function Name  : TP_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_Init(void) 
{ 	
	GPIO_InitTypeDef GPIO_InitStructure; 
	// RCC init
	__HAL_RCC_GPIOB_CLK_ENABLE();		// SCK, MISO, MOSI
	__HAL_RCC_GPIOC_CLK_ENABLE();		// CS
	__HAL_RCC_SPI2_CLK_ENABLE();			// SPI

	GPIO_InitStructure.Pin 			= GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 ;	// SCK, MISO, MOSI
	GPIO_InitStructure.Mode 		= GPIO_MODE_AF_PP;		// push-pull alternate
	GPIO_InitStructure.Speed 		= GPIO_SPEED_FREQ_VERY_HIGH;	// 100 MHz
	GPIO_InitStructure.Alternate 	= GPIO_AF5_SPI2;
	GPIO_InitStructure.Pull  		= GPIO_PULLUP;			// pull up
	
	HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin 			= GPIO_PIN_4;		// CS
	GPIO_InitStructure.Mode 		= GPIO_MODE_OUTPUT_PP;	// push-pull output
	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.Pin 			= GPIO_PIN_5;		// IRQ
	GPIO_InitStructure.Mode 		= GPIO_MODE_INPUT;		// input
	
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  	TP_CS(1); 
  	ADS7843_SPI_Init(); 
} 





/*******************************************************************************
* Function Name  : DelayUS
* Description    : 
* Input          : - cnt:
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/

static void DelayUS(volatile uint32_t cnt)
{
  uint16_t i;
  for(i = 0;i<cnt;i++)
  {
     uint8_t us = 12;
     while (us--)
     {
       ;   
     }
  }
}


/*******************************************************************************
* Function Name  : WR_CMD
* Description    : 
* Input          : - cmd: 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void WR_CMD (uint8_t cmd)  
{ 
	uint8_t buff_tx= cmd;
	uint8_t buff_rx;
	/* Wait for SPI2 Tx buffer empty */ 
	//while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); 
	/* Send SPI2 data */ 
	
	/* Wait for SPI2 data reception */ 
	//while (SPI_I2S_GetFlagStatus(Open_SPI, SPI_I2S_FLAG_RXNE) == RESET); 
	/* Read Open_SPI received data */ 
	
	HAL_SPI_TransmitReceive(&SPI_HandleStruct, &buff_tx, &buff_rx, 1, 100);
	// Timeout : 전송 대기 시간(systick 단위)
} 



/*******************************************************************************
* Function Name  : RD_AD
* Description    : 
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
static int RD_AD(void)  
{ 
	uint8_t 		buff_tx=0, buff_rx=0;
	uint16_t 	result=0;
	
	HAL_SPI_TransmitReceive(&SPI_HandleStruct, &buff_tx, &buff_rx, 1, 100);
	result=buff_rx<<8; 
	DelayUS(1);
	HAL_SPI_TransmitReceive(&SPI_HandleStruct, &buff_tx, &buff_rx, 1, 100);
	result |= buff_rx; 
	result >>= 3; 
	result &= 0xfff; 
	return result; 
} 


/*******************************************************************************
* Function Name  : Read_X
* Description    : Read ADS7843 ADC X 
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
int Read_X(void)  
{  
  int i; 
  TP_CS(0); 
  DelayUS(1); 
  WR_CMD(CHX); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS(1); 
  return i;    
} 

/*******************************************************************************
* Function Name  : Read_Y
* Description    : Read ADS7843 ADC Y
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
int Read_Y(void)  
{  
  int i; 
  TP_CS(0); 
  DelayUS(1); 
  WR_CMD(CHY); 
  DelayUS(1); 
  i=RD_AD(); 
  TP_CS(1); 
  return i;     
} 


/*******************************************************************************
* Function Name  : TP_GetAdXY
* Description    : Read ADS7843
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
void TP_GetAdXY(int *x,int *y)  
{ 
  int adx,ady; 
  adx=Read_X(); 
  DelayUS(1); 
  ady=Read_Y(); 
  *x=adx; 
  *y=ady; 
} 

/*******************************************************************************
* Function Name  : TP_DrawPoint
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TP_DrawPoint(uint16_t Xpos,uint16_t Ypos)
{
  LCD_SetPoint(Xpos,Ypos,Blue);     /* Center point */
  LCD_SetPoint(Xpos+1,Ypos,Blue);
  LCD_SetPoint(Xpos,Ypos+1,Blue);
  LCD_SetPoint(Xpos+1,Ypos+1,Blue);	
}	

/*******************************************************************************
* Function Name  : DrawCross
* Description    : 
* Input          : - Xpos: Row Coordinate
*                  - Ypos: Line Coordinate 
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void DrawCross(uint16_t Xpos,uint16_t Ypos)
{
  LCD_DrawLine(Xpos-15,Ypos,Xpos-2,Ypos,0xffff);
  LCD_DrawLine(Xpos+2,Ypos,Xpos+15,Ypos,0xffff);
  LCD_DrawLine(Xpos,Ypos-15,Xpos,Ypos-2,0xffff);
  LCD_DrawLine(Xpos,Ypos+2,Xpos,Ypos+15,0xffff);
  
  LCD_DrawLine(Xpos-15,Ypos+15,Xpos-7,Ypos+15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos-15,Ypos+7,Xpos-15,Ypos+15,RGB565CONVERT(184,158,131));

  LCD_DrawLine(Xpos-15,Ypos-15,Xpos-7,Ypos-15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos-15,Ypos-7,Xpos-15,Ypos-15,RGB565CONVERT(184,158,131));

  LCD_DrawLine(Xpos+7,Ypos+15,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos+15,Ypos+7,Xpos+15,Ypos+15,RGB565CONVERT(184,158,131));

  LCD_DrawLine(Xpos+7,Ypos-15,Xpos+15,Ypos-15,RGB565CONVERT(184,158,131));
  LCD_DrawLine(Xpos+15,Ypos-15,Xpos+15,Ypos-7,RGB565CONVERT(184,158,131));	  	
}	
	
/*******************************************************************************
* Function Name  : Read_Ads7846
* Description    : Get TouchPanel X Y
* Input          : None
* Output         : None
* Return         : Coordinate *
* Attention		 : None
*******************************************************************************/
Coordinate *Read_Ads7846(void)
{
  static Coordinate  screen;
  int m0,m1,m2,TP_X[1],TP_Y[1],temp[3];
  uint8_t count=0;
  int buffer[2][9]={{0},{0}};
  
  do
  {		   
    TP_GetAdXY(TP_X,TP_Y);  
	buffer[0][count]=TP_X[0];  
	buffer[1][count]=TP_Y[0];
	count++;  
  }
  while(!TP_INT_IN&& count<9);  /* TP_INT_IN  */
  if(count==9)   /* Average X Y  */ 
  {
	/* Average X  */
  temp[0]=(buffer[0][0]+buffer[0][1]+buffer[0][2])/3;
	temp[1]=(buffer[0][3]+buffer[0][4]+buffer[0][5])/3;
	temp[2]=(buffer[0][6]+buffer[0][7]+buffer[0][8])/3;

	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];

	m0=m0>0?m0:(-m0);
  m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);

	if( m0>THRESHOLD  &&  m1>THRESHOLD  &&  m2>THRESHOLD ) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.x=(temp[0]+temp[2])/2;
	  else 
	    screen.x=(temp[0]+temp[1])/2;	
	}
	else if(m2<m1) 
	  screen.x=(temp[0]+temp[2])/2;
	else 
	  screen.x=(temp[1]+temp[2])/2;

	/* Average Y  */
  temp[0]=(buffer[1][0]+buffer[1][1]+buffer[1][2])/3;
	temp[1]=(buffer[1][3]+buffer[1][4]+buffer[1][5])/3;
	temp[2]=(buffer[1][6]+buffer[1][7]+buffer[1][8])/3;
	m0=temp[0]-temp[1];
	m1=temp[1]-temp[2];
	m2=temp[2]-temp[0];
	m0=m0>0?m0:(-m0);
	m1=m1>0?m1:(-m1);
	m2=m2>0?m2:(-m2);
	if(m0>THRESHOLD&&m1>THRESHOLD&&m2>THRESHOLD) return 0;

	if(m0<m1)
	{
	  if(m2<m0) 
	    screen.y=(temp[0]+temp[2])/2;
	  else 
	    screen.y=(temp[0]+temp[1])/2;	
    }
	else if(m2<m1) 
	   screen.y=(temp[0]+temp[2])/2;
	else
	   screen.y=(temp[1]+temp[2])/2;

	return &screen;
  }  
  return 0; 
}
	 

/*******************************************************************************
* Function Name  : setCalibrationMatrix
* Description    : Calculate K A B C D E F
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
FunctionalState setCalibrationMatrix( Coordinate * displayPtr,
                          Coordinate * screenPtr,
                          Matrix * matrixPtr)
{

  FunctionalState retTHRESHOLD = ENABLE ;
  /* K＝(X0－X2) (Y1－Y2)－(X1－X2) (Y0－Y2) */
  matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                       ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
  if( matrixPtr->Divider == 0 )
  {
    retTHRESHOLD = DISABLE;
  }
  else
  {
    /* A＝((XD0－XD2) (Y1－Y2)－(XD1－XD2) (Y0－Y2))／K	*/
    matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;
	/* B＝((X0－X2) (XD1－XD2)－(XD0－XD2) (X1－X2))／K	*/
    matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) - 
                    ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* C＝(Y0(X2XD1－X1XD2)+Y1(X0XD2－X2XD0)+Y2(X1XD0－X0XD1))／K */
    matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;
    /* D＝((YD0－YD2) (Y1－Y2)－(YD1－YD2) (Y0－Y2))／K	*/
    matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) - 
                    ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;
    /* E＝((X0－X2) (YD1－YD2)－(YD0－YD2) (X1－X2))／K	*/
    matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) - 
                    ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;
    /* F＝(Y0(X2YD1－X1YD2)+Y1(X0YD2－X2YD0)+Y2(X1YD0－X0YD1))／K */
    matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                    (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                    (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
  }
  return( retTHRESHOLD ) ;
}

/*******************************************************************************
* Function Name  : getDisplayPoint
* Description    : Touch panel X Y to display X Y
* Input          : None
* Output         : None
* Return         : 
* Attention		 : None
*******************************************************************************/
FunctionalState getDisplayPoint(Coordinate * displayPtr,
                     Coordinate * screenPtr,
                     Matrix * matrixPtr )
{
  FunctionalState retTHRESHOLD =ENABLE ;

  if( matrixPtr->Divider != 0 )
  {
    /* XD = AX+BY+C */        
    displayPtr->x = ( (matrixPtr->An * screenPtr->x) + 
                      (matrixPtr->Bn * screenPtr->y) + 
                       matrixPtr->Cn 
                    ) / matrixPtr->Divider ;
	/* YD = DX+EY+F */        
    displayPtr->y = ( (matrixPtr->Dn * screenPtr->x) + 
                      (matrixPtr->En * screenPtr->y) + 
                       matrixPtr->Fn 
                    ) / matrixPtr->Divider ;
  }
  else
  {
    retTHRESHOLD = DISABLE;
  }
  return(retTHRESHOLD);
} 

/*******************************************************************************
* Function Name  : delay_ms
* Description    : Delay Time
* Input          : - cnt: Delay Time
* Output         : None
* Return         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void delay_ms(uint16_t ms)    
{ 
	uint16_t i,j; 
	for( i = 0; i < ms; i++ )
	{ 
		for( j = 0; j < 0xffff; j++ );
	}
} 

/*******************************************************************************
* Function Name  : TouchPanel_Calibrate
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void TouchPanel_Calibrate(void)
{
  uint8_t i;
  Coordinate * Ptr;

  for(i=0;i<3;i++)
  {
   LCD_Clear(Black);
   GUI_Text(44,10,"Touch crosshair to calibrate",White,Black);
   delay_ms(300);
   DrawCross(DisplaySample[i].x,DisplaySample[i].y);
   do
   {
     Ptr=Read_Ads7846();
   }
   while( Ptr == (void*)0 );
   ScreenSample[i].x= Ptr->x; ScreenSample[i].y= Ptr->y;
  }
  setCalibrationMatrix( &DisplaySample[0],&ScreenSample[0],&matrix );
  LCD_Clear(Black);
} 

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
