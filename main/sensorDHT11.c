/*
 * sensorDHT11.c
 *
 *  Created on: 17 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/queue.h"
#include "driver/gpio.h"


#define usRetardo 1
#define numDHT11bytes 5
#define numDHT11bits 40
#define DHT11pin 4			//no se pueden usar los puertos 0, 2, 5, 12, 15  <- terminales usados por el uC cuando está siendo grabado

extern xQueueHandle ColaSensor;
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;



static esp_err_t TiempoDeEspera(gpio_num_t pin,
									uint32_t TimeOut,
									int valorEsperado,
									uint32_t *usContador){

	gpio_set_direction( pin, GPIO_MODE_INPUT );

	for( uint32_t i = 0 ; i < TimeOut ; i += usRetardo ){

		ets_delay_us(usRetardo);

		if( gpio_get_level( pin ) == valorEsperado ){

			if ( usContador ) *usContador = i;
			return ESP_OK;

		}

		return ESP_ERR_TIMEOUT;

	}

	return ESP_ERR_TIMEOUT;

}

static esp_err_t CapturarDatos( gpio_num_t pin, uint8_t datos[ numDHT11bytes ] ){

	uint32_t tiempoLow, tiempoHigh;
	gpio_set_direction( pin, GPIO_MODE_OUTPUT_OD );
	gpio_set_level( pin, 0 );
	ets_delay_us( 20000 );
	gpio_set_level( pin, 1 );


	if ( TiempoDeEspera( pin, 40, 0, NULL ) != ESP_OK ) return ESP_ERR_TIMEOUT;
	if ( TiempoDeEspera( pin, 90, 1, NULL ) != ESP_OK ) return ESP_ERR_TIMEOUT;
	if ( TiempoDeEspera( pin, 90, 0, NULL ) != ESP_OK ) return ESP_ERR_TIMEOUT;


	for ( int i = 0; i < numDHT11bits; i++ ){

		if ( TiempoDeEspera( pin, 60, 1, &tiempoLow ) != ESP_OK ) return ESP_ERR_TIMEOUT;
		if ( TiempoDeEspera( pin, 75, 0, &tiempoHigh ) != ESP_OK ) return ESP_ERR_TIMEOUT;
		uint8_t b = i / 8, m = i % 8;
		if ( !m ) datos[ b ] = 0;
		datos[ b ] |= ( tiempoHigh > tiempoLow ) << ( 7 - m );

	}

	return ESP_OK;

}


static esp_err_t LeerDHT11( gpio_num_t pin, uint8_t *HumedadInt, uint8_t *TemperaturaInt,
																	uint8_t *TemperaturaDec ){

	uint8_t datos[ numDHT11bytes ] = { 0, 0, 0, 0, 0 };
	gpio_set_direction( pin, GPIO_MODE_OUTPUT_OD );
	gpio_set_level( pin, 1 );

	portENTER_CRITICAL( &mux );
	esp_err_t resultado = CapturarDatos( pin, datos );
	portEXIT_CRITICAL( &mux );

	gpio_set_direction( pin, GPIO_MODE_OUTPUT_OD );
	gpio_set_level( pin, 1 );

	if ( resultado != ESP_OK ) return resultado;		//si la operaciòn capturarDatos no se ejecuta con éxito, se procede a mostar el error arrojado
	if ( datos[4] != ( ( datos[0] + datos[1] + datos[2] + datos[3] ) & 0xFF ) ){

		ESP_LOGE( "Sensor_DHT11", "ERROR DE VERIFICACIÓN DE CHECKSUM" );
		return ESP_ERR_INVALID_CRC;

	}

	*HumedadInt = datos[0];
	*TemperaturaInt = datos[2];
	*TemperaturaDec = datos[3];

	return ESP_OK;

}



void TareaDHT11(void *P){

	uint8_t TemperaturaInt = 0, TemperaturaDec = 0, HumedadInt = 0;
	char datosSensor[] = ("https://api.thingspeak.com/update?api_key=K1ZLQ2N4Z1Z200FI&field1=00.0&field2=00");

	for(;;){

		vTaskDelay( 3000 / portTICK_PERIOD_MS );

		if ( LeerDHT11( DHT11pin, &HumedadInt, &TemperaturaInt, &TemperaturaDec ) == ESP_OK ){

			ESP_LOGI( "sensor_DHT11", "Humedad: %d%%, Temperatura: %d.%d C", HumedadInt, TemperaturaInt, TemperaturaDec );

			datosSensor[78] = ( HumedadInt / 10 ) + "0";
			datosSensor[79] = ( HumedadInt % 10 ) + "0";
			datosSensor[66] = ( HumedadInt / 10 ) + "0";
			datosSensor[67] = ( HumedadInt % 10 ) + "0";
			datosSensor[69] = ( HumedadInt % 10 ) + "0";

			xQueueSend( ColaSensor, &datosSensor, 0 / portTICK_PERIOD_MS );

		} else {

			ESP_LOGE( "sensor_DHT11", "No fue posible leer datos del DHT11" );

		}

	}

}



