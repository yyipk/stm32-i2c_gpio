#ifndef __I2C_GPIO_H
#define __I2C_GPIO_H

#include "platform.h"

void i2c_gpio_init(void);  		 

void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_ack_wait(void);
void i2c_ack(void);
void i2c_nack(void);
void i2c_byte_send(u8 txd);			
uint8_t i2c_byte_read(uint8_t ack);

#endif
















