/*
 * DS3231.h
 *
 *  Created on: 17 dic. 2020
 *      Author: William
 */

#ifndef MAIN_INCLUDE_DS3231_H_
#define MAIN_INCLUDE_DS3231_H_


void tareaDS3231 (void* P);
char* leerds3231( uint8_t* seg, uint8_t* min, uint8_t* hor,
			uint8_t* fec, uint8_t* mes, uint8_t* ani, uint8_t* ampm );


#endif /* MAIN_INCLUDE_DS3231_H_ */
