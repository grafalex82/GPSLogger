/**
  ******************************************************************************
  * @file    usbd_msc_scsi.c
  * @author  MCD Application Team
  * @version V2.4.2
  * @date    11-December-2015
  * @brief   This file provides all the USBD SCSI layer functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_bot.h"
#include "usbd_msc_scsi.h"
#include "usbd_msc.h"
#include "usbd_msc_data.h"

#include "stm32f1xx_ll_gpio.h"

extern void serialDebugWrite(const char * fmt, ...);
extern void serialDebugWriteC(char c);

/** @addtogroup STM32_USB_DEVICE_LIBRARY
  * @{
  */


/** @defgroup MSC_SCSI 
  * @brief Mass storage SCSI layer module
  * @{
  */ 

/** @defgroup MSC_SCSI_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup MSC_SCSI_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup MSC_SCSI_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup MSC_SCSI_Private_Variables
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup MSC_SCSI_Private_FunctionPrototypes
  * @{
  */ 
static int8_t SCSI_TestUnitReady(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_Inquiry(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_ReadFormatCapacity(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_ReadCapacity10(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_RequestSense (USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_StartStopUnit(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_ModeSense6 (USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_ModeSense10 (USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_Write10(USBD_HandleTypeDef  *pdev, uint8_t lun , uint8_t *params);
static int8_t SCSI_Read10(USBD_HandleTypeDef  *pdev, uint8_t lun , uint8_t *params);
static int8_t SCSI_Verify10(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params);
static int8_t SCSI_CheckAddressRange (USBD_HandleTypeDef  *pdev, 
                                      uint8_t lun , 
                                      uint32_t blk_offset , 
                                      uint16_t blk_nbr);
static int8_t SCSI_ProcessRead (USBD_HandleTypeDef  *pdev,
                                uint8_t lun);

static int8_t SCSI_ProcessWrite (USBD_HandleTypeDef  *pdev,
								 uint8_t lun);
/**
  * @}
  */ 


/** @defgroup MSC_SCSI_Private_Functions
  * @{
  */ 


/**
* @brief  SCSI_ProcessCmd
*         Process SCSI commands
* @param  pdev: device instance
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
int8_t SCSI_ProcessCmd(USBD_HandleTypeDef  *pdev,
                           uint8_t lun, 
                           uint8_t *params)
{
  switch (params[0])
  {
  case SCSI_TEST_UNIT_READY:
    return SCSI_TestUnitReady(pdev, lun, params);
    
  case SCSI_REQUEST_SENSE:
    return SCSI_RequestSense (pdev, lun, params);
  case SCSI_INQUIRY:
    return SCSI_Inquiry(pdev, lun, params);
    
  case SCSI_START_STOP_UNIT:
    return SCSI_StartStopUnit(pdev, lun, params);
    
  case SCSI_ALLOW_MEDIUM_REMOVAL:
    return SCSI_StartStopUnit(pdev, lun, params);
    
  case SCSI_MODE_SENSE6:
    return SCSI_ModeSense6 (pdev, lun, params);
    
  case SCSI_MODE_SENSE10:
    return SCSI_ModeSense10 (pdev, lun, params);
    
  case SCSI_READ_FORMAT_CAPACITIES:
    return SCSI_ReadFormatCapacity(pdev, lun, params);
    
  case SCSI_READ_CAPACITY10:
    return SCSI_ReadCapacity10(pdev, lun, params);
    
  case SCSI_READ10:
    return SCSI_Read10(pdev, lun, params); 
    
  case SCSI_WRITE10:
    return SCSI_Write10(pdev, lun, params);
    
  case SCSI_VERIFY10:
    return SCSI_Verify10(pdev, lun, params);
    
  default:
    SCSI_SenseCode(pdev, 
                   lun,
                   ILLEGAL_REQUEST, 
                   INVALID_CDB);    
    return -1;
  }
}


/**
* @brief  SCSI_TestUnitReady
*         Process SCSI Test Unit Ready Command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t SCSI_TestUnitReady(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC;  
    
  /* case 9 : Hi > D0 */
  if (hmsc->cbw.dDataLength != 0)
  {
    SCSI_SenseCode(pdev,
                   hmsc->cbw.bLUN, 
                   ILLEGAL_REQUEST, 
                   INVALID_CDB);
    return -1;
  }  
  
  if(pdev->pClassSpecificInterfaceMSC->IsReady(lun) !=0 )
  {
    SCSI_SenseCode(pdev,
                   lun,
                   NOT_READY, 
                   MEDIUM_NOT_PRESENT);
    
    hmsc->bot_state = USBD_BOT_NO_DATA;
    return -1;
  } 
  hmsc->bot_data_length = 0;
  return 0;
}

/**
* @brief  SCSI_Inquiry
*         Process Inquiry command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t  SCSI_Inquiry(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  uint8_t* pPage;
  uint16_t len;

  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  if (params[1] & 0x01)/*Evpd is set*/
  {
    pPage = (uint8_t *)MSC_Page00_Inquiry_Data;
    len = LENGTH_INQUIRY_PAGE00;
  }
  else
  {
    
    pPage = (uint8_t *)&pdev->pClassSpecificInterfaceMSC->pInquiry[lun * STANDARD_INQUIRY_DATA_LEN];
    len = pPage[4] + 5;
    
    if (params[4] <= len)
    {
      len = params[4];
    }
  }
  hmsc->bot_data_length = len;
  
  while (len) 
  {
    len--;
    hmsc->bot_data[len] = pPage[len];
  }
  return 0;
}

/**
* @brief  SCSI_ReadCapacity10
*         Process Read Capacity 10 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t SCSI_ReadCapacity10(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  if(pdev->pClassSpecificInterfaceMSC->GetCapacity(lun, &hmsc->scsi_blk_nbr, &hmsc->scsi_blk_size) != 0)
  {
    SCSI_SenseCode(pdev,
                   lun,
                   NOT_READY, 
                   MEDIUM_NOT_PRESENT);
    return -1;
  } 
  else
  {
    
    hmsc->bot_data[0] = (uint8_t)((hmsc->scsi_blk_nbr - 1) >> 24);
    hmsc->bot_data[1] = (uint8_t)((hmsc->scsi_blk_nbr - 1) >> 16);
    hmsc->bot_data[2] = (uint8_t)((hmsc->scsi_blk_nbr - 1) >>  8);
    hmsc->bot_data[3] = (uint8_t)(hmsc->scsi_blk_nbr - 1);
    
    hmsc->bot_data[4] = (uint8_t)(hmsc->scsi_blk_size >>  24);
    hmsc->bot_data[5] = (uint8_t)(hmsc->scsi_blk_size >>  16);
    hmsc->bot_data[6] = (uint8_t)(hmsc->scsi_blk_size >>  8);
    hmsc->bot_data[7] = (uint8_t)(hmsc->scsi_blk_size);
    
    hmsc->bot_data_length = 8;
    return 0;
  }
}
/**
* @brief  SCSI_ReadFormatCapacity
*         Process Read Format Capacity command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t SCSI_ReadFormatCapacity(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  uint16_t blk_size;
  uint32_t blk_nbr;
  uint16_t i;
  
  for(i=0 ; i < 12 ; i++) 
  {
    hmsc->bot_data[i] = 0;
  }
  
  if(pdev->pClassSpecificInterfaceMSC->GetCapacity(lun, &blk_nbr, &blk_size) != 0)
  {
    SCSI_SenseCode(pdev,
                   lun,
                   NOT_READY, 
                   MEDIUM_NOT_PRESENT);
    return -1;
  } 
  else
  {
    hmsc->bot_data[3] = 0x08;
    hmsc->bot_data[4] = (uint8_t)((blk_nbr - 1) >> 24);
    hmsc->bot_data[5] = (uint8_t)((blk_nbr - 1) >> 16);
    hmsc->bot_data[6] = (uint8_t)((blk_nbr - 1) >>  8);
    hmsc->bot_data[7] = (uint8_t)(blk_nbr - 1);
    
    hmsc->bot_data[8] = 0x02;
    hmsc->bot_data[9] = (uint8_t)(blk_size >>  16);
    hmsc->bot_data[10] = (uint8_t)(blk_size >>  8);
    hmsc->bot_data[11] = (uint8_t)(blk_size);
    
    hmsc->bot_data_length = 12;
    return 0;
  }
}
/**
* @brief  SCSI_ModeSense6
*         Process Mode Sense6 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t SCSI_ModeSense6 (USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  uint16_t len = 8 ;
  hmsc->bot_data_length = len;
  
  while (len) 
  {
    len--;
    hmsc->bot_data[len] = MSC_Mode_Sense6_data[len];
  }
  return 0;
}

/**
* @brief  SCSI_ModeSense10
*         Process Mode Sense10 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t SCSI_ModeSense10 (USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  uint16_t len = 8;
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  hmsc->bot_data_length = len;

  while (len) 
  {
    len--;
    hmsc->bot_data[len] = MSC_Mode_Sense10_data[len];
  }
  return 0;
}

/**
* @brief  SCSI_RequestSense
*         Process Request Sense command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/

static int8_t SCSI_RequestSense (USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  uint8_t i;
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  for(i=0 ; i < REQUEST_SENSE_DATA_LEN ; i++) 
  {
    hmsc->bot_data[i] = 0;
  }
  
  hmsc->bot_data[0]	= 0x70;		
  hmsc->bot_data[7]	= REQUEST_SENSE_DATA_LEN - 6;	
  
  if((hmsc->scsi_sense_head != hmsc->scsi_sense_tail)) {
    
    hmsc->bot_data[2]     = hmsc->scsi_sense[hmsc->scsi_sense_head].Skey;		
    hmsc->bot_data[12]    = hmsc->scsi_sense[hmsc->scsi_sense_head].w.b.ASCQ;	
    hmsc->bot_data[13]    = hmsc->scsi_sense[hmsc->scsi_sense_head].w.b.ASC;	
    hmsc->scsi_sense_head++;
    
    if (hmsc->scsi_sense_head == SENSE_LIST_DEEPTH)
    {
      hmsc->scsi_sense_head = 0;
    }
  }
  hmsc->bot_data_length = REQUEST_SENSE_DATA_LEN;  
  
  if (params[4] <= REQUEST_SENSE_DATA_LEN)
  {
    hmsc->bot_data_length = params[4];
  }
  return 0;
}

/**
* @brief  SCSI_SenseCode
*         Load the last error code in the error list
* @param  lun: Logical unit number
* @param  sKey: Sense Key
* @param  ASC: Additional Sense Key
* @retval none

*/
void SCSI_SenseCode(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t sKey, uint8_t ASC)
{
  //serialDebugWriteC('S');


  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  hmsc->scsi_sense[hmsc->scsi_sense_tail].Skey  = sKey;
  hmsc->scsi_sense[hmsc->scsi_sense_tail].w.ASC = ASC << 8;
  hmsc->scsi_sense_tail++;
  if (hmsc->scsi_sense_tail == SENSE_LIST_DEEPTH)
  {
    hmsc->scsi_sense_tail = 0;
  }
}
/**
* @brief  SCSI_StartStopUnit
*         Process Start Stop Unit command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t SCSI_StartStopUnit(USBD_HandleTypeDef  *pdev, uint8_t lun, uint8_t *params)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC;   
  hmsc->bot_data_length = 0;
  return 0;
}

/**
* @brief  SCSI_Read10
*         Process Read10 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/
static int8_t SCSI_Read10(USBD_HandleTypeDef  *pdev, uint8_t lun , uint8_t *params)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 

  // Synchronization to avoid several transmits at a time
  pdev->pClassSpecificInterfaceMSC->OnStartOp();

  //serialDebugWrite("Read command in state %d\n", hmsc->bot_state);

  if(hmsc->bot_state == USBD_BOT_IDLE)  /* Idle */
  {
	//serialDebugWriteC('\n');
    /* case 10 : Ho <> Di */
    
    if ((hmsc->cbw.bmFlags & 0x80) != 0x80)
    {
      SCSI_SenseCode(pdev,
                     hmsc->cbw.bLUN, 
                     ILLEGAL_REQUEST, 
                     INVALID_CDB);
      return -1;
    }    
    
    if(pdev->pClassSpecificInterfaceMSC->IsReady(lun) !=0 )
    {
      SCSI_SenseCode(pdev,
                     lun,
                     NOT_READY, 
                     MEDIUM_NOT_PRESENT);
      return -1;
    } 
    
    hmsc->scsi_blk_addr = (params[2] << 24) | \
      (params[3] << 16) | \
        (params[4] <<  8) | \
          params[5];
    
    hmsc->scsi_blk_len =  (params[7] <<  8) | \
      params[8];  
    
    
    
    if( SCSI_CheckAddressRange(pdev, lun, hmsc->scsi_blk_addr, hmsc->scsi_blk_len) < 0)
    {
      return -1; /* error */
    }

	//serialDebugWrite("Starting new read operation LBA=%08x, len=%d\n", hmsc->scsi_blk_addr, hmsc->scsi_blk_len);

    hmsc->bot_state = USBD_BOT_DATA_IN;
    hmsc->scsi_blk_addr *= hmsc->scsi_blk_size;
    hmsc->scsi_blk_len  *= hmsc->scsi_blk_size;
    
    /* cases 4,5 : Hi <> Dn */
    if (hmsc->cbw.dDataLength != hmsc->scsi_blk_len)
    {
      SCSI_SenseCode(pdev,
                     hmsc->cbw.bLUN, 
                     ILLEGAL_REQUEST, 
                     INVALID_CDB);
	  serialDebugWrite("Failed at point 1: cbw.dDataLength=%d != scsi_blk_len=%d\n", hmsc->cbw.dDataLength, hmsc->scsi_blk_len);
      return -1;
    }

	hmsc->bot_data_idx = 0;
	hmsc->bot_data_length = MSC_MEDIA_PACKET;

	return SCSI_ProcessRead(pdev, lun);
  }
  //else
	  //serialDebugWriteC('-');

  return 0;
}

/**
* @brief  SCSI_Write10
*         Process Write10 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/

static int8_t SCSI_Write10 (USBD_HandleTypeDef  *pdev, uint8_t lun , uint8_t *params)
{
	USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC;

	if (hmsc->bot_state == USBD_BOT_IDLE) /* Idle */
	{
		//serialDebugWriteC('\n');
		/* case 8 : Hi <> Do */

		if ((hmsc->cbw.bmFlags & 0x80) == 0x80)
		{
			SCSI_SenseCode(pdev,
						   hmsc->cbw.bLUN,
						   ILLEGAL_REQUEST,
						   INVALID_CDB);
			return -1;
		}

		/* Check whether Media is ready */
		if(pdev->pClassSpecificInterfaceMSC->IsReady(lun) !=0 )
		{
			SCSI_SenseCode(pdev,
						   lun,
						   NOT_READY,
						   MEDIUM_NOT_PRESENT);
			return -1;
		}

		/* Check If media is write-protected */
		if(pdev->pClassSpecificInterfaceMSC->IsWriteProtected(lun) !=0 )
		{
			SCSI_SenseCode(pdev,
						   lun,
						   NOT_READY,
						   WRITE_PROTECTED);
			return -1;
		}


		hmsc->scsi_blk_addr = (params[2] << 24) | \
				(params[3] << 16) | \
				(params[4] <<  8) | \
				params[5];
		hmsc->scsi_blk_len = (params[7] <<  8) | \
				params[8];

		/* check if LBA address is in the right range */
		if(SCSI_CheckAddressRange(pdev,
								  lun,
								  hmsc->scsi_blk_addr,
								  hmsc->scsi_blk_len) < 0)
		{
			return -1; /* error */
		}

		hmsc->scsi_blk_addr *= hmsc->scsi_blk_size;
		hmsc->scsi_blk_len  *= hmsc->scsi_blk_size;

		/* cases 3,11,13 : Hn,Ho <> D0 */
		if (hmsc->cbw.dDataLength != hmsc->scsi_blk_len)
		{
			SCSI_SenseCode(pdev,
						   hmsc->cbw.bLUN,
						   ILLEGAL_REQUEST,
						   INVALID_CDB);
			return -1;
		}

		//serialDebugWrite("Starting write operation for LBA=%08x, len=%d\n", hmsc->scsi_blk_addr, hmsc->scsi_blk_len);
		//serialDebugWrite("Receiving first block into buf=%d\n", hmsc->bot_data_idx);

		/* Prepare EP to receive first data packet */
		hmsc->bot_state = USBD_BOT_DATA_OUT_1ST;
		hmsc->bot_data_idx = 0;
		//serialDebugWriteC('R');
		USBD_LL_PrepareReceive (pdev,
								MSC_OUT_EP,
								hmsc->bot_data,
								MIN (hmsc->scsi_blk_len, MSC_MEDIA_PACKET));
	}
	else /* Write Process ongoing */
	{
		return SCSI_ProcessWrite(pdev, lun);
	}
	return 0;
}


/**
* @brief  SCSI_Verify10
*         Process Verify10 command
* @param  lun: Logical unit number
* @param  params: Command parameters
* @retval status
*/

static int8_t SCSI_Verify10(USBD_HandleTypeDef  *pdev, uint8_t lun , uint8_t *params)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  if ((params[1]& 0x02) == 0x02) 
  {
    SCSI_SenseCode (pdev,
                    lun, 
                    ILLEGAL_REQUEST, 
                    INVALID_FIELED_IN_COMMAND);
    return -1; /* Error, Verify Mode Not supported*/
  }
  
  if(SCSI_CheckAddressRange(pdev,
                            lun, 
                            hmsc->scsi_blk_addr, 
                            hmsc->scsi_blk_len) < 0)
  {
    return -1; /* error */      
  }
  hmsc->bot_data_length = 0;
  return 0;
}

/**
* @brief  SCSI_CheckAddressRange
*         Check address range
* @param  lun: Logical unit number
* @param  blk_offset: first block address
* @param  blk_nbr: number of block to be processed
* @retval status
*/
static int8_t SCSI_CheckAddressRange (USBD_HandleTypeDef  *pdev, uint8_t lun , uint32_t blk_offset , uint16_t blk_nbr)
{
  USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC; 
  
  if ((blk_offset + blk_nbr) > hmsc->scsi_blk_nbr )
  {
    SCSI_SenseCode(pdev,
                   lun, 
                   ILLEGAL_REQUEST, 
                   ADDRESS_OUT_OF_RANGE);
    return -1;
  }
  return 0;
}

/**
* @brief  SCSI_ProcessRead
*         Handle Read Process
* @param  lun: Logical unit number
* @retval status
*/
static int8_t SCSI_ProcessRead (USBD_HandleTypeDef  *pdev, uint8_t lun)
{
	USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC;
	uint32_t len;

	//serialDebugWriteC('R');
	len = MIN(hmsc->scsi_blk_len , MSC_MEDIA_PACKET);

	if( pdev->pClassSpecificInterfaceMSC->Read(lun ,
											   hmsc->bot_data + hmsc->bot_data_idx * MSC_MEDIA_PACKET,
											   hmsc->scsi_blk_addr / hmsc->scsi_blk_size,
											   len / hmsc->scsi_blk_size,
											   pdev) < 0)
	{

		SCSI_SenseCode(pdev,
					   lun,
					   HARDWARE_ERROR,
					   UNRECOVERED_READ_ERROR);
		serialDebugWrite("Faled at point 2\n");
		return -1;
	}

	hmsc->bot_state = USBD_BOT_DATA_IN;
	return 0;
}

void cardReadCompletedCB(uint8_t res, void * context)
{
	USBD_HandleTypeDef * pdev = (USBD_HandleTypeDef *)context;
	USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC;

	uint8_t lun = hmsc->cbw.bLUN;
	uint32_t len = MIN(hmsc->scsi_blk_len , MSC_MEDIA_PACKET);

	// Synchronization to avoid several transmits at a time
	// This must be located here as it waits finishing previous USB transfer
	// while the code below prepares next one
	pdev->pClassSpecificInterfaceMSC->OnFinishOp();

	//serialDebugWriteC('C');

	if(res != 0)
	{
		SCSI_SenseCode(pdev,
					   lun,
					   HARDWARE_ERROR,
					   UNRECOVERED_READ_ERROR);
		serialDebugWrite("Last read operation completed with errors\n");
		return;
	}

	//serialDebugWrite("Transmitting LBA=%08x, len=%d, buf=%08x\n", hmsc->scsi_blk_addr/512, len/512, hmsc->bot_data + hmsc->bot_data_idx * MSC_MEDIA_PACKET);

	// Save these values for transmitting data
	uint8_t * txBuf = hmsc->bot_data + hmsc->bot_data_idx * MSC_MEDIA_PACKET;
	uint16_t txSize = len;

	// But before transmitting set the correct state
	// Note: we are in context of SD thread, not the USB interrupt
	// So values have to be correct when DataIn interrupt occurrs
	hmsc->scsi_blk_addr   += len;
	hmsc->scsi_blk_len    -= len;

	/* case 6 : Hi = Di */
	hmsc->csw.dDataResidue -= len;

	//serialDebugWrite("%d bytes to go\n", hmsc->scsi_blk_len);

	if (hmsc->scsi_blk_len == 0)
	{
		hmsc->bot_state = USBD_BOT_LAST_DATA_IN;
	}
	else
	{
		hmsc->bot_data_idx ^= 1;
		hmsc->bot_data_length = MSC_MEDIA_PACKET;
		SCSI_ProcessRead(pdev, lun); // Not checking error code - SCSI_ProcessRead() already enters error state in case of read failure
	}

	//serialDebugWriteC('T');

	// Now we can transmit data read from SD
	USBD_LL_Transmit (pdev,
					  MSC_IN_EP,
					  txBuf,
					  txSize);
}

/**
* @brief  SCSI_ProcessWrite
*         Handle Write Process
* @param  lun: Logical unit number
* @retval status
*/
static int8_t SCSI_ProcessWrite (USBD_HandleTypeDef  *pdev, uint8_t lun)
{
	uint32_t len;
	USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC;
	len = MIN(hmsc->scsi_blk_len , MSC_MEDIA_PACKET);

	USBD_WriteBlockContext * ctxt = hmsc->write_ctxt + hmsc->bot_data_idx;

	// Figure out what to do after writing the block
	if(hmsc->scsi_blk_len == len)
	{
		//serialDebugWrite("  This will be the last block\n");
		ctxt->next_write_len = 0xffffffff;
	}
	else if(hmsc->scsi_blk_len == len + MSC_MEDIA_PACKET)
	{
		//serialDebugWrite("  This will be one before the last block\n");
		ctxt->next_write_len = 0;
	}
	else
	{
		//serialDebugWrite("  This will be regular block\n");
		ctxt->next_write_len = MIN(hmsc->scsi_blk_len - 2 * MSC_MEDIA_PACKET, MSC_MEDIA_PACKET);
	}

	// Prepare other fields of the context
	ctxt->buf = hmsc->bot_data + hmsc->bot_data_idx * MSC_MEDIA_PACKET;
	ctxt->pdev = pdev;


	// Do not allow several receives at a time
	if(hmsc->bot_state != USBD_BOT_DATA_OUT_1ST)
		pdev->pClassSpecificInterfaceMSC->OnStartOp();

	// Write received data
	serialDebugWriteC('W');

	if(pdev->pClassSpecificInterfaceMSC->Write(lun ,
											   ctxt->buf,
											   hmsc->scsi_blk_addr / hmsc->scsi_blk_size,
											   len / hmsc->scsi_blk_size,
											   ctxt) < 0)
	{
		SCSI_SenseCode(pdev,
					   lun,
					   HARDWARE_ERROR,
					   WRITE_FAULT);
		serialDebugWrite("Write failed at point 1\n");
		return -1;
	}

	// Switching blocks
	hmsc->bot_data_idx ^= 1;
	hmsc->scsi_blk_addr  += len;
	hmsc->scsi_blk_len   -= len;

	/* case 12 : Ho = Do */
	hmsc->csw.dDataResidue -= len;

	// Performing one extra receive for the first time in order to run receive and write operations in parallel
	if(hmsc->bot_state == USBD_BOT_DATA_OUT_1ST && hmsc->scsi_blk_len != 0)
	{
		//serialDebugWrite("Receiving an extra block into buf=%d\n", hmsc->bot_data_idx);

		hmsc->bot_state = USBD_BOT_DATA_OUT;
		//serialDebugWriteC('X');
		USBD_LL_PrepareReceive (pdev,
								MSC_OUT_EP,
								hmsc->bot_data + hmsc->bot_data_idx * MSC_MEDIA_PACKET, // Second buffer
								MIN (hmsc->scsi_blk_len, MSC_MEDIA_PACKET));
	}

	return 0;
}

void cardWriteCompletedCB(uint8_t res, void * context)
{
	USBD_WriteBlockContext * ctxt = (USBD_WriteBlockContext*)context;
	USBD_HandleTypeDef * pdev = ctxt->pdev;
	USBD_MSC_BOT_HandleTypeDef  *hmsc = pdev->pClassDataMSC;

	uint8_t lun = hmsc->cbw.bLUN;

	//serialDebugWrite("Write completed callback with status %d (buf=%d)\n", res, (ctxt->buf - hmsc->bot_data) / MSC_MEDIA_PACKET);
	//serialDebugWriteC('C');

	// Check error code first
	if(res != 0)
	{
		SCSI_SenseCode(pdev,
					   lun,
					   HARDWARE_ERROR,
					   WRITE_FAULT);
		serialDebugWrite("Write failed at point 2\n");
		return;
	}

	if (ctxt->next_write_len == 0xffffffff)
	{
		//serialDebugWrite("Write finished. Sending CSW\n");
		//serialDebugWriteC('!');
		MSC_BOT_SendCSW (pdev, USBD_CSW_CMD_PASSED);
	}
	else
	{
		pdev->pClassSpecificInterfaceMSC->OnFinishOp();

		if(ctxt->next_write_len != 0)
		{
			//serialDebugWrite("Preparing next receive into buf=%d\n", (ctxt->buf - hmsc->bot_data) / MSC_MEDIA_PACKET);

			/* Prepare EP to Receive next packet */
			//serialDebugWriteC('-');
			USBD_LL_PrepareReceive (pdev,
									MSC_OUT_EP,
									ctxt->buf,
									ctxt->next_write_len);
		}
	}
}


/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
