/*
 * PruebasNoFinal.h
 *
 *  Created on: 17 dic. 2020
 *      Author: William
 */

#ifndef MAIN_INCLUDE_INITHORARIOS_H_
#define MAIN_INCLUDE_INITHORARIOS_H_



void initParameters();
void publicarInfo( struct salones Salones, char * mensaje, uint8_t i, uint8_t j, uint8_t k );
void EliminarAsig( struct salones Salones, char * mensaje, uint8_t i, uint8_t j, uint8_t k, u );
void InsertarAsig( struct salones Salones, char * mensaje, uint8_t i, uint8_t j, uint8_t k, uint8_t asgtr );
void RestartHorarios( struct salones Salones );



#endif /* MAIN_INCLUDE_INITHORARIOS_H_ */
