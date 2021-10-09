#ifndef 		__SEARCH_H
#define 	__SEARCH_H

#include <stm32f4xx_hal.h>
#include "LCD/LCD.h"
#include "LCD/mp3gui.h"
#include "TouchPanel/TouchPanel.h"
#include "FATFS/ff.h"			/* FatFs configurations and declarations */
#include "SDCARD/sdcard.h"

#define Debug printf
#define TEXT_HEIGHT		16
#define TEXT_GAP			0

#define LIST_COMP_WIDTH	150
#define LIST_COMP_HEIGHT	16

#define LIST_PATH_TOP_POS	30

#define LIST_LEFT_POS			40
#define LIST_TOP_POS(num)		50 + (LIST_COMP_HEIGHT+TEXT_GAP)*(num)

#define LIST_NEXT_POS_X		210
#define LIST_NEXT_POS_Y		167

#define LIST_SIZE			7
#define LIST_SIZE_MAX	7

#define PLAY_STOP_POS_X		200
#define PLAY_STOP_POS_Y		200
#define VOLUME_UP_POS_X		260
#define VOLUME_UP_POS_Y		200
#define VOLUME_DOWN_POS_X	230
#define VOLUME_DOWN_POS_Y	200

#define VOLUME_POS_X	40
#define VOLUME_POS_Y	180

#define PROGRESS_POS_X	40
#define PROGRESS_POS_Y	200

#define ARROW_WIDTH			(20)
#define ARROW_HEIGHT			(20)
#define ARROW_HAlF_WIDTH		(ARROW_WIDTH/2)
#define ARROW_HALF_HEIGHT	(ARROW_HEIGHT/2)

void FileSystem_Init(void);
FRESULT ToUpperDir(void);
FRESULT ToDownDir(TCHAR * dir_name);
void DisplayCurrentDir(void);
uint32_t CheckExtension(TCHAR * FileName, const TCHAR * Ext);

FILINFO * PosToFInfo(uint16_t x_pos, uint16_t y_pos);
TCHAR * GetFilePath(TCHAR * name);

void OutPutFile(void);

void DrawUpArrow(uint16_t x, uint16_t y);
void DrawDownArrow(uint16_t x, uint16_t y);
int PosToCtrl(uint16_t x_pos, uint16_t y_pos);

#endif	/* __SEARCH_H */
