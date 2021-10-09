 #include "diskio.h"
 #include "SDCARD/sdcard.h"

/*--------------------------------------------------------------------------
这里为FS文件系统的底层函数
移植FS文件系统总共需要写6个底层函数

---------------------------------------------------------------------------*/

extern SD_HandleTypeDef 		SDIO_HandleStructure;

DSTATUS disk_initialize (
	BYTE drv				/* Physical drive nmuber (0..) */
)
{
 HAL_SD_ErrorTypedef Status;
	
 switch (drv) 
 {
	case 0 :	  
	Status = SD_Init();
	if(Status == SD_OK)
	{
		HAL_SD_HighSpeed(&SDIO_HandleStructure);
		SDIO_HandleStructure.Init.ClockDiv            	= SDIO_TRANSFER_CLK_DIV;
		HAL_SD_WideBusOperation_Config(&SDIO_HandleStructure, SDIO_BUS_WIDE_4B);
		return 0;
	}
	else 
		return STA_NOINIT;
	
	case 1 :	  
		return STA_NOINIT;
		  
	case 2 :
		return STA_NOINIT;
  }
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
	BYTE drv		/* Physical drive nmuber (0..) */
)
{
    switch (drv) 
	{
	  case 0 :		
	  /* translate the reslut code here	*/
	    return 0;
	  case 1 :
	  /* translate the reslut code here	*/
	    return 0;
	  case 2 :
	  /* translate the reslut code here	*/
	    return 0;
	  default:
        break;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	HAL_SD_ErrorTypedef Status;
	if( !count )
	{    
		return RES_PARERR;  /* count不能等于0，否则返回参数错误 */
	}
	switch (drv)
	{
		case 0:
			SD_LowLevel_DMA_RxConfig((uint32_t)buff);
			HAL_SD_ReadBlocks_DMA(&SDIO_HandleStructure, (uint32_t *)buff ,sector<< 9 ,SDCardInfo.CardBlockSize,count);
			/* Check if the Transfer is finished */
			Status = HAL_SD_CheckReadOperation(&SDIO_HandleStructure, 0xFFFFFF);
			//while(HAL_SD_GetStatus(&SDIO_HandleStructure) != SD_TRANSFER_OK);
			if(Status == SD_OK)
			{
				return RES_OK;
			}
			else
			{
				return RES_ERROR;
			}   
			break;
		case 1:	
			break;

		case 2:	
			break;

		default:
			break;
	} 
	return RES_ERROR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	        /* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..255) */
)
{
  HAL_SD_ErrorTypedef Status;
  if( !count )
  {    
    return RES_PARERR;  /* count不能等于0，否则返回参数错误 */
  }
  switch (drv)
  {
    case 0:
		SD_LowLevel_DMA_TxConfig();
	    HAL_SD_WriteBlocks(&SDIO_HandleStructure, (uint32_t *)(&buff[0]) ,sector << 9 ,SDCardInfo.CardBlockSize,count);

    /* Check if the Transfer is finished */
    Status = HAL_SD_CheckWriteOperation(&SDIO_HandleStructure, 0xFFFF);
    while(HAL_SD_GetStatus(&SDIO_HandleStructure) != SD_TRANSFER_OK);	                                               
    if(Status == SD_OK)
    {
       return RES_OK;
    }
    else
    {
       return RES_ERROR;
    }
    case 2:
	   break;
    default :
       break;
  }
 return RES_ERROR;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
    if (drv)
    {    
        return RES_PARERR;  /* 仅支持单磁盘操作，否则返回参数错误 */
    }
	switch (ctrl) 
	{
	  case CTRL_SYNC :	      
		return RES_OK;
	  case GET_SECTOR_COUNT : 
	    *(DWORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
	    return RES_OK;
	  case GET_BLOCK_SIZE :
	    *(WORD*)buff = SDCardInfo.CardBlockSize;
	    return RES_OK;	
	  case CTRL_POWER :
		break;
	  case CTRL_LOCK :
		break;
	  case CTRL_EJECT :
		break;
      /* MMC/SDC command */
	  case MMC_GET_TYPE :
		break;
	  case MMC_GET_CSD :
		break;
	  case MMC_GET_CID :
		break;
	  case MMC_GET_OCR :
		break;
	  case MMC_GET_SDSTAT :
		break;	
	}
	return RES_PARERR;   
}

/* 得到文件Calendar格式的建立日期,是DWORD get_fattime (void) 逆变换 */							
/*-----------------------------------------------------------------------*/
/* User defined function to give a current time to fatfs module          */
/* 31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
/* 15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */                                                                                                                                                                                                                                                
DWORD get_fattime (void)
{
   
    return 0;
}
