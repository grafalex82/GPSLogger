#include "SdMscDriver.h"
#include "SdCard/SdSpiCard.h"

#include "SdFatSPIDriver.h"

extern SdFatSPIDriver spiDriver;
SdSpiCard card;

bool initSD()
{
	bool res = card.begin(&spiDriver, PA4, SPI_FULL_SPEED);
	return res;
}

const uint8_t SD_MSC_Inquirydata[] = {/* 36 */
  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,
  0x00,
  'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
  'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0' ,'1'                     /* Version      : 4 Bytes */
};

int8_t SD_MSC_Init (uint8_t lun)
{
	(void)lun; // Not used

//	if(!card.begin(&spiDriver, 0, SPI_FULL_SPEED))
//		return USBD_FAIL;

	return (USBD_OK);
}

int8_t SD_MSC_GetCapacity (uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
	(void)lun; // Not used

	*block_num  = card.cardSize();
	*block_size = 512;
	return (USBD_OK);
}

int8_t  SD_MSC_IsReady (uint8_t lun)
{
	(void)lun; // Not used

	return (USBD_OK);
}

int8_t  SD_MSC_IsWriteProtected (uint8_t lun)
{
	(void)lun; // Not used

	return (USBD_OK); // Never write protected
}

int8_t SD_MSC_Read (uint8_t lun,
						uint8_t *buf,
						uint32_t blk_addr,
						uint16_t blk_len)
{
	(void)lun; // Not used

	if(!card.readBlocks(blk_addr, buf, blk_len))
		return USBD_FAIL;

	return (USBD_OK);
}

int8_t SD_MSC_Write (uint8_t lun,
						 uint8_t *buf,
						 uint32_t blk_addr,
						 uint16_t blk_len)
{
	(void)lun; // Not used

	if(!card.writeBlocks(blk_addr, buf, blk_len))
		return USBD_FAIL;

	return (USBD_OK);
}

int8_t SD_MSC_GetMaxLun (void)
{
  return 0; // We have just 1 Logic unit number (LUN) which is zero
}

USBD_StorageTypeDef SdMscDriver =
{
	SD_MSC_Init,
	SD_MSC_GetCapacity,
	SD_MSC_IsReady,
	SD_MSC_IsWriteProtected,
	SD_MSC_Read,
	SD_MSC_Write,
	SD_MSC_GetMaxLun,
	(int8_t *)SD_MSC_Inquirydata,
};
