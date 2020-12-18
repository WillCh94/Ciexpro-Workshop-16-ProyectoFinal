/*
 * configuracionWIFI.c
 *
 *  Created on: 18 dic. 2020
 *      Author: William
 */



#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"

#define nombreWiFi "ZTE E10Q"		//"nombreREDwifi"
#define claveWiFi "f5e584c7b963"	//"claveREDwifi"


static esp_err_t ManejadorEventos( void *ctx, system_event_t *evento ){

	switch( evento -> event_id ){

		case SYSTEM_EVENT_STA_START:

			esp_wifi_connect();
			break;

		case SYSTEM_EVENT_STA_DISCONNECTED:

			printf("\n\nConexión perdida con el Punto de Acceso\n\n");
			vTaskDelay( 1000 / portTICK_PERIOD_MS );
			esp_wifi_connect();
			break;

		default:
			break;

	}

	return ESP_OK;

}


void iniciarWiFi(){

	tcpip_adapter_init();
	esp_event_loop_init( ManejadorEventos, NULL );
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
	esp_wifi_init( &cfg );
	esp_wifi_set_storage( WIFI_STORAGE_RAM );


	wifi_config_t WIFIconfig ={

		.sta = {
			.ssid = nombreWiFi,
			.password = claveWiFi,
		},

	};

	esp_wifi_set_mode( WIFI_MODE_STA );
	esp_wifi_set_config( WIFI_IF_STA, &WIFIconfig );
	esp_wifi_start();

}




