/*
 * PruebasNoFinal.c
 *
 *  Created on: 17 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>


enum Bloques{

	Bloque_A,
	Bloque_B,
	Bloque_C,
	Bloque_D,
	Bloque_E,
	NoBloque,

} auxBloques;

enum BloqNomb{

	BloqueA = "Bloque A",
	BloqueB = "Bloque B",
	BloqueC = "Bloque C",
	BloqueD = "Bloque D",
	BloqueE = "Bloque E",
	NoBloq = "No Bloque",

} auxBloqNomb;

enum horariosIN{

	horaIN1 = "07:00:00 AM",
	horaIN2 = "09:00:00 AM",
	horaIN3 = "11:00:00 AM",
	horaIN4 = "01:00:00 Pm",
	horaIN5 = "02:00:00 Pm",
	horaIn6 = "04:00:00 Pm",
	horaIN7 = "06:00:00 Pm",

} auxHorariosIN;


enum horariosOUT{

	horaOUT1 = "09:00:00 AM",
	horaOUT2 = "11:00:00 AM",
	horaOUT3 = "01:00:00 PM",
	horaOUT4 = "02:00:00 Pm",
	horaOUT5 = "04:00:00 Pm",
	horaOUT6 = "06:00:00 Pm",
	horaOUT7 = "07:00:00 AM",

} auxHorariosOUT;


enum Dias{

	domingo= 0,
	lunes,
	martes,
	miercoles,
	jueves,
	viernes,
	sabado,

} auxDias;


enum DiasNomb{

	DOMINGO		=	"DOMINGO",
	LUNES		=	"LUNES",
	MARTES		=	"MARTES",
	MIERCOLES	=	"MIERCOLES",
	JUEVES		=	"JUEVES",
	VIERNES		=	"VIERNES",
	SABADO		=	"SABADO",

} auxDiasNomb;

enum Asignts{

	NoAsig = 0,
	Asig1,
	Asig2,
	Asig3,
	Asig4,
	Asig5,
	Asig6,

} auxAsignts;

enum AsigntsN{

	NoAsignt 	= "No Asignatura",
	Asignt1 	= "Asignatura 1",
	Asignt2 	= "Asignatura 2",
	Asignt3 	= "Asignatura 3",
	Asignt4 	= "Asignatura 4",
	Asignt5 	= "Asignatura 5",
	Asignt6 	= "Asignatura 6",

} auxAsigntsN;

enum Aulas{

	Aula_A = 0,
	Aula_B,
	Aula_C,

} auxAulas;


int AulasInit[ sizeof( auxAulas ) ][ sizeof ( auxDias ) * sizeof( auxBloques ) ] = {
			{ NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, Asig1, Asig2, Asig3, NoAsig, NoAsig, Asig4, Asig5, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, Asig2, Asig3, NoAsig, NoAsig, Asig4, Asig5, NoAsig, Asig6, Asig6, NoAsig, NoAsig, Asig1, Asig2, Asig3, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig },
			{ NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, Asig3, Asig2, NoAsig, NoAsig, NoAsig, NoAsig, Asig1, Asig4, Asig5, Asig6, Asig6, NoAsig, Asig3, Asig2, NoAsig, NoAsig, NoAsig, NoAsig, Asig1, Asig4, Asig5, NoAsig, NoAsig, NoAsig, Asig3, Asig3, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig },
			{ NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, Asig2, Asig3, Asig1, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, Asig5, Asig4, NoAsig, Asig2, Asig3, NoAsig, Asig6, Asig6, NoAsig, NoAsig, NoAsig, NoAsig, Asig4, Asig5, NoAsig, Asig2, Asig3, Asig1, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig, NoAsig }
			};

/* *
struct BloqueHorarios{

	char HoraInicio[11];	//"XX:XX:XX YM"
	char HoraFin[11];		//"XX:XX:XX YM"
} ;


struct BloquesClase{

	int8_t Asignatura;
	int8_t Bloque;
	struct BloqueHorarios Hora;

};


struct DiaClase{

	struct BloquesClase bloque[ sizeof( auxBloques ) ];

};


struct Salones{

	struct DiaClase Dia[ sizeof ( auxDias ) ];

}  Aula[ sizeof( auxAulas ) ];
* */


struct salones{

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

} ;


struct salones Salones[ sizeof( auxAulas ) ];

//valores iniciales de las aulas según el problema
void initParameters() {


	for ( uint8_t i = 0; i < sizeof( auxAulas ); i++ ){

		for( uint8_t j = 0; j < sizeof ( auxDias ); j++ ){

			for(  uint8_t k = 0; k < sizeof ( sizeof( auxBloques ) ); k++  ){

				Salones[ i ].dia[ j ].bloque[ k ].Bloque = auxBloques[ k ];
				strcpy ( Salones[ i ].dia[ j ].bloque[ k ].BloqueName, auxBloqNomb[ k ] );
				strcpy ( Salones[ i ].dia[ j ].bloque[ k ].DiaName, auxDiasNomb[ k ] );
				Salones[ i ].dia[ j ].bloque[ k ].Asignatura = AulasInit[ i ][ j * sizeof( auxBloques ) + k ];
				strcpy ( Salones[ i ].dia[ j ].bloque[ k ].AsigntName , auxAsigntsN [ k ] );
				strcpy ( Salones[ i ].dia[ j ].bloque[ k ].Hora.Ini , auxHorariosIN [ k ] );
				strcpy ( Salones[ i ].dia[ j ].bloque[ k ].Hora.Fin , auxHorariosOUT [ k ] );

			}

		}

	}

}



//comits para manipular los horarios de cada aula
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








