#ifndef __MAIN_H__
#define __MAIN_H__

#include <stm32f4xx_hal.h>
#include "LCD/LCD.h"
#include "LCD/mp3gui.h"
#include "TouchPanel/TouchPanel.h"
#include "FATFS/ff.h"			/* FatFs configurations and declarations */
#include "SDCARD/sdcard.h"
#include "SDCARD/search.h"
//#include "minimp3/minimp3.h"
#include "minimp3/wav_play.h"
#include "minimp3/vs1003.h"
//#include "minimp3/MP3Sample.c"

#define Debug printf
#define printf(x, ...)

#pragma pack(1)
typedef struct {
	uint8_t id[3];
	uint8_t version[2];
	uint8_t flags;
	uint8_t size[4];
} MP3_ID3v2;
#pragma pack(4)

#define ID3v2_SIZE_POS			6
#define ID3v2_FLAGS_POS			5
#define FOOTER_PRESEND_FLAG	0x10

typedef enum {
	TOUCH_NONE,
	TOUCH_ON,
	TOUCH_OFF
} TOUCH_STATE;

typedef enum {
	PLAY_NONE,
	PLAY_ON1,
	PLAY_ON2
} PLAY_STATE;

typedef enum {
	MP3_OFF,
	MP3_ON
} MP3_STATE;

extern int8_t	volume_str[20];

#endif  /* __MAIN_H__ */