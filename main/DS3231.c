/*
 * DS3231.c
 *
 *  Created on: 17 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c.h"
#include "string.h"

#define ds3231 0x68

#define SecZ 0x00
#define MinZ 0x00
#define HorZ 0x52	//formato para las 12am
#define DiaZ 0x05	//dia viernes
#define FecZ 0x18	//fecha 18
#define MesZ 0x12	//diciembre
#define AniZ 0x20	//2020



extern xQueueHandle ColaFecha;


struct formatoMsjFecha{

	char formatTime = "__:__:__ __";
	char formatDate = "__-__-20__";
	char dayName[9];
	char formatMsg[33];

};


struct formatoMsjFecha FechaFormateada[ 0 ];


void formateoMensajeFecha( uint8_t* seg, uint8_t* min, uint8_t* hor,
		uint8_t* fec, uint8_t* mes, uint8_t* ani, uint8_t* ampm, char* dia, uint8_t contador, char alerta){


	FechaFormateada[ contador ].formatTime[ 0 ] = ( registros_alarma[cont_alarma].hora / 10 ) + "0";
	FechaFormateada[ contador ].formatTime[ 1 ] = ( registros_alarma[cont_alarma].hora % 10 ) + "0";
	FechaFormateada[ contador ].formatTime[ 3 ] = ( registros_alarma[cont_alarma].min / 10 ) + "0";
	FechaFormateada[ contador ].formatTime[ 4 ] = ( registros_alarma[cont_alarma].min % 10 ) + "0";
	FechaFormateada[ contador ].formatTime[ 6 ] = ( registros_alarma[cont_alarma].seg / 10 ) + "0";
	FechaFormateada[ contador ].formatTime[ 7 ] = ( registros_alarma[cont_alarma].seg % 10 ) + "0";
	FechaFormateada[ contador ].formatTime[ 9 ] = registros_alarma[cont_alarma].formato_hora[0];
	FechaFormateada[ contador ].formatTime[ 10 ] = registros_alarma[cont_alarma].formato_hora[1];

	strcpy(FechaFormateada[ contador ].dayName, dia);

	FechaFormateada[ contador ].formatDate[ 0 ] = ( registros_alarma[cont_alarma].dia / 10 ) + "0";
	FechaFormateada[ contador ].formatDate[ 1 ] = ( registros_alarma[cont_alarma].dia % 10 ) + "0";
	FechaFormateada[ contador ].formatDate[ 3 ] = ( registros_alarma[cont_alarma].mes / 10 ) + "0";
	FechaFormateada[ contador ].formatDate[ 4 ] = ( registros_alarma[cont_alarma].mes % 10 ) + "0";
	FechaFormateada[ contador ].formatDate[ 8 ] = ( registros_alarma[cont_alarma].anio / 10 ) + "0";
	FechaFormateada[ contador ].formatDate[ 9 ] = ( registros_alarma[cont_alarma].anio % 10 ) + "0";


	strcpy( FechaFormateada[ contador ].formatMsg, FechaFormateada[ contador ].formatDate);
	strcat( FechaFormateada[ contador ].formatMsg, " " );
	strcat( FechaFormateada[ contador ].formatMsg, FechaFormateada[ contador ].dayName );
	strcat( FechaFormateada[ contador ].formatMsg, " " );
	strcat( FechaFormateada[ contador ].formatMsg, FechaFormateada[ contador ].formatDate );


	FechaFormateada[ contador ].formatMsg[32] = alerta[0];

}




static void enviards3231(){

	i2c_cmd_handle_t CMD = i2c_cmd_link_create();
	i2c_master_start(CMD);

	i2c_master_write_byte( CMD, ( ds3231 << 1 ) | I2C_MASTER_WRITE, 1);


	i2c_master_write_byte( CMD, 0x0, 1);
	i2c_master_write_byte( CMD, SecZ, 1);
	i2c_master_write_byte( CMD, MinZ, 1);
	i2c_master_write_byte( CMD, HorZ, 1);
	i2c_master_write_byte( CMD, DiaZ, 1);
	i2c_master_write_byte( CMD, FecZ, 1);
	i2c_master_write_byte( CMD, MesZ, 1);
	i2c_master_write_byte( CMD, AniZ, 1);

	i2c_master_stop(CMD);
	i2c_master_cmd_begin( I2C_NUM_0, CMD, 1000/portTICK_PERIOD_MS );
	i2c_cmd_link_delete(CMD);

}



char* leerds3231( uint8_t* seg, uint8_t* min, uint8_t* hor,
			uint8_t* fec, uint8_t* mes, uint8_t* ani, uint8_t* ampm ){

	char* diax[] = {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"};
	uint8_t segundos, minutos, horas, fecha, dia, meses, anios;
	i2c_cmd_handle_t CMD = i2c_cmd_link_create();


	i2c_master_start(CMD);
	i2c_master_write_byte( CMD, ( ds3231 << 1 ) | I2C_MASTER_WRITE, 1 );
	i2c_master_write_byte( CMD, 0x0, 1);


	i2c_master_start(CMD);
	i2c_master_write_byte( CMD, ( ds3231 << 1 ) | I2C_MASTER_READ, 1 );
	i2c_master_read( CMD, &segundos, 1, 0 );
	i2c_master_read( CMD, &minutos, 1, 0 );
	i2c_master_read( CMD, &horas, 1, 0 );
	i2c_master_read( CMD, &dia, 1, 0 );
	i2c_master_read( CMD, &fecha, 1, 0 );
	i2c_master_read( CMD, &meses, 1, 0 );
	i2c_master_read( CMD, &anios, 1, 1 );

	i2c_master_stop(CMD);
	i2c_master_cmd_begin( I2C_NUM_0, CMD, 1000/portTICK_PERIOD_MS );
	i2c_cmd_link_delete(CMD);


	*seg = segundos;
	*min = minutos;
	*hor = 0x1F & horas;
	*fec = fecha;
	*mes = meses;
	*ani = anios;

	if( horas & 0x20 ) *ampm = 1; else *ampm = 0;

	return diax[dia - 1];

}



void tareaDS3231 (void* P){

	uint8_t seg, min, hor, fec, mes, ani, ampm;
	char diax[10], pmam[2];

	//enviards3231();

	for(;;){

		strcpy( diax, leerds3231( &seg, &min, &hor, &fec, &mes, &ani, &ampm ) );

		if ( ampm ) memcpy( pmam, "PM", 2 );
		else memcpy( pmam, "AM", 2 );


		formateoMensajeFecha( &seg, &min, &hor, &fec, &mes, &ani, &ampm, 0, diax, '0' );

		xQueueSend( ColaFecha, &FechaFormateada[ 0 ].formatMsg, 0 / portTICK_PERIOD_MS );


		printf( "Fecha: %s %x-%x-20%x\n\r", diax, fec, mes, ani );
		printf( "Hora: %x:%x:%x %C%C\n\r", hor, min, seg, pmam[0], pmam[1] );

		vTaskDelay(1000/portTICK_PERIOD_MS);

	}

}







