/*
 * neo_m8_conversion.h
 *
 *  Created on: Jul 17, 2018
 *      Author: alexis
 */

#ifndef NEO_M8_CONVERSION_H_
#define NEO_M8_CONVERSION_H_

#include "neo_m8_ubx_structs.h"

/* Input a UBX payload, outputs a parsed ubx_nav_pvt_msg_t struct */
bool UBX_Parse_Raw_To_NAV_PVT(char* buffer, uint32_t len, ubx_nav_pvt_msg_t * dest);

/* Input a UBX payload, outputs a parsed ubx_nav_att_msg_t struct */
bool UBX_Parse_Raw_To_NAV_ATT(char* buffer, uint32_t len, ubx_nav_att_msg_t * dest);

/* Input a UBX payload, outputs a parsed ubx_nav_dop_msg_t struct */
bool UBX_Parse_Raw_To_NAV_DOP(char* buffer, uint32_t len, ubx_nav_dop_msg_t * dest);

/* Input a UBX payload, outputs a parsed ubx_nav_odo_msg_t struct */
bool UBX_Parse_Raw_To_NAV_ODO(char* buffer, uint32_t len, ubx_nav_odo_msg_t * dest);

#endif /* NEO_M8_CONVERSION_H_ */
