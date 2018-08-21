/*
 * neo_m8_conversion.c
 *
 *  Created on: Jul 17, 2018
 *      Author: alexis
 */

#include "neo_m8_conversion.h"
#include <stdint.h>

bool UBX_Parse_Raw_To_NAV_PVT(char* buffer, uint32_t len, ubx_nav_pvt_t * dest)
{
	/* Length must be 92 */
	if (len != 92)
	{
		//todo: assert/error handler?
		return false;
	}

	dest->iTOW 	 = (buffer[3] << 24) | (buffer[2] << 16) | (buffer [1] << 8) | buffer[0] ;
	dest->year 	 = (buffer[5] << 8 ) | (buffer[4]) ;
	dest->month	 =  buffer[6] ;
	dest->day	 =  buffer[7] ;
	dest->hour	 =  buffer[8] ;
	dest->min	 =  buffer[9] ;
	dest->sec	 =  buffer[10];

	/* Handle the 'validity flags' in byte 11 */
	dest->validDate 		=  buffer[11] 	    & 0b00000001;
	dest->validTime 		= (buffer[11] >> 1) & 0b00000001;
	dest->fullyResolved	= (buffer[11] >> 2) & 0b00000001;
	dest->validMag		= (buffer[11] >> 3) & 0b00000001;

	dest->tAcc    = (buffer[15] << 24) | (buffer[14] << 16) | (buffer [13] << 8) | buffer[12] ;
	dest->nano	  = (buffer[19] << 24) | (buffer[18] << 16) | (buffer [17] << 8) | buffer[16] ;
	dest->fixType =  buffer[20] ;

	/* Handle the 'flags' in byte 21 */
	dest->gnssFixOK		=  buffer[21] 	    & 0b00000001;
	dest->diffSoln		= (buffer[21] >> 1) & 0b00000001;
	dest->psmState		= (buffer[21] >> 2) & 0b00000111;
	dest->headVehValid  = (buffer[21] >> 5) & 0b00000001;
	dest->carrSoln		= (buffer[21] >> 6) & 0b00000011;

	/* Handle the 'flags' in byte 22 */
	dest->confirmedAvai = (buffer[22] >> 5) & 0b00000001;
	dest->confirmedDate = (buffer[22] >> 6) & 0b00000001;
	dest->confirmedTime = (buffer[22] >> 7) & 0b00000001;

	dest->numSV 	  =  buffer[23];

	dest->lon	  = (buffer[27] << 24) | (buffer[26] << 16) | (buffer [25] << 8) | buffer[24] ;
	dest->lat	  = (buffer[31] << 24) | (buffer[30] << 16) | (buffer [29] << 8) | buffer[28] ;
	dest->height  = (buffer[35] << 24) | (buffer[34] << 16) | (buffer [33] << 8) | buffer[32] ;
	dest->hMSL    = (buffer[39] << 24) | (buffer[38] << 16) | (buffer [37] << 8) | buffer[36] ;
	dest->hAcc    = (buffer[43] << 24) | (buffer[42] << 16) | (buffer [41] << 8) | buffer[40] ;
	dest->vAcc    = (buffer[47] << 24) | (buffer[46] << 16) | (buffer [45] << 8) | buffer[44] ;
	dest->velN    = (buffer[51] << 24) | (buffer[50] << 16) | (buffer [49] << 8) | buffer[48] ;
	dest->velE    = (buffer[55] << 24) | (buffer[54] << 16) | (buffer [53] << 8) | buffer[52] ;
	dest->velD    = (buffer[59] << 24) | (buffer[58] << 16) | (buffer [57] << 8) | buffer[56] ;
	dest->gSpeed  = (buffer[63] << 24) | (buffer[62] << 16) | (buffer [61] << 8) | buffer[60] ;
	dest->headMot = (buffer[67] << 24) | (buffer[66] << 16) | (buffer [65] << 8) | buffer[64] ;
	dest->sAcc    = (buffer[71] << 24) | (buffer[70] << 16) | (buffer [69] << 8) | buffer[68] ;
	dest->headAcc = (buffer[75] << 24) | (buffer[74] << 16) | (buffer [73] << 8) | buffer[72] ;

	dest->pDOP	  = (buffer[77] << 8 ) | (buffer[76]) ;

	/* Bytes 78 to 83 included are reserved */

	dest->headVeh = (buffer[87] << 24) | (buffer[86] << 16) | (buffer [85] << 8) | buffer[84] ;
	dest->magDec  = (buffer[89] << 8 ) | (buffer[88]) ;
	dest->magAcc  = (buffer[91] << 8 ) | (buffer[90]) ;


	return true;

}
