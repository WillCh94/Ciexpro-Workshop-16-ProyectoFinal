/*
 * comitsHorarios.c
 *
 *  Created on: 18 dic. 2020
 *      Author: William
 */




#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>
#include "InitHorarios.h"




enum Bloques auxBloques;
enum horariosIN auxHorariosIN;
enum horariosOUT auxHorariosOUT;
enum Bloques auxBloques;
enum BloqNomb auxBloqNomb;
enum horariosIN auxHorariosIN;
enum horariosOUT auxHorariosOUT;
extern enum Dias auxDias;
//extern enum DiasNomb auxDiasNomb;
extern enum Asignts auxAsignts;
extern enum AsigntsN auxAsigntsN;
extern enum Aulas auxAulas;
//extern int AulasInit[ sizeof( auxAulas ) ][ sizeof ( auxDias ) * sizeof( auxBloques ) ] ;

extern struct salones {

	struct diaClase{

		struct bloquesClase{

			int8_t Asignatura;
			char *AsigntName;
			int8_t Bloque;
			char *BloqueName;
			char *DiaName;
			struct BloqueHorarios{

				char Ini[11];		//"XX:XX:XX YM"
				char Fin[11];		//"XX:XX:XX YM"

			} Hora;

		} bloque[ sizeof( auxBloques ) ];

	} dia[ sizeof ( auxDias ) ];

} Salones [ sizeof( auxAulas ) ]  ;



void publicarInfo( struct salones Salones, char * mensaje, uint8_t i, uint8_t j, uint8_t k ){

	strcpy(mensaje, Salones[ i ].dia[ j ].bloque[ k ].AsigntName );
	strcpy(mensaje, ". " );
	strcpy(mensaje, Salones[ i ].dia[ j ].bloque[ k ].BloqueName );
	strcpy(mensaje, ". " );
	strcpy(mensaje, Salones[ i ].dia[ j ].bloque[ k ].AsigntName );
	strcpy(mensaje, " - " );
	strcpy(mensaje, Salones[ i ].dia[ j ].bloque[ k ].Hora.Ini );
	strcpy(mensaje, Salones[ i ].dia[ j ].bloque[ k ].Hora.Fin );
	strcpy(mensaje, " - " );
	strcpy(mensaje, Salones[ i ].dia[ j ].bloque[ k ].DiaName );

}



void EliminarAsig( struct salones Salones, char * mensaje, uint8_t i, uint8_t j, uint8_t k, u ){

	Salones[ i ].dia[ j ].bloque[ k ].Asignatura = auxAsignts[ 0 ];
	strcpy ( Salones[ i ].dia[ j ].bloque[ k ].AsigntName , auxAsigntsN [ 0 ] );
	strcpy ( mensaje, "valores eliminados exitosamente" );

}



void InsertarAsig( struct salones Salones, char * mensaje, uint8_t i, uint8_t j, uint8_t k, uint8_t asgtr ){

	Salones[ i ].dia[ j ].bloque[ k ].Asignatura = auxAsignts[ asgtr ];
	strcpy ( Salones[ i ].dia[ j ].bloque[ k ].AsigntName , auxAsigntsN [ asgtr ] );
	strcpy ( mensaje, "valores modificados exitosamente" );

}


void RestartHorarios( struct salones Salones ){

	initParameters();

}





