/***************************************************************
**
** PS2 Controller readout functions
** 
***************************************************************/
#include <avr/io.h> 
#include <util/delay.h>
#include "spi.h"
#include "lcd-routines.h"

void spi_init(){
	// Inport
	SPI_REG &= ~(1<<MISO_PIN); //-> automatically configured according to Atmel
	
	// Outports
	SPI_REG |= (1<<SCK_PIN) | (1<<MOSI_PIN) | (1<<SS_PS2_PIN) | (1<<CSN);

    SPI_PORT |= (1<<CSN) | (1<<SS_PS2_PIN);
}

void spi_ps2_config(){

	// LSB first
	// Clock Speed ATmega328p = 16MHz, desired SCK speed 250kHz -> Prescaler = 64
	// CLOCK Leading Edge is falling
	// Sample on Trailing EdgeSPI_Put
	SPCR = (1<<SPE)|
			(0<<SPIE)|
			(1<<DORD)|
			(1<<MSTR)|
			(1<<SPR1)|(0<<SPR0)|
			(1<<CPOL)|
			(1<<CPHA);

	SPSR &= ~(1<<SPI2X);
	_delay_ms(10);
}

void spi_radio_config(){
	
	SPCR = (1<<SPE)|
			(0<<SPIE)|
			(0<<DORD)|
			(1<<MSTR)|
			(0<<SPR1)|(1<<SPR0)|
			(0<<CPOL)|
			(0<<CPHA);   
	
	SPSR |= (1<<SPI2X);
	_delay_ms(10);
}

unsigned char SPI_Put(char cData){
	/* Start transmission */
	SPDR = cData;

	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));

	_delay_us(30);
	return SPDR;
}

uint8_t SPI_Transmit(unsigned char *cmd, signed int nbytes){
	uint8_t *answ_start = cmd;
	for(signed int i=0; i<nbytes; i++){
		*cmd = SPI_Put(*cmd);
		cmd++;
	}
	return *answ_start;
}

void radio_ss_low(){
	SPI_PORT &= ~(1<<CSN);
}

void radio_ss_high(){
	SPI_PORT |= (1<<CSN);
}

void ps2_ss_low(){
	SPI_PORT &= ~(1<<SS_PS2_PIN);
}

void ps2_ss_high(){
	SPI_PORT |= (1<<SS_PS2_PIN);
}