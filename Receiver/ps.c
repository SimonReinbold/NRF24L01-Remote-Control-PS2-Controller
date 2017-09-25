/***************************
*
*	Routines to talk to PS2 controller
*
**************************/

#include <avr/io.h>
#include <util/delay.h>
#include "ps.h"
#include "../common/spi.h"

uint8_t ps2_x_pressed(){
	if(ps2_data[BUTTONS2_BYTE] & (1<<X)){
		return 0;
	}else{
		return 1;
	}

}

void ps2_setup(){
	uint8_t cmd_conf[5] = {0x01,0x43,0x00,0x01,0x00};
	uint8_t cmd_AnalogOn[9] = {0x01,0x44,0x00,0x01,0x03,0x00,0x00,0x00,0x00};
	uint8_t cmd_MotorMap[9] = {0x01,0x4D,0x00,0x00,0x01,0xFF,0xFF,0xFF,0xFF};
	uint8_t cmd_PressureConf[9] = {0x01,0x4F,0x00,0xFF,0xFF,0x03,0x00,0x00,0x00};
	uint8_t cmd_ExitConf[9] = {0x01,0x43,0x00,0x00,0x5A,0x5A,0x5A,0x5A,0x5A};

	spi_ps2_config();
	// Attention pin idle high
    SPI_PORT &= ~(1<<SS_PS2_PIN);
	SPI_Transmit(cmd_conf,5);
	SPI_PORT |= (1<<SS_PS2_PIN);
	
	_delay_ms(250);
	
	SPI_PORT &= ~(1<<SS_PS2_PIN);
	SPI_Transmit(cmd_AnalogOn,9);
	SPI_PORT |= (1<<SS_PS2_PIN);
	
	_delay_ms(250);
	
	SPI_PORT &= ~(1<<SS_PS2_PIN);
	SPI_Transmit(cmd_MotorMap,9);
	SPI_PORT |= (1<<SS_PS2_PIN);
	
	_delay_ms(250);
	
	SPI_PORT &= ~(1<<SS_PS2_PIN);
	SPI_Transmit(cmd_PressureConf,9);
	SPI_PORT |= (1<<SS_PS2_PIN);
	
	_delay_ms(250);
	
	SPI_PORT &= ~(1<<SS_PS2_PIN);
	SPI_Transmit(cmd_ExitConf,9);
	SPI_PORT |= (1<<SS_PS2_PIN);
}