#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "freertos/queue.h"
#include "nvs_flash.h"

//#include "PruebasNoFinal.h"
#include "configuracionWIFI.h"
#include "configuracionI2C.h"
#include "comunicacionMQTT.h"
#include "DS3231.h"
#include "sensorDHT11.h"
#include "sensorPIR.h"


#define Pila 				1024
#define Punteros 			1
#define BytesColaSensor 	6		//00-000
#define BytesComits		 	100
#define BytesFecha		 	33		//DD-MM-20AA_miercoles_XX:XX:XX_YMA		_ se reemplaza por espacios. el ultimo Bytes es de Alerta
#define Bytes5GDM		 	10		// A SRSRSRSRSR     straight/Reverse


xQueueHandle ColaSensor;
xQueueHandle ColaComit;
xQueueHandle ColaFecha;
xQueueHandle ColaRobot;




void app_main(void){

	ColaSensor = xQueueCreate( Punteros, BytesColaSensor );
	ColaComit = xQueueCreate( Punteros, BytesComits );
	ColaFecha = xQueueCreate( Punteros, BytesFecha );
	ColaRobot = xQueueCreate( Punteros, Bytes5GDM );

	nvs_flash_init();
	iniciarWiFi();
	initParameters();
	enviards3231();
	void initDGMGPIO();


	xTaskCreatePinnedToCore( TareaMQTT, "MQTT", Pila * 10, NULL, 2, NULL, 1 );
	xTaskCreatePinnedToCore( TareaDHT11, "DHT11", Pila * 4, NULL, 2, NULL, 0 );
	xTaskCreatePinnedToCore( TareaDHT11, "DHT11", Pila * 4, NULL, 2, NULL, 0 );

}


