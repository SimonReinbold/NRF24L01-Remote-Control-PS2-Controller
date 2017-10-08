/***************************************************************
** NRF24L01+ - Radio communication
***************************************************************/

#ifndef _RADIO_H_
#define _RADIO_H_

// Selected Configurations

#define RADIO_REG DDRC
#define RADIO_PORT PORTC
#define CE PC0		// 13

// Define Modes
#define CONF_RX ( (1<<EN_CRC) | (1<<CRCO) )
#define CONF_TX	( (1<<MASK_RX_DR) | (1<<EN_CRC) | (1<<CRCO) )


#define TX_POWERUP radio_config_register(CONFIG, CONF_TX | ( (1<<PWR_UP) | (0<<PRIM_RX) ) )
#define RX_POWERUP radio_config_register(CONFIG, CONF_RX | ( (1<<PWR_UP) | (1<<PRIM_RX) ) )

// Define Payload size
#define PAYLOAD_WIDTH	9

//Register Adresses
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define RPD			0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD		0x1C
#define FEATURE		0x1D

//Bit Mnemonics
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0
#define ENAA_P5     5
#define ENAA_P4     4
#define ENAA_P3     3
#define ENAA_P2     2
#define ENAA_P1     1
#define ENAA_P0     0
#define ERX_P5      5
#define ERX_P4      4
#define ERX_P3      3
#define ERX_P2      2
#define ERX_P1      1
#define ERX_P0      0
#define AW          0
#define ARD         4
#define ARC         0
#define PLL_LOCK    4
#define RF_DR_HIGH  3
#define RF_DR_LOW	5
#define RF_PWR      1
#define LNA_HCURR   0        
#define RX_DR       6
#define TX_DS       5
#define MAX_RT      4
#define RX_P_NO     1
#define TX_FULL     0
#define PLOS_CNT    4
#define ARC_CNT     0
#define TX_REUSE    6
#define FIFO_FULL   5
#define TX_EMPTY    4
#define RX_FULL     1
#define RX_EMPTY    0

//Command Name Mnemonics (Instructions)
#define R_REGISTER		0x00
#define W_REGISTER		0x20
#define REGISTER_MASK	0x1F
#define R_RX_PAYLOAD	0x61
#define W_TX_PAYLOAD	0xA0
#define FLUSH_TX		0xE1
#define FLUSH_RX		0xE2
#define REUSE_TX_PL		0xE3
#define NOP				0xFF

volatile uint8_t is_sending;
unsigned char rx_buffer[PAYLOAD_WIDTH];
unsigned char read_buffer[32];
static uint8_t DESTINATION_ADDRESSE[3] = {0x2A,0x2A,0x2A};

void radio_hardware_init( void );
void radio_conf_tx(void);
void radio_conf_rx();
void send_radio_msg(uint8_t *msg);
void radio_config_register(uint8_t reg, uint8_t val);
void radio_write_register(uint8_t reg, uint8_t *val, uint8_t length);
uint8_t radio_read_register(uint8_t reg);
uint8_t radio_get_status();
void radio_get_payload(uint8_t *data);
void ce_low();
void ce_high();
void radio_common_conf();
extern uint8_t wl_module_data_ready();

#endif /* _RADIO_H_ */
