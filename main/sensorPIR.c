/*
 * sensorPIR.c
 *
 *  Created on: 17 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "string.h"

//#include "ds3231.h"

#define DATA_PIR GPIO_NUM_4
#define BUZZER GPIO_NUM_22
#define PULSADOR GPIO_NUM_0


uint8_t cont_alarma = 0;
uint8_t NumMaxAlarm = 50;

struct ALARMA{

	uint8_t anio;
	uint8_t mes;
	uint8_t fecha;
	uint8_t seg;
	uint8_t min;
	uint8_t hora;
	uint8_t formato_hora;
	char dia[10];
	char ampm[2];

} registros_alarma[ NumMaxAlarm ];


extern char* leerds3231( uint8_t* seg, uint8_t* min, uint8_t* hor,
		uint8_t* dia, uint8_t* mes, uint8_t* ani, uint8_t* ampm );

extern enum horariosIN auxHorariosIN;
extern enum DiasNomb auxDiasNomb;

extern xQueueHandle ColaFecha;


extern struct formatoMsjFecha{

	char formatTime = "__:__:__ __";
	char formatDate = "__-__-20__";
	char dayName[9];
	char formatMsg[33];

};


struct formatoMsjFecha FechaFormateada[ NumMaxAlarm ];


extern void formateoMensajeFecha( uint8_t* seg, uint8_t* min, uint8_t* hor,
		uint8_t* fec, uint8_t* mes, uint8_t* ani, uint8_t* ampm, char* dia, uint8_t contador, char alerta);



void TareaPir(void* P){

	gpio_set_direction(DATA_PIR | PULSADOR, GPIO_MODE_INPUT);
	gpio_set_direction(BUZZER, GPIO_MODE_OUTPUT);

	leerds3231(

			&registros_alarma[0].anio,
			&registros_alarma[0].mes,
			&registros_alarma[0].fecha,
			&registros_alarma[0].seg,
			&registros_alarma[0].min,
			&registros_alarma[0].hora,
			&registros_alarma[0].formato_hora

			);


	for(;;){

		if(gpio_get_level(DATA_PIR)){


			strcpy( registros_alarma[cont_alarma].dia,
					leerds3231(

					&registros_alarma[cont_alarma].seg,
					&registros_alarma[cont_alarma].min,
					&registros_alarma[cont_alarma].hora,
					&registros_alarma[cont_alarma].fecha,
					&registros_alarma[cont_alarma].mes,
					&registros_alarma[cont_alarma].anio,
					&registros_alarma[cont_alarma].formato_hora

					) );



			if( registros_alarma[cont_alarma].formato_hora )	strcpy( registros_alarma[cont_alarma].ampm, "Pm" );
			else 	strcpy( registros_alarma[cont_alarma].ampm, "AM" );


			formateoMensajeFecha(

					&registros_alarma[cont_alarma].seg,
					&registros_alarma[cont_alarma].min,
					&registros_alarma[cont_alarma].hora,
					&registros_alarma[cont_alarma].fecha,
					&registros_alarma[cont_alarma].mes,
					&registros_alarma[cont_alarma].anio,
					&registros_alarma[cont_alarma].formato_hora,
					cont_alarma,
					&registros_alarma[cont_alarma].dia,
					'1'

			);

			/* *
			char formatoHora = "__:__:__ __";

			formatoHora[0] = ( registros_alarma[cont_alarma].hora / 10 ) + "0";
			formatoHora[1] = ( registros_alarma[cont_alarma].hora % 10 ) + "0";
			formatoHora[3] = ( registros_alarma[cont_alarma].min / 10 ) + "0";
			formatoHora[4] = ( registros_alarma[cont_alarma].min % 10 ) + "0";
			formatoHora[6] = ( registros_alarma[cont_alarma].seg / 10 ) + "0";
			formatoHora[7] = ( registros_alarma[cont_alarma].seg % 10 ) + "0";
			formatoHora[9] = registros_alarma[cont_alarma].formato_hora[0];
			formatoHora[10] = registros_alarma[cont_alarma].formato_hora[1];
			* */


			if ( toupper( registros_alarma[cont_alarma].dia ) == auxDiasNomb[0] ||
					toupper( registros_alarma[cont_alarma].dia ) == auxDiasNomb[ sizeof( auxDiasNomb ) ]  ){

				xQueueSend( ColaFecha, &FechaFormateada[ cont_alarma ].formatMsg, 0 / portTICK_PERIOD_MS );

				gpio_set_level(BUZZER, 1);
				vTaskDelay( 2000 / portTICK_PERIOD_MS );
				gpio_set_level(BUZZER, 0);

			}else{

				if ( strcmp( FechaFormateada[ cont_alarma ].formatTime, auxHorariosIN[0] ) < 0 ||
						strcmp( FechaFormateada[ cont_alarma ].formatTime, auxHorariosIN[ sizeof ( auxHorariosIN ) ] ) > 0  ){

					xQueueSend( ColaFecha, &FechaFormateada[ cont_alarma ].formatMsg, 0 / portTICK_PERIOD_MS );

					gpio_set_level(BUZZER, 1);
					vTaskDelay( 2000 / portTICK_PERIOD_MS );
					gpio_set_level(BUZZER, 0);

				}

			}


			cont_alarma++;
			if( cont_alarma > NumMaxAlarm ) cont_alarma = 0;

		}

		vTaskDelay( 100 / portTICK_PERIOD_MS );

	}

}


void Pulsador(void* P){

	for(;;){

		if( gpio_get_level(PULSADOR) == 0 ){

			vTaskDelay( 750 / portTICK_PERIOD_MS );


			for(int i = 0; i < cont_alarma; i++ ){

				printf("=======================ALARMA %d=======================\n\r", i);

				printf( "Fecha: %s %x-%x-20%x\n\r",
						registros_alarma[i].dia,
						registros_alarma[i].fecha,
						registros_alarma[i].mes,
						registros_alarma[i].anio
						);

				printf( "Hora: %x:%x:%x %C%C\n\r",
						registros_alarma[i].hora,
						registros_alarma[i].min,
						registros_alarma[i].seg,
						registros_alarma[i].ampm[0],
						registros_alarma[i].ampm[1]
						);

				printf("=======================================================\n\r");


				formateoMensajeFecha(

									&registros_alarma[cont_alarma].seg,
									&registros_alarma[cont_alarma].min,
									&registros_alarma[cont_alarma].hora,
									&registros_alarma[cont_alarma].fecha,
									&registros_alarma[cont_alarma].mes,
									&registros_alarma[cont_alarma].anio,
									&registros_alarma[cont_alarma].formato_hora,
									cont_alarma,
									&registros_alarma[cont_alarma].dia

							);

				xQueueSend( ColaFecha, &FechaFormateada[ cont_alarma ].formatTime, 0 / portTICK_PERIOD_MS );

			}

			cont_alarma++;

		}

		vTaskDelay( 15 / portTICK_PERIOD_MS );

	}

}

