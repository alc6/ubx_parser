/*
 * neo_m8_gps.c
 *
 *  Created on: Jul 17, 2018
 *      Author: alexis
 */

#include "ringbuffer_char.h"
#include "neo_m8_gps.h"
#include "usart.h"
#include "string.h"
#include "neo_m8_messages.h"
#include "neo_m8_ubx_checksum.h"

/* Variables */
/* Ring buffer to store incoming bytes ; raw data storage */
static ring_buffer_t gps_rx_ring_buffer;

static char UBXPayload[128];
static UBXHeader header;
static char CRC_A;
static char CRC_B;

static UBXMsg ValidUBXMsg;


//todo: remove
static char gps_raw_buffer[200]; /* Buffer used to extract msg from ring buffer */
static char dbg_buf[128];

/* Initialisation function */
void neoInit(void)
{
	/* Init the reception buffer */
	neoInitRxBuf();

	/* Enable interrupts */
	//todo: use a define for the USART name and handler
    HAL_NVIC_SetPriority(USART6_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
    /* Enable RX Not Empty interrupt */
    __HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);
}


void neoInitRxBuf(void)
{
	ring_buffer_init(&gps_rx_ring_buffer);
}

uint32_t neoNumElemBuf(void)
{
	return (uint32_t) ring_buffer_num_items(&gps_rx_ring_buffer);
}


/* Rx callback to handle the reception of one byte */
void neoRxCallback(char c)
{
#if 0 //todo: remove
	uint32_t s;
	s = snprintf(dbg_buf, 128, "rxd:%0x\n", c);
	HAL_UART_Transmit(&huart3, dbg_buf, s, 0xff);
#endif
	// Queue in ring buffer the rx'd byte
	ring_buffer_queue(&gps_rx_ring_buffer, c);
	//todo: fill ring buffer
}

void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */
  if (__HAL_UART_GET_FLAG(&huart6, UART_FLAG_RXNE))
  {
	  /*Clear the interrupt by reading the DR:*/
	  char rxdByte;
	  rxdByte = huart6.Instance->DR;
	  neoRxCallback(rxdByte);
  }
  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

UBX_OP_RESULT neoRetrieveMsg(void)
{
	static UBX_NEO_M8_DECODE_STATE smState = UBX_LOOKING_FOR_SYNC;
	switch (smState)
	{
		case UBX_LOOKING_FOR_SYNC:
		{
			if (neoFindSyncBytes())
			{
				smState = UBX_BUILDING_MSG_INFOS;
				/* Flow through */
			}
			else
			{
				return UBX_NO_DATA;
				break;
			}
		}

		case UBX_BUILDING_MSG_INFOS:
		{
			if (neoBuildHeader(&header))
			{
				/* Retrieved a header (informations of the UBX Packet: ID, Class, Len) */
				/* Check the length, in case a error a very high value can be reached */
				if (header.length > 172) //todo: define the max length of a ubx payload
				{
					/* That's too high, switch to previous state again */
					smState = UBX_LOOKING_FOR_SYNC;
					break;
				}
				else
				{
					/* Valide length, allow flow through the next state */
					smState = UBX_CHECKING_CRC;
				}
			}
			else
			{
				break;
			}
		}

		case UBX_CHECKING_CRC:
		{
			int i;
			char rxd_crca, rxd_crcb;
			/* Check if enough data is present inside the buffer for this operation */
			/* Header (4 bytes) + Payload (length bytes) + CRC (2 bytes) = length + 6 */
			/* Last two bytes are the CRC, we won't copy it into the raw buffer, but in separate values */
			if (ring_buffer_num_items(&gps_rx_ring_buffer) >= (header.length+6))
			{
				if (!neoCopyPacketFromRing(gps_raw_buffer, header.length+4))
				{
					/* Error to handle */
					break;
				}
				/* Arrive here only if copy was successfull and CRC is available in ring buffer */
				/* Verify checksum is correct */

				/* Retrieve Rxd CRC*/
				ring_buffer_peek(&gps_rx_ring_buffer, &rxd_crca, (header.length+4));
				ring_buffer_peek(&gps_rx_ring_buffer, &rxd_crcb, (header.length+5));

				/* Calculate CRC on Header (class + id + length) + payload */
				UBX_Fletcher(gps_raw_buffer, header.length+4, &CRC_A, &CRC_B);

				/* In any case, this is the end and it will be time to look for new SYNC bytes */
				smState = UBX_LOOKING_FOR_SYNC;

				/* Check CRC */
				if ((CRC_A == rxd_crca) && (CRC_B == rxd_crcb))
				{
					/* We retrieved a correct message, remove it from ring buffer */
					neoDequeueFromRing(header.length+6);
					memset(ValidUBXMsg.payload, 0, 172);
					memcpy(ValidUBXMsg.payload, &gps_raw_buffer[4], header.length);
					ValidUBXMsg.header = header;

					return UBX_NEW_DATA;
				}
				else
				{
					/* Else don't remove it and try to find a SYNC
					 * A packet could be present in the byte we dealt with, that's why we only peek'd */
					return UBX_INCORRECT_PACKET_RXD;
				}
			}
			else
			{
				/* Not enough item in the ring buffer, let him get feed */
				return UBX_WAITING_MORE_DATA;
				break;
			}

			/* Should never get there */
			break;
		}

	}
}

/* Browse into the RX Ring buffer to find SYNC Char 1 followed by SYNC Char 2, indicating the begin of a UBX msg */
/* Returns true if we retrieved this combination, false otherwise */
bool neoFindSyncBytes()
{
	char c1, c2;
	while (!ring_buffer_is_empty(&gps_rx_ring_buffer) && (ring_buffer_num_items(&gps_rx_ring_buffer) >= 2))
	{
		ring_buffer_dequeue(&gps_rx_ring_buffer, &c1);
		if (c1 == UBX_SYNC_CHAR1)
		{
			ring_buffer_peek(&gps_rx_ring_buffer, &c2, 0);
			if (c2 == UBX_SYNC_CHAR2)
			{
				ring_buffer_dequeue(&gps_rx_ring_buffer, &c2); /* Dequeue the byte we just peek'd before */
				return true; /* We just found SYNC1 followed by SYNC2 */
			}
		}
	}
	return false;
}

bool neoRetrieveHeaderBytes(char* buf)
{
    int  i;

    for (i = 0; i < 4; i++)
    {
    		if (!ring_buffer_peek(&gps_rx_ring_buffer, buf++, i))
    		{
    			/* The ring_buffer_peek will return false if the index don't exist
    			 * it means that there are less than 4 bytes! */
    			return false;
    		}
    }

    /* Correctly peeked 4 bytes from the ring buffer */
    return true;
}


bool neoBuildHeader(UBXHeader* header)
{
	//memset(header->rawBuf, 0, 4);

	if (neoRetrieveHeaderBytes(gps_raw_buffer))
	{
		/* Successfully retrieved 4 bytes to build a header */
		header->class  = gps_raw_buffer[0];
		header->id     = gps_raw_buffer[1];
		header->length = gps_raw_buffer[3] << 8 | gps_raw_buffer[2]; /* Little Endian */

		return true;
	}
	else
	{
		return false;
	}
}

bool neoDequeueFromRing(uint32_t n)
{
	char dummy;
	if (ring_buffer_num_items(&gps_rx_ring_buffer) >= n)
	{
		//dequeue
		for (int i = 0; i > n; i++)
		{
			ring_buffer_dequeue(&gps_rx_ring_buffer, &dummy);
		}
	}
}

/* This function copies bytes from a ring buffer to the raw buffer.
 * Second argument is a pointer to the raw buffer,
 * Third argument is the number of bytes to copy
 * Returns true if the copy occured correctly,
 * Returns false if something went wrong (not enough elements in the ring buffer)
 * Use this function after the sync byte were detected and dequeue'd like this:
 */
bool neoCopyPacketFromRing(char* rbuffer, uint32_t n)
{
	char dummy;
	int i;
	if (ring_buffer_num_items(&gps_rx_ring_buffer) >= n)
	{
		for (i = 0; i < (header.length+4); i++)
		{
			if (!ring_buffer_peek(&gps_rx_ring_buffer,&gps_raw_buffer[i], i))
			{
				/* Oops, no item at the index we indicated */
				return false;
			}
		}
	}
	else
	{
		return false;
	}
	/* OK */
	return true;
}

uint8_t neoGetMsgClass()
{
	return ValidUBXMsg.header.class;
}

uint8_t neoGetMsgId()
{
	return ValidUBXMsg.header.id;
}

uint16_t neoGetPayloadSize()
{
	return ValidUBXMsg.header.length;
}

void UBXUpdate_NAV_PVT(ubx_nav_pvt_t * dest)
{
	UBX_Parse_Raw_To_NAV_PVT(ValidUBXMsg.payload, ValidUBXMsg.header.length, dest);
}



