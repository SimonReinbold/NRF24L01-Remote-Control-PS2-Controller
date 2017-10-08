/***************************************************************
**
** PS2 Controller readout functions
** 
***************************************************************/

#ifndef F_CPU				//define F_CPU if not done 
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../common/lcd-routines.h"
#include "../common/spi.h"
#include "../common/radio.h"
#include "receiver.h"
#include "ps.h"

volatile uint8_t new_data_FLG;
uint8_t payload[PAYLOAD_WIDTH];
unsigned char redMode_cmd[9] = {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

int main(){

	lcd_init();
	lcd_string("START");
	radio_hardware_init();

	EICRA = (1<<ISC01)|(0<<ISC00);	// Set external interupt on falling edge for INT0 and INT1
	EIMSK = (1<<INT0);							// Activate INT0  

    // Initialize spi module
    spi_init();
	spi_radio_config();	

	DDRD |= (1<<PD3);

    _delay_ms(50);
	sei();

	radio_conf_rx();
	new_data_FLG = 0;
	radio_config_register(STATUS,(1<<RX_DR));

   	while(1){
		
		if(new_data_FLG == 1){
			radio_get_payload(payload);
			//display(payload,PAYLOAD_WIDTH,1);
			
			if(payload[4]==0xBF){
				PORTD |= (1<<PD3);
			}else{
				PORTD &= ~(1<<PD3);
			}
			display(payload,PAYLOAD_WIDTH,2);
/*
			strncpy(ps2_data,payload,9);
			if(ps2_x_pressed()){
				PORTD |= (1<<PD3);
			}else{
				PORTD &= ~(1<<PD3);
			}*/

			new_data_FLG = 0;
			radio_config_register(STATUS,(1<<RX_DR));
		}
		
    }
}	

void checkStatus(){
	uint8_t stat;
	stat = radio_get_status();

	// Check which bit is set
	if(stat & (1<<RX_DR)){
		// New Data received
		new_data_FLG = 1;
	}
}

// Called if transmission successful, message received or transmission counter full
ISR(INT0_vect){
	// First check STATUS register
	uint8_t stat;
	stat = radio_get_status();

	// Check which bit is set
	if(stat & (1<<RX_DR)){
		// New Data received
		new_data_FLG = 1;
	}
	//if(stat & (1<<TX_DS)){
		// Data is sent
	//	is_sending = 0;
	//	radio_config_register(STATUS,(1<<TX_DS));	// Reset Interrupt Flag
	//}
	//if(stat & (1<<MAX_RT)){
		// Max retransmissions reached
	//	radio_config_register(STATUS,(1<<MAX_RT));	// Reset Interrupt Flag
	//}
}