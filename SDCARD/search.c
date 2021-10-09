#include "SDCARD/search.h"
#include "main.h"
#include <string.h>

/* Private functions ---------------------------------------------------------*/
uint8_t textFileBuffer[100] = "Thank you for using Waveshare Development Board!\r\n";

FATFS 		FileObject;		// Work area (file system object) for logical driv
FIL 			fsrc, fdst;		// file objects
BYTE 		buffer[512];	// file copy buffer
FRESULT 	res;			// FatFs function common result code
UINT 		br, bw;			// File R/W count

DIR 		curr_dir;
FILINFO	FileList[LIST_SIZE_MAX];


struct path_tag {
	uint32_t size;
	TCHAR path[100];
} dir_path = {3, {'0', ':', '\\', 0, }};


void FileSystem_Init(void)		// 파일 시스템 초기화
{
	SD_Init();
	delay_ms(10);
	f_mount(0, &FileObject);
	f_opendir(&curr_dir, dir_path.path);
	
	return;
}

FRESULT ToUpperDir(void)			// 이전 디렉토리로 이동
{
	int idx;
	
	for(idx=dir_path.size-1; idx > 0; idx--)
	{
		if(dir_path.path[idx] == '\\')
		{
			if(dir_path.path[idx-1] != ':')
			{
				dir_path.path[idx] = 0;
				dir_path.size = idx;
				break;
			}
			else
			{
				idx++;
				dir_path.path[idx] = 0;
				dir_path.size = idx;
				break;
			}
		}
	}
	
	return f_opendir(&curr_dir, dir_path.path);
}

FRESULT ToDownDir(TCHAR * dir_name)	// 내부 디렉토리로 이동
{
	int cnt = 0;
	
	if(dir_path.path[dir_path.size-1] != '\\')
	{
		dir_path.path[dir_path.size] = '\\';
		dir_path.size++;
	}
	
	for(cnt=0; *((TCHAR *)dir_name+cnt) != 0; cnt++)
	{
		dir_path.path[dir_path.size] = *(dir_name+cnt);
		dir_path.size++;
	}
	dir_path.path[dir_path.size] = 0;
	
	return f_opendir(&curr_dir, dir_path.path);
}

TCHAR * GetFilePath(TCHAR * name)
{
	static TCHAR FilePath[100] = {0,};
	uint32_t cnt = 0, offset = 0;
	
	memcpy(FilePath, dir_path.path, dir_path.size);
	if(dir_path.path[dir_path.size-1] != '\\')
	{
		FilePath[dir_path.size] = '\\';
		offset = 1;
	}
	
	for(cnt=0; *((TCHAR *)name+cnt) != 0; cnt++)
	{
		FilePath[dir_path.size+offset+cnt] = *(name+cnt);
	}
	FilePath[dir_path.size+offset+cnt] = 0;
	
	return FilePath;
}

uint32_t CheckExtension(TCHAR * FileName, const TCHAR * Ext)
{
	uint32_t cnt = 0;
	uint8_t idx=0;
	
	for(cnt=0; *(FileName+cnt) != '.'; cnt++)
	{
		if(cnt > 13)
		{
			return 0;
		}
	}
	cnt++;
	for(idx=0; idx<3; idx++)
	{
		if( *(FileName+cnt+idx) != *(Ext+idx) )
		{
			return 0;
		}
	}
	
	return 1;
}

FILINFO * PosToFInfo(uint16_t x_pos, uint16_t y_pos)
{
	uint32_t cnt;
	if((x_pos >= LIST_LEFT_POS) && (x_pos <= LIST_LEFT_POS+LIST_COMP_WIDTH))
	{
		if((y_pos <= LIST_PATH_TOP_POS+LIST_COMP_HEIGHT) && (y_pos >= LIST_PATH_TOP_POS))
		{
			return (FILINFO *)0;
		}
		for(cnt=0; cnt<LIST_SIZE; cnt++)
		{
			if((y_pos >= LIST_TOP_POS(cnt)) && (y_pos <= LIST_TOP_POS(cnt)+LIST_COMP_HEIGHT))
			{
				if(FileList[cnt].fattrib != 0)
				{
					return &FileList[cnt];
				}
			}
		}
	}
	if((x_pos >= LIST_NEXT_POS_X) && (x_pos <= LIST_NEXT_POS_X+40))
	{
		if((y_pos <= LIST_NEXT_POS_Y+LIST_COMP_HEIGHT) && (y_pos >= LIST_NEXT_POS_Y))
		{
			return (FILINFO *)1;
		}
	}
	
	return (FILINFO *)-1;
}

void DisplayCurrentDir(void)
{
	FILINFO	info;
	uint32_t 	cnt;
	
	LCD_Clear(Black);
	if (curr_dir.clust != 0) 
	{
		GUI_Text(LIST_LEFT_POS, 	LIST_PATH_TOP_POS, (uint8_t *)dir_path.path, 	Black,Grey);
		GUI_Text(LIST_NEXT_POS_X+10,	LIST_NEXT_POS_Y,	(uint8_t *)"NEXT",			Yellow,Black);
		GUI_Text(VOLUME_POS_X,VOLUME_POS_Y,(uint8_t*)volume_str,Red,Black);
		
		for(cnt = 0; cnt < LIST_SIZE; cnt++)
		{
			while(f_readdir(&curr_dir, &info) == FR_OK)  
			{
				if(!info.fname[0]) 	
				{
					for(; cnt < LIST_SIZE; cnt++)
					{
						FileList[cnt].fattrib = 0;
					}
					f_opendir(&curr_dir, dir_path.path);
					Main_GUI();
					return; 
				}
				if(info.fattrib == AM_DIR)
				{
					memcpy(&FileList[cnt], &info, sizeof(FILINFO));
//					GUI_Text(50,					LIST_TOP_POS(cnt),	(uint8_t *)"Dir",			White,Black);
					GUI_Text(LIST_LEFT_POS+10, 	LIST_TOP_POS(cnt), 	(uint8_t *)info.fname, 	White,Red);
					break;
				}
				else if (info.fattrib == AM_ARC) 
				{ 
					memcpy(&FileList[cnt], &info, sizeof(FILINFO));
//					GUI_Text(50,					LIST_TOP_POS(cnt),	(uint8_t *)"File",			White,Black);
					GUI_Text(LIST_LEFT_POS+10, 	LIST_TOP_POS(cnt), 	(uint8_t *)info.fname, 	White,Blue);
					break;
				}
			}
		}
//		while (f_readdir(&curr_dir, &finfo) == FR_OK)  
//		{
//			if(!finfo.fname[0]) break;   
//			if(finfo.fattrib == AM_DIR)
//			{
//				y_axis++;
//				GUI_Text(50,	y_axis*TEXT_HEIGHT,	"Dir",					White,Red);
//				GUI_Text(100, 	y_axis*TEXT_HEIGHT, 	(uint8_t *)finfo.fname, 	White,Red);
//			}
//			if (finfo.fattrib == AM_ARC) 
//			{ 
//				y_axis++;
//				GUI_Text(50,	y_axis*TEXT_HEIGHT,	"File",					White,Red);
//				GUI_Text(100, 	y_axis*TEXT_HEIGHT, 	(uint8_t *)finfo.fname, 	White,Red);
//			}
//		} 
	}
	Main_GUI();
}
	
void OutPutFile(void)
{ 
	unsigned int a;
	FRESULT res;
	FILINFO finfo;
	DIR dirs;
	char path[16]={""};  

	if (f_opendir(&dirs, path) == FR_OK) 
	{
		a=0;
		while (f_readdir(&dirs, &finfo) == FR_OK)  
		{
			if(!finfo.fname[0]) break;   
			if(finfo.fattrib == AM_DIR)
			{
				a++;
				GUI_Text(50,a*20,"Dir",White,Red);
				GUI_Text(100, a*20, (uint8_t *)finfo.fname, White,Red);
			}
			if (finfo.fattrib == AM_ARC) 
			{ 
				a++;
				GUI_Text(50,a*20,"FILE",White,Red);
				GUI_Text(100, a*20, (uint8_t *)finfo.fname, White,Red);
/*				
				GUI_Text(200,100,"Open",White,Red);
				res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ);
				br=1;
				a=0;
				{					
					for(a=0; a<512; a++) 
						buffer[a]=0; 
					res = f_read(&fsrc, buffer, sizeof(buffer), &br);
					if (res || br == 0) break;   // error or eof
				}
				f_close(&fsrc);            
*/	
			}
		} 
	}
	if (f_opendir(&dirs, "dir1") == FR_OK) 
	{
		a++;
		GUI_Text(50,a*20,"Open [ 0:\\dir1 ]",White,Blue);
		while (f_readdir(&dirs, &finfo) == FR_OK)  
		{
			if(!finfo.fname[0]) break;   
			if(finfo.fattrib == AM_DIR)
			{
				a++;
				GUI_Text(50,a*20,"Dir",White,Red);
				GUI_Text(100, a*20, (uint8_t *)finfo.fname, White,Red);
			}
			if (finfo.fattrib == AM_ARC) 
			{ 
				a++;
				GUI_Text(50,a*20,"File",White,Red);
				GUI_Text(100, a*20, (uint8_t *)finfo.fname, White,Red);
/*				
				res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ);
				br=1;
				a=0;
				{					
					for(a=0; a<512; a++) 
						buffer[a]=0; 
					res = f_read(&fsrc, buffer, sizeof(buffer), &br);
					if (res || br == 0) break;   // error or eof
				}
				f_close(&fsrc);            
*/				
			}
		} 
	}
	if (f_opendir(&dirs, "dir1\\subdir1") == FR_OK) 
	{
		a++;
		GUI_Text(50,a*20,"Open [ 0:\\subdir1 ]",White,Blue);
		while (f_readdir(&dirs, &finfo) == FR_OK)  
		{
			if(!finfo.fname[0]) break;   
			if(finfo.fattrib == AM_DIR)
			{
				a++;
				GUI_Text(50,a*20,"Dir",White,Red);
				GUI_Text(100, a*20, (uint8_t *)finfo.fname, White,Red);
			}
			if (finfo.fattrib == AM_ARC) 
			{ 
				a++;
				GUI_Text(50,a*20,"FILE",White,Red);
				GUI_Text(100, a*20, (uint8_t *)finfo.fname, White,Red);
/*
				res = f_open(&fsrc, finfo.fname, FA_OPEN_EXISTING | FA_READ);
				br=1;
				a=0;
				{					
					for(a=0; a<512; a++) 
						buffer[a]=0; 
					res = f_read(&fsrc, buffer, sizeof(buffer), &br);
					if (res || br == 0) break;   // error or eof
				}
				f_close(&fsrc);            
*/
			}
		} 
	}
  	res = f_unlink("0:/test.txt");
	res = f_open(&fsrc,"0:/test.txt", FA_CREATE_ALWAYS | FA_WRITE);
	if(res==FR_OK)
	{
		res = f_write(&fsrc,textFileBuffer,sizeof(textFileBuffer),(void *)&bw);
	}	
  	f_close(&fsrc);

	res = f_open(&fsrc,"0:/test.txt", FA_OPEN_EXISTING | FA_READ);
	if(res==FR_OK)
	{
		res = f_read(&fsrc, buffer, sizeof(buffer), &br);
	}
  	f_close(&fsrc);

	while(1);
}

void DrawUpArrow(uint16_t x, uint16_t y)
{
	LCD_DrawLine(x-ARROW_HAlF_WIDTH, y+ARROW_HALF_HEIGHT, x+ARROW_HAlF_WIDTH, y+ARROW_HALF_HEIGHT, Red);
	LCD_DrawLine(x-ARROW_HAlF_WIDTH, y+ARROW_HALF_HEIGHT, x, y-ARROW_HALF_HEIGHT, Red);
	LCD_DrawLine(x+ARROW_HAlF_WIDTH, y+ARROW_HALF_HEIGHT, x, y-ARROW_HALF_HEIGHT, Red);
}

void DrawDownArrow(uint16_t x, uint16_t y)
{
	LCD_DrawLine(x-ARROW_HAlF_WIDTH, y-ARROW_HALF_HEIGHT, x+ARROW_HAlF_WIDTH, y-ARROW_HALF_HEIGHT, Red);
	LCD_DrawLine(x-ARROW_HAlF_WIDTH, y-ARROW_HALF_HEIGHT, x, y+ARROW_HALF_HEIGHT, Red);
	LCD_DrawLine(x+ARROW_HAlF_WIDTH, y-ARROW_HALF_HEIGHT, x, y+ARROW_HALF_HEIGHT, Red);
}

int PosToCtrl(uint16_t x_pos, uint16_t y_pos)
{
	uint32_t cnt;
	if((y_pos <= VOLUME_UP_POS_Y+ARROW_HEIGHT) && (y_pos >= VOLUME_UP_POS_Y))
	{
		if((x_pos >= VOLUME_UP_POS_X) && (x_pos <= VOLUME_UP_POS_X+ARROW_WIDTH))
		{
			return 1;
		}
		if((x_pos >= VOLUME_DOWN_POS_X) && (x_pos <= VOLUME_DOWN_POS_X+ARROW_WIDTH))
		{
			return 2;
		}
		if((x_pos >= PLAY_STOP_POS_X) && (x_pos <= PLAY_STOP_POS_X+40))
		{
			return 3;
		}
	}
	
	return -1;
}
