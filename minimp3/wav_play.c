#include "wav_play.h"
#include "minimp3/i2s.h"
#include "minimp3/i2c.h"
#include "minimp3/UDA1380.h"

volatile uint32_t XferCplt = 0;/* wait for DMA transfert complete Flag*/
__IO uint32_t WaveDataLength = 0;						// wav 파일 크기

// Private functions ---------------------------------------------------------
uint16_t buffer1[BUFF_MAX_SS] ={0x00};
uint16_t buffer2[BUFF_MAX_SS] ={0x00};
uint32_t BytesRead;         // File R/W count

static uint32_t ReadUnit(uint8_t *buffer, uint8_t idx, uint8_t NbrOfBytes, Endianness BytesFormat);


void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	XferCplt = 1;
	if (WaveDataLength) WaveDataLength -= BUFF_MAX_SS;
	if (WaveDataLength < BUFF_MAX_SS) WaveDataLength = 0;
}

int PlayWav(FIL * file)
{
	static uint32_t wavepos = 0;
	uint32_t cnt;
	uint8_t buffer[512];
	WAVE_FormatTypeDef	WAVE_Format;
	__IO ErrorCode WaveFileStatus = Unvalid_RIFF_ID;
	
	f_read (file, buffer, 512, &cnt);
	
	if(cnt == 512)
	{
		WaveFileStatus = WavePlayer_WaveParsing(&WAVE_Format, buffer, &wavepos);
		if (WaveFileStatus == Valid_WAVE_File) 
		{
			f_lseek(file, wavepos);
			Audio_Play(file, &WAVE_Format);
			return 0;
		}
	}
	return -1;
}

void Audio_Play(FIL * fileR, WAVE_FormatTypeDef * WAVE_Format)
{
	uint8_t buffer_switch = 1;
	XferCplt = 0;
	
	WaveDataLength = WAVE_Format->DataSize;
	I2S_Configuration(WAVE_Format->SampleRate);

  	f_read (fileR, buffer1, BUFF_MAX_SS, &BytesRead); 
  	f_read (fileR, buffer2, BUFF_MAX_SS, &BytesRead);
 	
  	// Start playing wave      
	Audio_MAL_Play(buffer1, BUFF_MAX_SS);
	UDA1380_Configuration();
	
	while(WaveDataLength != 0)
  	{
		while(XferCplt == 0)
		{
			// wait for DMA transfert complete   
			// You can write some code for operate on UDA1380   
		}
		XferCplt = 0;

		if(buffer_switch == 0)
		{
			// Play data from buffer1   	
			Audio_MAL_Play(buffer1, BUFF_MAX_SS);
			// Store data in buffer2   
			f_read (fileR, buffer2, BUFF_MAX_SS, &BytesRead);
			buffer_switch = 1;
		}
		else 
		{   
			// Play data from buffer2   
			Audio_MAL_Play(buffer2, BUFF_MAX_SS);
			// Store data in buffer1   
			f_read (fileR, buffer1, BUFF_MAX_SS, &BytesRead);
			buffer_switch = 0;
		}
	} 
}

ErrorCode WavePlayer_WaveParsing(WAVE_FormatTypeDef * WAVE_Format, uint8_t * buffer, uint32_t *FileLen)
{
  uint32_t temp = 0x00;
  uint32_t extraformatbytes = 0;
  __IO uint32_t SpeechDataOffset = 0x00;
  
  // Read chunkID, must be 'RIFF'
  temp = ReadUnit((uint8_t*)buffer, 0, 4, BigEndian);
  if (temp != CHUNK_ID)
  {
    return(Unvalid_RIFF_ID);
  }

  // Read the file length 
  WAVE_Format->RIFFchunksize = ReadUnit((uint8_t*)buffer, 4, 4, LittleEndian);

  // Read the file format, must be 'WAVE' 
  temp = ReadUnit((uint8_t*)buffer, 8, 4, BigEndian);
  if (temp != FILE_FORMAT)
  {
    return(Unvalid_WAVE_Format);
  }

  // Read the format chunk, must be'fmt ' 
  temp = ReadUnit((uint8_t*)buffer, 12, 4, BigEndian);
  if (temp != FORMAT_ID)
  {
    return(Unvalid_FormatChunk_ID);
  }

  // Read the length of the 'fmt' data, must be 0x10 -------------------------
  temp = ReadUnit((uint8_t*)buffer, 16, 4, LittleEndian);
  if (temp != 0x10)
  {
    extraformatbytes = 1;
  }

  // Read the audio format, must be 0x01 (PCM) 
  WAVE_Format->FormatTag = ReadUnit((uint8_t*)buffer, 20, 2, LittleEndian);
  if (WAVE_Format->FormatTag != WAVE_FORMAT_PCM)
  {
    return(Unsupporetd_FormatTag);
  }
  
  // Read the number of channels, must be 0x01 (Mono) or 0x02 (Stereo) 
  WAVE_Format->NumChannels = ReadUnit((uint8_t*)buffer, 22, 2, LittleEndian);

  // Read the Sample Rate 
  WAVE_Format->SampleRate = ReadUnit((uint8_t*)buffer, 24, 4, LittleEndian);

  // Read the Byte Rate 
  WAVE_Format->ByteRate = ReadUnit((uint8_t*)buffer, 28, 4, LittleEndian);

  // Read the block alignment 
  WAVE_Format->BlockAlign = ReadUnit((uint8_t*)buffer, 32, 2, LittleEndian);
  
  // Read the number of bits per sample 
  WAVE_Format->BitsPerSample = ReadUnit((uint8_t*)buffer, 34, 2, LittleEndian);
  if (WAVE_Format->BitsPerSample != BITS_PER_SAMPLE_16) 
  {
    return(Unsupporetd_Bits_Per_Sample);
  }

  SpeechDataOffset = 36;
  // If there is Extra format bytes, these bytes will be defined in "Fact Chunk" 
  if (extraformatbytes == 1)
  {
    // Read th Extra format bytes, must be 0x00 
    temp = ReadUnit((uint8_t*)buffer, 36, 2, LittleEndian);
    if (temp != 0x00)
    {
      return(Unsupporetd_ExtraFormatBytes);
    }
    // Read the Fact chunk, must be 'fact' 
    temp = ReadUnit((uint8_t*)buffer, 38, 4, BigEndian);
    if (temp != FACT_ID)
    {
      return(Unvalid_FactChunk_ID);
    }
    // Read Fact chunk data Size/
    temp = ReadUnit((uint8_t*)buffer, 42, 4, LittleEndian);
    
    SpeechDataOffset += 10 + temp;
  }
  // Read the Data chunk, must be 'data' 
  temp = ReadUnit((uint8_t*)buffer, SpeechDataOffset, 4, BigEndian);
  SpeechDataOffset += 4;
  if (temp != DATA_ID)
  {
    return(Unvalid_DataChunk_ID);
  }
  // Read the number of sample data 
  WAVE_Format->DataSize = ReadUnit((uint8_t*)buffer, SpeechDataOffset, 4, LittleEndian);
  SpeechDataOffset += 4;
  *FileLen =  SpeechDataOffset;
  
  return(Valid_WAVE_File);
}

static uint32_t ReadUnit(uint8_t *buffer, uint8_t idx, uint8_t NbrOfBytes, Endianness BytesFormat)
{
  uint32_t index = 0;
  uint32_t temp = 0;

  for (index = 0; index < NbrOfBytes; index++)
  {
    temp |= buffer[idx + index] << (index * 8);
  }
  
  if (BytesFormat == BigEndian)
  {
    temp = __REV(temp);
  }
  return temp;
}
