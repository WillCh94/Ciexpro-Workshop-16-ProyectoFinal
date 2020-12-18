/*
 * protocoloMQTT.c
 *
 *  Created on: 18 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include <string.h>
//#include "ctype.h"		//tolower() y toupper()
//#include "lwip/sockets.h"
//#include "driver/gpio.h"


#define direccionMQTT "mqtt://127.0.0.1.com"		//"mqtt://DNS_DEL_BROKER.com"
#define puertoMQTT 1883
#define usuarioMQTT "usuarioBroker"
#define claveMQTT "claveBroker"

static const char *TAG = "MQTT";
extern xQueueHandle ColaSensor;
extern xQueueHandle ColaFecha;



static esp_err_t eventosMQTT( esp_mqtt_event_handle_t evento ){

	esp_mqtt_client_handle_t cliente = evento -> client;
	int msgID;

	switch( evento -> event_id ){


		case MQTT_EVENT_CONNECTED:

			ESP_LOGI( TAG, "MQTT_EVENT_CONNECTED" );
			msgID = esp_mqtt_client_subscribe( cliente, "Humedad", 0 );
			ESP_LOGI( TAG, "Successfull subscription, id = %d", msgID );
			msgID = esp_mqtt_client_subscribe( cliente, "Temperatura", 0 );
			ESP_LOGI( TAG, "Successfull subscription, id = %d", msgID );

			break;


		case MQTT_EVENT_DISCONNECTED:

			ESP_LOGI( TAG, "MQTT DESCONECTADO" );

			break;


		case MQTT_EVENT_SUBSCRIBED:

			ESP_LOGI( TAG, "SUBSCRIBED ON TOPIC ID = %d", evento -> msg_id );

			break;


		case MQTT_EVENT_UNSUBSCRIBED:

			ESP_LOGI( TAG, "SUBSCRIBED ON TOPIC ID = %d ENDED", evento -> msg_id );

			break;


		case MQTT_EVENT_PUBLISHED:

			ESP_LOGI( TAG, "PUBLISH DONE, ID = %d ENDED", evento -> msg_id );

			break;


		case MQTT_EVENT_DATA:

			ESP_LOGW( TAG, "DATA RECIBDED" );
			printf( "TOPIC = %.*s\n\r", evento -> topic_len, evento -> topic );
			printf( "DATA = %.*s\n\r", evento -> data_len, evento -> data );

			break;


		case MQTT_EVENT_ERROR:

			ESP_LOGE( TAG, "MQTT_EVENT_ERROR" );

			break;

		default:

			ESP_LOGE( TAG, "Other event id:%d", evento -> event_id );

			break;
	}

	return ESP_OK;

}



static void ManejadorMQTT( void *handler_args, esp_event_base_t evento_base, int32_t evento_id, void *evento_data){

	ESP_LOGD( TAG, "Evento activo de tipo %s, id = %d", evento_base, evento_id);
	eventosMQTT( evento_data );

}



void TareaMQTT( void *P ){


	const esp_mqtt_client_config_t mqtt_cfg = {

		.uri = direccionMQTT,
		.port = puertoMQTT,
		.username = usuarioMQTT,
		.password = claveMQTT,

	};

	esp_mqtt_client_handle_t cliente = esp_mqtt_client_init( &mqtt_cfg );
	esp_mqtt_client_register_event( cliente, ESP_EVENT_ANY_ID, ManejadorMQTT, cliente );
	esp_mqtt_client_start( cliente );

	char datosDHT11[6];
	char HR_DHT11[] = {" Humedad Relativa: 00% "};
	char T_DHT11[] = {" Temperatura: 00.0C "};

	char FechaComplt[33];
	char FechaCompltSinAlerta[33];
	char SoloFecha[] = {" La Fecha es: 00-00-2099"};
	char SoloDia[] = {" Hoy es el dia: "};
	char SoloHora[] = {" La Hora es: 00:00:00 YM "};
	char alerta[] = {"_"};


	for(;;){

		if ( xQueueReceive( ColaSensor, &datosDHT11, portMAX_DELAY ) ){


			HR_DHT11[12]= datosDHT11[0];
			HR_DHT11[13]= datosDHT11[1];
			T_DHT11[13] = datosDHT11[3];
			T_DHT11[14] = datosDHT11[4];
			T_DHT11[16] = datosDHT11[5];

		}

		if ( xQueueReceive( ColaFecha, &FechaComplt, portMAX_DELAY ) ){

			SoloFecha[13] = FechaComplt[0];
			SoloFecha[14] = FechaComplt[1];
			SoloFecha[16] = FechaComplt[3];
			SoloFecha[17] = FechaComplt[4];
			SoloFecha[21] = FechaComplt[8];
			SoloFecha[22] = FechaComplt[9];

			for( uint8_t i = 11; i < 11 + 9; i++) strcat( SoloDia, FechaComplt[ i ] );

			SoloHora[13] = FechaComplt[21];
			SoloHora[14] = FechaComplt[22];
			SoloHora[16] = FechaComplt[24];
			SoloHora[17] = FechaComplt[25];
			SoloHora[21] = FechaComplt[27];
			SoloHora[22] = FechaComplt[28];
			SoloHora[21] = FechaComplt[30];
			SoloHora[22] = FechaComplt[31];

			//se verifica si se activó la alerta
			if ( FechaComplt[32] ){

				strcat( SoloFecha, "- ALERTA" );
				strcat( SoloDia, "- ALERTA" );
				strcat( SoloHora, "- ALERTA" );

			}

		}


		/** parámetros (6) de la función esp_mqtt_client_publish:
		 * - El cliente MQTT
		 * - Nombre del tópico (cadena de caracteres)
		 * - contenido a publicar (cadena de caracteres)
		 * - tamaño de Bytes del contenido a publicar
		 * - La prioridad de la publicación QoS ( 0, 1 ó 2 )
		 * - Especificar si se quiere que el Broker retenga la publicación de modo que, si no existe dispositivo suscrito al momento de la publicación
		 * 	 	el primer dispositivo que se suscriba podrá leer el último mensaje publicado (booleando: "0" - no, "1" - sí)
		 */

		/* ------------------------------------------------
		 *
		 *	ETIQUETAS MQTT:
		 *
		 * 	PROYECTO1
		 * 	PROYECTO/AMBIENTE/
		 * 	PROYECTO/AMBIENTE/TEMPERATURA
		 * 	PROYECTO/AMBIENTE/HUMEDADRELATVIA
		 * 	PROYECTO/DATE/ALERTA
		 * 	PROYECTO/DATE/FECHA
		 * 	PROYECTO/DATE/HORA
		 * 	PROYECTO/DATE/DIA
		 * 	PROYECTO/BRAZO/
		 * 	PROYECTO/BRAZO/MOTOR1
		 * 	PROYECTO/BRAZO/MOTOR2
		 * 	PROYECTO/BRAZO/MOTOR3
		 * 	PROYECTO/BRAZO/MOTOR4
		 * 	PROYECTO/BRAZO/MOTOR5
		 * 	PROYECTO/AULAS
		 * 	PROYECTO/AULAS/DIAS/
		 * 	PROYECTO/AULAS/DIAS/BLOQUE/
		 * 	PROYECTO/AULAS/DIAS/BLOQUE/ASIGNATURA
		 *
		 ---------------------------------------------------*/



		switch( toupper( cliente.topic ) ){

		case "PROYECTO":{


			esp_mqtt_client_publish( cliente, "PROYECTO/AMBIENTE/HUMEDADRELATVIA", HR_DHT11, strlen(HR_DHT11), 0, 0 );
			esp_mqtt_client_publish( cliente, "PROYECTO/AMBIENTE/TEMPERATURA", T_DHT11, strlen(T_DHT11), 0, 0 );

			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/FECHA", SoloFecha, strlen( SoloFecha ), 0, 0 );
			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/HORA", SoloDia, strlen( SoloDia ), 0, 0 );
			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/DIA", SoloHora, strlen( SoloHora ), 0, 0 );



		}break;


		case "PROYECTO/AMBIENTE":{

			esp_mqtt_client_publish( cliente, "PROYECTO/AMBIENTE/TEMPERATURA", HR_DHT11, strlen( HR_DHT11 ), 0, 0 );
			esp_mqtt_client_publish( cliente, "PROYECTO/AMBIENTE/TEMPERATURA", T_DHT11, strlen( T_DHT11 ), 0, 0 );

		}break;


		case "PROYECTO/AMBIENTE/TEMPERATURA":{

			esp_mqtt_client_publish( cliente, "PROYECTO/AMBIENTE/TEMPERATURA", T_DHT11, strlen( T_DHT11 ), 0, 0 );

		}break;


		case "PROYECTO/AMBIENTE/HUMEDADRELATVIA":{

			esp_mqtt_client_publish( cliente, "PROYECTO/AMBIENTE/HUMEDADRELATVIA", HR_DHT11, strlen( HR_DHT11 ), 0, 0 );

		}break;


		case "PROYECTO/DATE":{

			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/FECHA", SoloFecha, strlen( SoloFecha ), 0, 0 );
			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/HORA", SoloDia, strlen( SoloDia ), 0, 0 );
			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/DIA", SoloHora, strlen( SoloHora ), 0, 0 );


		}break;


		case "PROYECTO/DATE/FECHA":{

			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/FECHA", SoloFecha, strlen( SoloFecha ), 0, 0 );

		}break;


		case "PROYECTO/DATE/DIA":{

			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/HORA", SoloDia, strlen( SoloDia ), 0, 0 );

		}break;


		case "PROYECTO/DATE/HORA":{

			esp_mqtt_client_publish( cliente, "PROYECTO/DATE/HORA", SoloDia, strlen( SoloDia ), 0, 0 );

		}break;


		case "PROYECTO/BRAZO":{



		}break;


		case "PROYECTO/BRAZO/MOTOR1":{



		}break;


		case "PROYECTO/BRAZO/MOTOR2":{



		}break;


		case "PROYECTO/BRAZO/MOTOR3":{



		}break;


		case "PROYECTO/BRAZO/MOTOR4":{



		}break;


		case "PROYECTO/BRAZO/MOTOR5":{



		}break;










		}


	}

}



