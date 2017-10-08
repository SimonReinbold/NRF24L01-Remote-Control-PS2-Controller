/***************************************************************
**
** Radio module control
** Configurations must happen before the radio enters Tx or Rx mode
** Modes are entered by pulling CE high, modes depend on PRIM_RX in CONFIG
** 
***************************************************************/
#include <avr/io.h>
#include "radio.h"
#include "spi.h"
#include <util/delay.h>
#include "lcd-routines.h"

void radio_hardware_init(){
	RADIO_REG |= (1<<CE);
    ce_low();		
}

void send_radio_msg(uint8_t *msg){
	spi_radio_config();

	// Wait for last transmission in auto acknowlegdement
	while(is_sending==1);
	ce_low();
	is_sending = 1;                        // Set to transmitter mode
    TX_POWERUP; 

    radio_ss_low();                    // Pull down chip select
    SPI_Put( FLUSH_TX );     // Write cmd to flush tx fifo
    radio_ss_high();                    // Pull up chip select
    
    radio_ss_low();                    // Pull down chip select
    SPI_Put( W_TX_PAYLOAD ); // Write cmd to write payload
    SPI_Transmit(msg,PAYLOAD_WIDTH);   // Write payload
    radio_ss_high();                    // Pull up chip select
    
    ce_high();                     // Start transmission
	_delay_us(10);						// Grünes Modul funktioniert nicht mit 10µs delay
	ce_low();
}

void ce_high(){
	RADIO_PORT |= (1<<CE);
}

void ce_low(){
	RADIO_PORT &= ~(1<<CE);	
}

void radio_conf_tx(){
	spi_radio_config();

	radio_config_register(RF_SETUP,(0x06 | 0x20));

    radio_config_register(CONFIG,CONF_TX);

	radio_config_register(SETUP_RETR,(0x20 | 0x0F));

	// Power up as TX
    TX_POWERUP;

    is_sending = 0;
}

void radio_conf_rx(){
	spi_radio_config();

	// Set pipe 0 payload width
	radio_config_register(RF_SETUP,(0x06 | 0x20));

	radio_config_register(RX_PW_P0, PAYLOAD_WIDTH);

	is_sending = 0;
	RX_POWERUP;
	ce_high();
}

void radio_config_register(uint8_t reg, uint8_t val){
	spi_radio_config();

	uint8_t command[2] = {W_REGISTER | (REGISTER_MASK & reg) ,val};
	radio_ss_low();
	SPI_Transmit(command, 2);
	radio_ss_high();
}

void radio_write_register(uint8_t reg, uint8_t *val, uint8_t length){
	spi_radio_config();

	radio_ss_low();
	SPI_Put(W_REGISTER | (REGISTER_MASK & reg));
	SPI_Transmit(val, length);
	radio_ss_high();
}

uint8_t radio_read_register(uint8_t reg){
	spi_radio_config();

	uint8_t answer;
	radio_ss_low();
	SPI_Put(R_REGISTER | (REGISTER_MASK & reg));
	answer = SPI_Transmit(0x00, 1);
	radio_ss_high();
	return answer;
}

void radio_get_payload(uint8_t *data){
	spi_radio_config();
	
	radio_ss_low();
	SPI_Put(R_RX_PAYLOAD);
	SPI_Transmit(data, PAYLOAD_WIDTH);
	radio_ss_high();
}

uint8_t radio_get_status(){
	uint8_t stat;
	stat = SPI_Put(NOP);
	return stat;
}