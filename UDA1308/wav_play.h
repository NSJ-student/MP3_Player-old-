#ifndef __WAV_PLAY_H__
#define __WAV_PLAY_H__

#include <stm32f4xx_hal.h>
#include "ff.h"
#include "i2s.h"

typedef enum
{
  LittleEndian,
  BigEndian
}Endianness;

typedef struct
{
  uint32_t  RIFFchunksize;
  uint16_t  FormatTag;
  uint16_t  NumChannels;
  uint32_t  SampleRate;
  uint32_t  ByteRate;
  uint16_t  BlockAlign;
  uint16_t  BitsPerSample;
  uint32_t  DataSize;
}
WAVE_FormatTypeDef;

typedef enum
{
  Valid_WAVE_File = 0,
  Unvalid_RIFF_ID,
  Unvalid_WAVE_Format,
  Unvalid_FormatChunk_ID,
  Unsupporetd_FormatTag,
  Unsupporetd_Number_Of_Channel,
  Unsupporetd_Sample_Rate,
  Unsupporetd_Bits_Per_Sample,
  Unvalid_DataChunk_ID,
  Unsupporetd_ExtraFormatBytes,
  Unvalid_FactChunk_ID
} ErrorCode;

// Exported constants --------------------------------------------------------
#define  CHUNK_ID                            0x52494646  // correspond to the letters 'RIFF'/
#define  FILE_FORMAT                         0x57415645  // correspond to the letters 'WAVE' 
#define  FORMAT_ID                           0x666D7420  // correspond to the letters 'fmt ' 
#define  DATA_ID                             0x64617461  // correspond to the letters 'data'
#define  FACT_ID                             0x66616374  // correspond to the letters 'fact' 
#define  WAVE_FORMAT_PCM                     0x01
#define  FORMAT_CHNUK_SIZE                   0x10
#define  CHANNEL_MONO                        0x01
#define  CHANNEL_STEREO                      0x02
#define  SAMPLE_RATE_8000                    8000
#define  SAMPLE_RATE_11025                   11025
#define  SAMPLE_RATE_22050                   22050
#define  SAMPLE_RATE_44100                   44100
#define  BITS_PER_SAMPLE_8                   8
#define  BITS_PER_SAMPLE_16                  16

#define	BUFF_MAX_SS		4096	// 512, 1024, 2048 or 4096 


ErrorCode WavePlayer_WaveParsing(uint8_t * buffer, uint32_t *FileLen);
void Audio_Play(FIL * fileR, WAVE_FormatTypeDef * WAVE_Format);

#endif