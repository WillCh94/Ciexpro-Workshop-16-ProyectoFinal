/*
 * BrazoRobot5GDM.c
 *
 *  Created on: 18 dic. 2020
 *      Author: William
 */




#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/queue.h"

//straight - reverse - enable 5 GDM robot GPIO
/*
#define	BaseStg			GPIO_NUM_4
#define	BaseRev			GPIO_NUM_13
#define	BaseEna			GPIO_NUM_14
#define	HombroStg		GPIO_NUM_18
#define	HombroRev		GPIO_NUM_19
#define	HombroEna		GPIO_NUM_21
#define	CodoStg			GPIO_NUM_22
#define	CodoRev			GPIO_NUM_23
#define	CodoEna			GPIO_NUM_25
#define	WristStg		GPIO_NUM_36
#define	WristRev		GPIO_NUM_27
#define	WristEna		GPIO_NUM_32
#define	ActuatorStr		GPIO_NUM_33
#define	ActuadorRev		GPIO_NUM_34
#define	ActuadorEna		GPIO_NUM_2
*/


#define	BaseStg			GPIO_NUM_13
#define	BaseRev			GPIO_NUM_14
#define	HombroStg		GPIO_NUM_18
#define	HombroRev		GPIO_NUM_21
#define	CodoStg			GPIO_NUM_23
#define	CodoRev			GPIO_NUM_25
#define	WristStg		GPIO_NUM_26
#define	WristRev		GPIO_NUM_27
#define	ActuadorStg		GPIO_NUM_32
#define	ActuadorRev		GPIO_NUM_33


extern xQueueHandle ColaRobot;


void initDGMGPIO(){

	gpio_set_direction(BaseStg | BaseRev | HombroStg | HombroRev | CodoStg |
			CodoRev | WristStg | WristRev | ActuadorStr | ActuadorRev , GPIO_MODE_INPUT);


}


void Tarea5GDM(void *P){

	uint8_t DutCBase = 0;

	uint8_t DutCHombro = 0;

	uint8_t DutCCodo = 0;

	uint8_t DutCWrist = 0;

	uint8_t DutCActu = 0;

	uint8_t DutyCycles[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	//uint8_t tp[8] = {1, 2, 3, 4, 3, 2, 1};

	char señalRobot[10];


	for (;;){

		if ( xQueueReceive( ColaRobot, &señalRobot, portMAX_DELAY ) ){

			for(uint8_t i = 0; i < sizeof ( señalRobot ); i++) if ( señalRobot[0] == '1' & DutyCycles[ i ] < 4) DutyCycles[ i ] = DutyCycles[ i ]++;
					else if ( señalRobot[0] == '-1'  & DutyCycles[ i ] > 0 ) DutyCycles[ i ] = DutyCycles[ i ]--;

			ledcWrite( BaseStg, DutCBase[ 0 ] * 64 );
			ledcWrite( BaseRev, DutCBase[ 1 ] * 64 );
			ledcWrite( HombroStg, DutCBase[ 2 ] * 64 );
			ledcWrite( HombroRev, DutCBase[ 3 ] * 64 );
			ledcWrite( CodoStg, DutCBase[ 4 ] * 64 );
			ledcWrite( CodoRev, DutCBase[ 5 ] * 64 );
			ledcWrite( WristStg, DutCBase[ 6 ] * 64 );
			ledcWrite( WristStg, DutCBase[ 7 ] * 64 );
			ledcWrite( ActuadorStg, DutCBase[ 8 ] * 64 );
			ledcWrite( ActuadorStg, DutCBase[ 9 ] * 64 );

		vTaskDelay(1000/portTICK_PERIOD_MS);
	}


	//ledcWrite( BaseStg, DutCBase );

	/* *
	for ( uint8_t i = 0; i < 255; i+=64 ){

		ledcWrite( BaseStg, i );
		ledcWrite( BaseRev, i );
		ledcWrite( HombroStg, i );
		ledcWrite( HombroRev, i );
		ledcWrite( CodoStg, i );
		ledcWrite( CodoRev, i );
		ledcWrite( WristStg, i );
		ledcWrite( WristRev, i );
		ledcWrite( ActuadorStg, i );
		ledcWrite( ActuadorRev, i );


	}
	* */


}



