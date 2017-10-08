#ifndef F_CPU               //Define F_CPU if not done 
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "../common/spi.h"
#include "../common/radio.h"
#include "../common/lcd-routines.h"
#include "../Receiver/ps.h"
#include "remote.h"

volatile uint8_t timercounter;
unsigned char redMode_cmd[9] = {0x01,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t ps2_data[9];

int main(void)
{
    uint8_t payload[PAYLOAD_WIDTH];     //Array for Payload
    uint8_t maincounter =0;
    uint8_t k;

    lcd_init();
    radio_hardware_init();
	spi_init();
    spi_radio_config();

    _delay_ms(50);

    EICRA = ((1<<ISC11)|(0<<ISC10)|(1<<ISC01)|(0<<ISC00));  // Set external interupt on falling edge for INT0 and INT1
    EIMSK  = ((0<<INT1)|(1<<INT0)); 
    sei();                    // turn on interrupts

    DDRD |= (1<<PD3);
    DDRC |= (1<<PC5);

    radio_conf_tx();
    uint8_t cnt = 0;
    while(1)
    {
                // Poll data form PS2 Controller
                strncpy(ps2_data,redMode_cmd,9);
                
                spi_ps2_config();
                ps2_ss_low();
                SPI_Transmit(ps2_data,sizeof(ps2_data));
                ps2_ss_high();
                //display(&ps2_data[4],1,2);
                /*if(ps2_data[4]==0xBF){
                    PORTD |= (1<<PD3);
                }else{
                    PORTD &= ~(1<<PD3);
                }*/
                PORTC |= (1<<PC5);
                if(ps2_data[0] == 0xFF && ps2_data[2]==0x5A){
                    
                    checkStatus();

                    //display(&cnt,1,1);
                    PORTD |= (1<<PD3);
                    send_radio_msg(ps2_data);
                    PORTD &= ~(1<<PD3);
                    _delay_ms(10);
                }
                PORTC &= ~(1<<PC5);
    }
}

void checkStatus(){
    uint8_t status;   
        // Read wl_module status 

        radio_ss_low();                               // Pull down chip select
        status = SPI_Put(NOP);                  // Read status register
        radio_ss_high();                               // Pull up chip select
        
        if (status & (1<<TX_DS))                            // IRQ: Package has been sent
        {
            radio_config_register(STATUS, (1<<TX_DS));  //Clear Interrupt Bit
            is_sending=0;
        }
        
        if (status & (1<<MAX_RT))                           // IRQ: Package has not been sent, send again
        {
            
            radio_config_register(STATUS, (1<<MAX_RT)); // Clear Interrupt Bit
            ce_high();                                // Start transmission
            _delay_us(10);                              
            ce_low();
            is_sending=0;
        }
        
        if (status & (1<<TX_FULL))                          //TX_FIFO Full <-- this is not an IRQ
        {
            radio_ss_low();                               // Pull down chip select
            SPI_Put(FLUSH_TX);                      // Flush TX-FIFO
            radio_ss_high();                               // Pull up chip select
            is_sending=0;
        }
}

// Disabled due to voltage problems on IRQ Pin
ISR(INT0_vect){
    uint8_t status;   
        // Read wl_module status 

        radio_ss_low();                               // Pull down chip select
        status = SPI_Put(NOP);                  // Read status register
        radio_ss_high();                               // Pull up chip select
        
        if (status & (1<<TX_DS))                            // IRQ: Package has been sent
        {
            radio_config_register(STATUS, (1<<TX_DS));  //Clear Interrupt Bit
            is_sending=0;
        }
        
        if (status & (1<<MAX_RT))                           // IRQ: Package has not been sent, send again
        {
            radio_config_register(STATUS, (1<<MAX_RT)); // Clear Interrupt Bit
            ce_high();                                // Start transmission
            _delay_us(10);                              
            ce_low();
        }
        
        if (status & (1<<TX_FULL))                          //TX_FIFO Full <-- this is not an IRQ
        {
            radio_ss_low();                               // Pull down chip select
            SPI_Put(FLUSH_TX);                      // Flush TX-FIFO
            radio_ss_high();                               // Pull up chip select
        }
}
