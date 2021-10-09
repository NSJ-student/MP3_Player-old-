#ifndef		__MP3GUI_H__
#define 	__MP3GUI_H__

#include <stm32f4xx_hal.h>
#include "LCD/LCD.h"
#include "FATFS/ff.h"			/* FatFs configurations and declarations */

#define BMP_MAX_WIDTH		30
#define BMP_MAX_HEIGHT	30

#define TEXT_W	8
#define TEXT_H	16

#define DRAW_BITMAP 0
#define TEXT_OFFSET	3
#define PATH_PLAY	"0:\\Image\\play.bin"
#define PATH_VOLD	"0:\\Image\\volD.bin"
#define PATH_VOLU	"0:\\Image\\volU.bin"

typedef struct _rect {
	int x;
	int y;
	int width;
	int height;
} RECT;

void Main_GUI(void);


#endif		/* __MP3GUI_H__ */
