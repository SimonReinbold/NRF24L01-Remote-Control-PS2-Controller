/***************************************************************
** Definition of Pins, Functions and Constants 
** for readout of PS2 Controllers
***************************************************************/

#ifndef _SPI_H_
#define _SPI_H_

#define SPI_REG DDRB
#define SPI_PORT PORTB
#define SCK_PIN PB5		// 13
#define MISO_PIN PB4	// 12
#define MOSI_PIN PB3	// 11
#define SS_PS2_PIN PD4 		// 10
#define CSN PB1

void spi_init( void );
void spi_init_2();
unsigned char SPI_Put(char cData);
uint8_t SPI_Transmit(unsigned char *cmd, signed int nbytes);
void spi_ps2_config();
void spi_radio_config();
void radio_ss_low();
void radio_ss_high();
void ps2_ss_low();
void ps2_ss_high();

#endif /* _SPI_H_ */