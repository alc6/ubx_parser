/*
 * neo_m8_messages.h
 *
 *  Created on: Jul 17, 2018
 *      Author: alexis
 */

#ifndef NEO_M8_MESSAGES_H_
#define NEO_M8_MESSAGES_H_

/* See page 134 of document UBX-13003221 - R15 for more precisions */

/*
 *  • Every Frame starts with a 2-byte Preamble consisting of two synchronization characters: 0xB5 0x62.
 *	• A 1-byte Message Class field follows. A Class is a group of messages that are related to each other.
 *	• A 1-byte Message ID field defines the message that is to follow.
 *	• A 2-byte Length field follows. The length is defined as being that of the payload only. It does not include
 *		the Preamble, Message Class, Message ID, Length, or CRC fields. The number format of the length field is a
 *		Little-Endian unsigned 16-bit integer.
 *	• The Payload field contains a variable number of bytes.
 *	• The two 1-byte CK_A and CK_B fields hold a 16-bit checksum whose calculation is defined below. This
 *		concludes the Frame.
 */



#define UBX_SYNC_CHAR1	0xB5
#define UBX_SYNC_CHAR2	0x62

/* UBX Class IDs */
/* See UBX-13003221 - R15 Early Production Information, Page 137 of 386 for details */
#define UBX_MSG_CLASS_ACK	0x05		/* Ack/Nak Messages: Acknowledge or Reject messages to CFG input messages */
#define UBX_MSG_CLASS_AID	0x0B		/* AssistNow Aiding Messages: Ephemeris, Almanac, other A-GPS data input */
#define UBX_MSG_CLASS_CFG	0x06		/* Configuration Input Messages: Set Dynamic Model, Set DOP Mask, Set Baud Rate, etc. */
#define UBX_MSG_CLASS_ESF	0x10		/* External Sensor Fusion Messages: External Sensor Measurements and Status Information */
#define UBX_MSG_CLASS_HNR	0x28		/* High Rate Navigation Results Messages: High rate time, position, speed, heading */
#define UBX_MSG_CLASS_INF	0x04		/* Information Messages: Printf-Style Messages, with IDs such as Error, Warning, Notice */
#define UBX_MSG_CLASS_LOG	0x21		/* Logging Messages: Log creation, deletion, info and retrieval */
#define UBX_MSG_CLASS_MGA   0x13		/* Multiple GNSS Assistance Messages: Assistance data for various GNSS */
#define UBX_MSG_CLASS_MON	0x0A 	/* Monitoring Messages: Communication Status, CPU Load, Stack Usage, Task Status */
#define UBX_MSG_CLASS_NAV 	0x01		/* Navigation Results Messages: Position, Speed, Time, Acceleration, Heading, DOP, SVs used */
#define UBX_MSG_CLASS_RXM	0x02 	/* Receiver Manager Messages: Satellite Status, RTC Status */
#define UBX_MSG_CLASS_SEC	0x27		/* Security Feature Messages */
#define UBX_MSG_CLASS_TIM	0x0D		/* Timing Messages: Time Pulse Output, Time Mark Results */
#define UBX_MSG_CLASS_UPD	0x09		/* Firmware Update Messages: Memory/Flash erase/write, Reboot, Flash identification, etc. */

/* UBX Class ACK */
#define UBX_MSG_ID_ACK_ACK 	0x01
#define UBX_MSG_ID_ACK_NAK 	0x00

/* UBX Class AID */
#define UBX_MSG_ID_AID_ALM 	0x30
#define UBX_MSG_ID_AID_AOP 	0x33
#define UBX_MSG_ID_AID_EPH 	0x31
#define UBX_MSG_ID_AID_HUI 	0x02
#define UBX_MSG_ID_AID_INI 	0x01

/* UBX Class CFG */
#define UBX_MSG_ID_CFG_ANT 			0x13
#define UBX_MSG_ID_CFG_BATCH			0x93
#define UBX_MSG_ID_CFG_CFG 			0x09
#define UBX_MSG_ID_CFG_DAT 			0x06
#define UBX_MSG_ID_CFG_DGNSS 		0x70
#define UBX_MSG_ID_CFG_DOSC 			0x61
#define UBX_MSG_ID_CFG_DYNSEED		0x85
#define UBX_MSG_ID_CFG_ESRC 			0x60
#define UBX_MSG_ID_CFG_FIXSEED 		0x84
#define UBX_MSG_ID_CFG_GEOFENCE	 	0x69
#define UBX_MSG_ID_CFG_GNSS 			0x3E
#define UBX_MSG_ID_CFG_HNR    		0x5C
#define UBX_MSG_ID_CFG_INF 			0x02
#define UBX_MSG_ID_CFG_ITFM 			0x39
#define UBX_MSG_ID_CFG_LOGFILTER 	0x47
#define UBX_MSG_ID_CFG_MSG			0x01
#define UBX_MSG_ID_CFG_NAV5 			0x24
#define UBX_MSG_ID_CFG_NAVX5 		0x23
#define UBX_MSG_ID_CFG_NMEA 		    0x17
#define UBX_MSG_ID_CFG_ODO 			0x1E
#define UBX_MSG_ID_CFG_PM2 			0x3B
#define UBX_MSG_ID_CFG_PMS 			0x86
#define UBX_MSG_ID_CFG_PRT 			0x00
#define UBX_MSG_ID_CFG_PWR 			0x57
#define UBX_MSG_ID_CFG_RATE 			0x08
#define UBX_MSG_ID_CFG_RINV 			0x34
#define UBX_MSG_ID_CFG_RST 			0x04
#define UBX_MSG_ID_CFG_RXM 			0x11
#define UBX_MSG_ID_CFG_SBAS 			0x16
#define UBX_MSG_ID_CFG_SMGR 			0x62
#define UBX_MSG_ID_CFG_TMODE2 		0x3D
#define UBX_MSG_ID_CFG_TMODE3 		0x71
#define UBX_MSG_ID_CFG_TP5 			0x31
#define UBX_MSG_ID_CFG_TXSLOT 		0x53
#define UBX_MSG_ID_CFG_USB 			0x1B

//todo: add esf, hnr, inf, log, mga, rxm, sec, tim, and upd sections

/* UBX Class NAV */
#define UBX_MSG_ID_NAV_AOPSTATUS 	0x60
#define UBX_MSG_ID_NAV_ATT 			0x05
#define UBX_MSG_ID_NAV_CLOCK 		0x22
#define UBX_MSG_ID_NAV_DGPS 			0x31
#define UBX_MSG_ID_NAV_DOP 			0x04
#define UBX_MSG_ID_NAV_EOE 			0x61
#define UBX_MSG_ID_NAV_GEOFENCE 		0x39
#define UBX_MSG_ID_NAV_HPPOSECEF 	0x13
#define UBX_MSG_ID_NAV_HPPOSLLH 		0x14
#define UBX_MSG_ID_NAV_ODO 			0x09
#define UBX_MSG_ID_NAV_ORB 			0x34
#define UBX_MSG_ID_NAV_POSECEF 		0x01
#define UBX_MSG_ID_NAV_POSLLH 		0x02
#define UBX_MSG_ID_NAV_PVT 			0x07
#define UBX_MSG_ID_NAV_RELPOSNED 	0x3C
#define UBX_MSG_ID_NAV_RESETODO		0x10 /* This is a command */
#define UBX_MSG_ID_NAV_SAT 			0x35
#define UBX_MSG_ID_NAV_SBAS 			0x32
#define UBX_MSG_ID_NAV_SOL 			0x06
#define UBX_MSG_ID_NAV_STATUS 		0x03
#define UBX_MSG_ID_NAV_SVINFO 		0x30
#define UBX_MSG_ID_NAV_SVIN 			0x3B
#define UBX_MSG_ID_NAV_TIMEBDS 		0x24
#define UBX_MSG_ID_NAV_TIMEGAL 		0x25
#define UBX_MSG_ID_NAV_TIMEGLO 		0x23
#define UBX_MSG_ID_NAV_TIMEGPS 		0x20
#define UBX_MSG_ID_NAV_TIMELS 		0x26
#define UBX_MSG_ID_NAV_TIMEUTC 		0x21
#define UBX_MSG_ID_NAV_VELECEF 		0x11
#define UBX_MSG_ID_NAV_VELNED 		0x12





#endif /* NEO_M8_MESSAGES_H_ */
