#ifndef __SDCARD_H
#define __SDCARD_H
#include <stm32f4xx_hal.h>


/** 
  * @brief  Card Specific Data: CSD Register   
  */ 
typedef struct
{
  __IO uint8_t  CSDStruct;            /*!< CSD structure */
  __IO uint8_t  SysSpecVersion;       /*!< System specification version */
  __IO uint8_t  Reserved1;            /*!< Reserved */
  __IO uint8_t  TAAC;                 /*!< Data read access-time 1 */
  __IO uint8_t  NSAC;                 /*!< Data read access-time 2 in CLK cycles */
  __IO uint8_t  MaxBusClkFrec;        /*!< Max. bus clock frequency */
  __IO uint16_t CardComdClasses;      /*!< Card command classes */
  __IO uint8_t  RdBlockLen;           /*!< Max. read data block length */
  __IO uint8_t  PartBlockRead;        /*!< Partial blocks for read allowed */
  __IO uint8_t  WrBlockMisalign;      /*!< Write block misalignment */
  __IO uint8_t  RdBlockMisalign;      /*!< Read block misalignment */
  __IO uint8_t  DSRImpl;              /*!< DSR implemented */
  __IO uint8_t  Reserved2;            /*!< Reserved */
  __IO uint32_t DeviceSize;           /*!< Device Size */
  __IO uint8_t  MaxRdCurrentVDDMin;   /*!< Max. read current @ VDD min */
  __IO uint8_t  MaxRdCurrentVDDMax;   /*!< Max. read current @ VDD max */
  __IO uint8_t  MaxWrCurrentVDDMin;   /*!< Max. write current @ VDD min */
  __IO uint8_t  MaxWrCurrentVDDMax;   /*!< Max. write current @ VDD max */
  __IO uint8_t  DeviceSizeMul;        /*!< Device size multiplier */
  __IO uint8_t  EraseGrSize;          /*!< Erase group size */
  __IO uint8_t  EraseGrMul;           /*!< Erase group size multiplier */
  __IO uint8_t  WrProtectGrSize;      /*!< Write protect group size */
  __IO uint8_t  WrProtectGrEnable;    /*!< Write protect group enable */
  __IO uint8_t  ManDeflECC;           /*!< Manufacturer default ECC */
  __IO uint8_t  WrSpeedFact;          /*!< Write speed factor */
  __IO uint8_t  MaxWrBlockLen;        /*!< Max. write data block length */
  __IO uint8_t  WriteBlockPaPartial;  /*!< Partial blocks for write allowed */
  __IO uint8_t  Reserved3;            /*!< Reserded */
  __IO uint8_t  ContentProtectAppli;  /*!< Content protection application */
  __IO uint8_t  FileFormatGrouop;     /*!< File format group */
  __IO uint8_t  CopyFlag;             /*!< Copy flag (OTP) */
  __IO uint8_t  PermWrProtect;        /*!< Permanent write protection */
  __IO uint8_t  TempWrProtect;        /*!< Temporary write protection */
  __IO uint8_t  FileFormat;           /*!< File Format */
  __IO uint8_t  ECC;                  /*!< ECC code */
  __IO uint8_t  CSD_CRC;              /*!< CSD CRC */
  __IO uint8_t  Reserved4;            /*!< always 1*/
} SD_CSD;

/** 
  * @brief  Card Identification Data: CID Register   
  */
typedef struct
{
  __IO uint8_t  ManufacturerID;       /*!< ManufacturerID */
  __IO uint16_t OEM_AppliID;          /*!< OEM/Application ID */
  __IO uint32_t ProdName1;            /*!< Product Name part1 */
  __IO uint8_t  ProdName2;            /*!< Product Name part2*/
  __IO uint8_t  ProdRev;              /*!< Product Revision */
  __IO uint32_t ProdSN;               /*!< Product Serial Number */
  __IO uint8_t  Reserved1;            /*!< Reserved1 */
  __IO uint16_t ManufactDate;         /*!< Manufacturing Date */
  __IO uint8_t  CID_CRC;              /*!< CID CRC */
  __IO uint8_t  Reserved2;            /*!< always 1 */
} SD_CID;

/** 
  * @brief SD Card Status 
  */
typedef struct
{
  __IO uint8_t DAT_BUS_WIDTH;
  __IO uint8_t SECURED_MODE;
  __IO uint16_t SD_CARD_TYPE;
  __IO uint32_t SIZE_OF_PROTECTED_AREA;
  __IO uint8_t SPEED_CLASS;
  __IO uint8_t PERFORMANCE_MOVE;
  __IO uint8_t AU_SIZE;
  __IO uint16_t ERASE_SIZE;
  __IO uint8_t ERASE_TIMEOUT;
  __IO uint8_t ERASE_OFFSET;
} SD_CardStatus;


   
#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)

//#define SD_SDIO_DMA_STREAM3	          3
#define SD_SDIO_DMA_STREAM6           6

#define SD_SDIO_DMA                   	DMA2
#define SD_SDIO_DMA_CLK               	RCC_AHB1Periph_DMA2
 
//#define SD_SDIO_DMA_STREAM3	          3
#define SD_SDIO_DMA_STREAM6           6

#ifdef SD_SDIO_DMA_STREAM3
#define SD_SDIO_DMA_BASE_ADDRESS	DMA2_Stream3_BASE
 #define SD_SDIO_DMA_STREAM           	DMA2_Stream3
 #define SD_SDIO_DMA_CHANNEL			DMA_CHANNEL_4
 #define SD_SDIO_DMA_FLAG_FEIF         	DMA_FLAG_FEIF3_7
 #define SD_SDIO_DMA_FLAG_DMEIF    	DMA_FLAG_DMEIF3_7
 #define SD_SDIO_DMA_FLAG_TEIF         	DMA_FLAG_TEIF3_7
 #define SD_SDIO_DMA_FLAG_HTIF         	DMA_FLAG_HTIF3_7
 #define SD_SDIO_DMA_FLAG_TCIF         	DMA_FLAG_TCIF3_7
 #define SD_SDIO_DMA_IRQn              	DMA2_Stream3_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream3_IRQHandler 
#elif defined SD_SDIO_DMA_STREAM6
 #define SD_SDIO_DMA_STREAM			DMA2_Stream6
 #define SD_SDIO_DMA_CHANNEL			DMA_CHANNEL_4
 #define SD_SDIO_DMA_FLAG_FEIF         	DMA_FLAG_FEIF2_6
 #define SD_SDIO_DMA_FLAG_DMEIF    	DMA_FLAG_DMEIF2_6
 #define SD_SDIO_DMA_FLAG_TEIF         	DMA_FLAG_TEIF2_6
 #define SD_SDIO_DMA_FLAG_HTIF         	DMA_FLAG_HTIF2_6
 #define SD_SDIO_DMA_FLAG_TCIF         	DMA_FLAG_TCIF2_6
 #define SD_SDIO_DMA_IRQn              	DMA2_Stream6_IRQn
 #define SD_SDIO_DMA_IRQHANDLER        DMA2_Stream6_IRQHandler
#endif /* SD_SDIO_DMA_STREAM3 */

/** 
  * @brief Supported SD Memory Cards 
  */
#define SDIO_STD_CAPACITY_SD_CARD_V1_1             ((uint32_t)0x00000000)
#define SDIO_STD_CAPACITY_SD_CARD_V2_0             ((uint32_t)0x00000001)
#define SDIO_HIGH_CAPACITY_SD_CARD                 ((uint32_t)0x00000002)
#define SDIO_MULTIMEDIA_CARD                       ((uint32_t)0x00000003)
#define SDIO_SECURE_DIGITAL_IO_CARD                ((uint32_t)0x00000004)
#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD            ((uint32_t)0x00000005)
#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD          ((uint32_t)0x00000006)
#define SDIO_HIGH_CAPACITY_MMC_CARD                ((uint32_t)0x00000007)

/**
  * @brief  SD detection on its memory slot
  */
#define SD_PRESENT                                 ((uint8_t)0x01)
#define SD_NOT_PRESENT                             ((uint8_t)0x00)

extern SD_HandleTypeDef 		SDIO_HandleStructure;
extern DMA_HandleTypeDef 		Rx_DmaHandle;
extern DMA_HandleTypeDef 		Tx_DmaHandle;
extern HAL_SD_CardInfoTypedef	SDCardInfo;

HAL_StatusTypeDef SD_LowLevel_DMA_RxConfig(uint32_t dst_addr);
void SD_LowLevel_DMA_TxConfig(void);
void SD_DeInit(void);
HAL_SD_ErrorTypedef SD_Init(void);
#endif
