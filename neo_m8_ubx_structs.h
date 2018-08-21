/*
 * neo_m8_ubx_structs.h
 *
 *  Created on: Jul 17, 2018
 *      Author: alexis
 */

#ifndef NEO_M8_UBX_STRUCTS_H_
#define NEO_M8_UBX_STRUCTS_H_

#include <stdint.h>
#include <stdbool.h>

/* See the page 309 of UBX-13003221 - R15 for details */
typedef enum {
	PSM_NOT_ACTIVE 		= 0,
	PSM_ENABLED 	   		= 1,
	PSM_ACQUISITION 		= 2,
	PSM_TRACKING			= 3,
	PSM_OPT_TRACKING 	= 4,
	PSM_INACTIVE			= 5
} ubx_power_save_mode;

/* See the page 309 of UBX-13003221 - R15 for details */
typedef enum {
	NO_CARRIER_PHASE_RANGE_SOLUTION = 0,
	FLOAT_SOLUTION					= 1,
	FIXED_SOLUTION					= 2
} ubx_carrier_phase_range;


/* See the page 307 of UBX-13003221 - R15 for the units and scalings */
typedef struct {
	unsigned long 	iTOW;		/*(ms) GPS time of week of the navigation epoch. */
	unsigned short 	year;		/* Year (UTC) */
	unsigned char   month;		/* Month, range 1..12 (UTC) */
	unsigned char   day;			/* Day of month, range 1..31 (UTC) */
	unsigned char   hour;		/* Hour of day, range 0..23 (UTC) */
	unsigned char 	min;			/* Minute of hour, range 0..59 (UTC) */
	unsigned char 	sec;			/* Seconds of minute, range 0..60 (UTC) */
	unsigned char   valid; 	 	/* Validity flags */
	unsigned long 	tAcc;		/* GPS time of week of the navigation epoch. */
	signed   long  	nano;		/* Fraction of second, range -1e9 .. 1e9 (UTC) */
	unsigned char   fixType;		/* GNSSfix Type: 0: no fix ; 1: dead reckoning only 2: 2D-fix
												 3: 3D-fix   4: GNSS + dead reckoning combined
												 5: time only fix */
	unsigned char 	flags;		/* Fix status flags (see graphic below) */
	unsigned char	flags2;		/* Additional flags (see graphic below) */
	unsigned char	numSV;		/* Number of satellites used in Nav Solution */
	signed long		lon;			/* Longitude */
	signed long		lat;			/* Latitude (deg) */
	signed long		height;		/* Height above ellipsoid */
	signed long		hMSL;		/* Height above mean sea level */
	signed long		hAcc;		/* Horizontal accuracy estimate */
	unsigned long	vAcc;		/* Vertical accuracy estimate */
	signed long		velN;		/* NED north velocity */
	signed long		velE;		/* NED east velocity */
	signed long		velD;	 	/* NED down velocity */
	signed long 		gSpeed;		/* Ground Speed (2-D) */
	signed long		headMot;		/* Heading of motion (2-D) */
	unsigned long	sAcc;		/* Speed accuracy estimate */
	unsigned long 	headAcc;		/* Heading accuracy estimate (both motion and vehicle) */
	unsigned short	pDOP;		/* Position DOP */
/*  unsigned char	reserved;*/ /* Byte offset 78 is reserved, this is only here to indicate it */
	signed long		headVeh;		/* Heading of vehicle (2-D) */
	signed short		magDec;		/* Magnetic declination */
	unsigned short	magAcc;		/* Magnetic declination accuracy */

	/* Bitfield valid */
	bool				validDate;		/* 1 = valid UTC Date (see Time Validity section for details) */
	bool				validTime;		/* 1 = valid UTC Time of Day (see Time Validity section for details) */
	bool				fullyResolved;	/* 1 = UTC Time of Day has been fully resolved (no seconds uncertainty) */
	bool 			validMag;		/* 1 = valid Magnetic declination */

	/*	Bitfield flags */
	bool 			gnssFixOK;			/* 1 = valid fix (i.e within DOP & accuracy masks) */
	bool				diffSoln;			/* 1 = differential corrections were applied */
	bool				headVehValid;		/* 1 = heading of vehicle is valid */
	ubx_power_save_mode		psmState;	/* Power Save Mode state (see Power Management) */
	ubx_carrier_phase_range	carrSoln;	/* Carrier phase range solution status */


	/* Bitfield flags2 */
	bool 			confirmedAvai;		/* 1 = information about UTC Date and Time of Day validity confirmation is available */
	bool 			confirmedDate;		/* 1 = UTC Date validity could be confirmed (see Time Validity section for details) */
	bool				confirmedTime;		/* 1 = UTC Time of Day could be confirmed (see Time Validity section for details) */

} ubx_nav_pvt_t;




#endif /* NEO_M8_UBX_STRUCTS_H_ */
