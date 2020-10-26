/*Nathan Orloff
 * keypad.h
 * Header file that declares all of the
 * functions of the associated C file
 * */


#include "msp.h"
#include <stdint.h>
#include <stdio.h>

#define COL1  BIT5
#define COL2  BIT6
#define COL3  BIT7
#define ROW1  BIT0
#define ROW2  BIT1
#define ROW3  BIT2
#define ROW4  BIT7

uint8_t keypad_getkey(void);
void keypad_init(void);
char int_to_char(int a);
