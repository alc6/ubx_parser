/*
 * neo_m8_ubx_checksum.h
 *
 *  Created on: Jul 17, 2018
 *      Author: alexis
 */

#ifndef NEO_M8_UBX_CHECKSUM_H_
#define NEO_M8_UBX_CHECKSUM_H_

#include <stdint.h>

/* Straightforward Solution */
uint16_t Fletcher16( uint8_t *data, int count );

/* Following UBX's documentation */
void 	 UBX_Fletcher (uint8_t *data, int count, uint8_t * cka, uint8_t * ckb);

/* Check page 136 of document UBX-13003221 - R15 for more details */

/*
 *  The checksum algorithm used is the > 8-Bit Fletcher Algorithm <, which is used in the TCP standard (RFC 1145).
 *	This algorithm works as follows:
 *	Buffer[N] contains the data over which the checksum is to be calculated.
 *	The two CK_ values are 8-Bit unsigned integers, only! If implementing with larger-sized integer values, make
 *	sure to mask both CK_A and CK_B with 0xFF after both operations in the loop.
 */

/* Straightforward implementation of the Fletcher Algorithm */
uint16_t Fletcher16( uint8_t *data, int count )
{
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    int index;

    for( index = 0; index < count; ++index )
    {
       sum1 = (sum1 + data[index]) % 255;
       sum2 = (sum2 + sum1) % 255;
    }

    return (sum2 << 8) | sum1;
}

void 	 UBX_Fletcher (uint8_t *data, int count, uint8_t * cka, uint8_t * ckb)
{
	int i;

	*cka = 0;
	*ckb = 0;

	for (i = 0; i < count; i++)
	{
		*cka = *cka + data[i];
		*ckb = *ckb + *cka;
	}

}

#endif /* NEO_M8_UBX_CHECKSUM_H_ */
