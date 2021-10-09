#include "mp3gui.h"


uint8_t temp_buffer[BMP_MAX_WIDTH*BMP_MAX_HEIGHT*2];

void Draw_Object(const TCHAR * FilePath, const uint32_t XOffset, const uint32_t YOffset)
{
	FIL fileR;      // file objects
	uint8_t fileS[2];
	uint32_t ReadByte = 0;
	uint16_t * p_image = (uint16_t *)temp_buffer;
	uint32_t x, y;
	
	if(f_open(&fileR, FilePath, FA_READ) == FR_OK)
	{
		if(f_read(&fileR, fileS, 2, &ReadByte) == FR_OK)
		{
			if(f_read(&fileR, temp_buffer, fileS[0]*fileS[1]*2, &ReadByte) == FR_OK)
			{
				for(y=0; y<fileS[1]; y++)
				{
					for(x=0; x<fileS[0]; x++)
					{
						LCD_SetPoint(XOffset+x, YOffset+y, p_image[x + y*fileS[0]]);
					}
				}
			}
		}
		f_close(&fileR);
	}
}

void Draw_Rect(RECT * rect, uint16_t color)
{
	uint32_t x, y;
	
	for(x=0; x<rect->width; x++)
		LCD_SetPoint(rect->x + x, rect->y, color);
	
	for(x=0; x<rect->width; x++)
		LCD_SetPoint(rect->x + x, rect->y + rect->height - 1, color);
	
	for(y=0; y<rect->height; y++)
		LCD_SetPoint(rect->x, rect->y + y, color);
	
	for(y=0; y<rect->height; y++)
		LCD_SetPoint(rect->x + rect->width - 1, rect->y + y, color);
}

void Main_GUI(void)
{
	RECT rect;

	rect.x = 40;	rect.y = 50;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
	rect.x = 40;	rect.y = 66;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
	rect.x = 40;	rect.y = 82;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
	rect.x = 40;	rect.y = 98;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
	rect.x = 40;	rect.y = 114;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
	rect.x = 40;	rect.y = 130;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
	rect.x = 40;	rect.y = 30;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
#if (DRAW_BITMAP == 1)
	Draw_Object(PATH_PLAY, 200, 200);
	Draw_Object(PATH_VOLD, 230, 200);
	Draw_Object(PATH_VOLU, 260, 200);
#else
	rect.x = 200;	rect.y = 200;	rect.width = BMP_MAX_WIDTH;	rect.height = BMP_MAX_HEIGHT;
	Draw_Rect(&rect,Green);
	GUI_Text(rect.x+TEXT_OFFSET,rect.y+TEXT_OFFSET, "P",Green,Black);
	rect.x = 230;	rect.y = 200;	rect.width = BMP_MAX_WIDTH;	rect.height = BMP_MAX_HEIGHT;
	Draw_Rect(&rect,Blue);
	GUI_Text(rect.x+TEXT_OFFSET,rect.y+TEXT_OFFSET, "U",Blue,Black);
	rect.x = 260;	rect.y = 200;	rect.width = BMP_MAX_WIDTH;	rect.height = BMP_MAX_HEIGHT;
	Draw_Rect(&rect,Red);
	GUI_Text(rect.x+TEXT_OFFSET,rect.y+TEXT_OFFSET, "D",Red,Black);
#endif
	rect.x = 40;	rect.y = 146;	rect.width = 220;	rect.height = 16;
	Draw_Rect(&rect,Grey);
	rect.x = 210;	rect.y = 167;	rect.width = 50;	rect.height = 16;
	Draw_Rect(&rect,Grey);
//	rect.x = 40;	rect.y = 180;	rect.width = 100;	rect.height = 16;
//	Draw_Rect(&rect,Grey);
//	rect.x = 40;	rect.y = 200;	rect.width = 100;	rect.height = 16;
//	Draw_Rect(&rect,Grey);
}

void Play_GUI(void)
{
}
