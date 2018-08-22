/*
 * neo_m8_gps.h
 *
 *  Created on: Jul 17, 2018
 *      Author: alexis
 */

#ifndef NEO_M8_GPS_H_
#define NEO_M8_GPS_H_

/* Standard libraries */
#include <stdbool.h>
#include <stdint.h>

#include "usart.h"

#include "neo_m8_ubx_structs.h"


#define GPS_UART_INSTANCE UART6
#define GPS_UART_HANDLER  huart6

extern UART_HandleTypeDef GPS_UART_HANDLER;


/* Header struct used to have readable value (especially length, needed for the CRC calculation) */
typedef struct {
	uint8_t class;
	uint8_t id;
	uint16_t length; /* Payload length */
} UBXHeader;

typedef struct {
	UBXHeader header;
	char	      payload[172];
} UBXMsg;

typedef enum {
	UBX_NO_DATA,
	UBX_WAITING_MORE_DATA,
	UBX_INCORRECT_PACKET_RXD,
	UBX_NEW_DATA
} UBX_OP_RESULT;



/* Rx callback when a byte is rxd */
void neoRxCallback(char c);
/* Initialize the ring buffer */
void neoInitRxBuf(void);

/*Return number of elems inside the buffer */
uint32_t neoNumElemBuf(void);

/* Process the ring buffer to find a correct message */
UBX_OP_RESULT neoRetrieveMsg(void);

uint8_t neoGetMsgClass();

uint8_t neoGetMsgId();

uint16_t neoGetPayloadSize();

/* Updates */
void UBXUpdate_NAV_PVT(ubx_nav_pvt_msg_t * dest);


/* Private API */
/* Returns true if Sync Bytes are found into the ring buffer, and dequeue'd*/
bool neoFindSyncBytes();

/* Read 4 bytes from the ring buffer */
bool neoRetrieveHeaderBytes(char* buf);

/* Peek bytes from the ring buffer to parse the Header informations into a readable struct */
bool neoBuildHeader(UBXHeader* header);

/* Dequeue N elements from the ring buffer used by the GPS */
bool neoDequeueFromRing(uint32_t n);

/* Copy a UBX Packet without the SYNC Bytes and the CRC into a raw buffer */
bool neoCopyPacketFromRing(char* rbuffer, uint32_t n);


/* Enumeration of the GPS Decode state machine */
typedef enum {
	UBX_LOOKING_FOR_SYNC,     /* Looking for Sync char 1 and Sync char 2 */
	UBX_BUILDING_MSG_INFOS,   /* MSG Infos is Class, ID, and Length bytes (4) */
	UBX_BUILDING_MSG_PAYLOAD, /* Retrieving 'Length' bytes */
	UBX_BUILDING_CRC,         /* Retrieving 2 bytes (CRC) */
	UBX_CHECKING_CRC,		  /* Checking CRC, switch to first state if CRC is wrong,
							   * or to valid msg if it is right */
	UBX_VALID_MSG			  /* Valid message retrieved */
} UBX_NEO_M8_DECODE_STATE;


#endif /* NEO_M8_GPS_H_ */
