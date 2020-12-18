
/*
 * protocoloI2C.c
 *
 *  Created on: 17 dic. 2020
 *      Author: William
 */



#include "driver/gpio.h"
#include "driver/i2c.h"


#define PinSDA 19
#define PinSCL 18
#define ClockI2C 100000


void iniciarI2C(){

	i2c_config_t configuracionI2C;
	configuracionI2C.mode = I2C_MODE_MASTER;
	configuracionI2C.sda_io_num = PinSDA;
	configuracionI2C.scl_io_num = PinSCL;
	configuracionI2C.sda_pullup_en = GPIO_PULLUP_ENABLE;
	configuracionI2C.scl_pullup_en = GPIO_PULLUP_ENABLE;
	configuracionI2C.master.clk_speed = ClockI2C;


	i2c_param_config( I2C_NUM_0, &configuracionI2C );
	i2c_driver_install( I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0 );

}



