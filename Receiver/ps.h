/************************
*
*	PS2 Button Mappings
*
************************/

#ifndef _PS2_
#define _PS2_

// Byte 4
#define BUTTONS1_BYTE	3

#define SELECT 		0
#define JOY_L 		1
#define JOY_R 		2
#define START 		3
#define UP			4
#define RIGHT		5
#define DOWN		6
#define LEFT		7
#define L2			8

// Byte 5
#define BUTTONS2_BYTE	4

#define R2			1
#define L1			2
#define R1			3
#define TRIANGLE 	4
#define CIRCLE		5
#define X			6
#define SQUARE		7

// Joysticks
#define RX_BYTE		6
#define RY_BYTE		7
#define LX_BYTE		8
#define LY_BYTE		9

uint8_t ps2_data[9];

uint8_t ps2_x_pressed();
void ps2_setup();

#endif /* _PS2_ */