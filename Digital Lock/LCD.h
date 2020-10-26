/*Nathan Orloff
 * LCD.h
 * Header file that declares all of the
 * functions of the associated C file
 * */

#include "msp.h"
#define RS 1     /* P4.0 mask */
#define RW 2     /* P4.1 mask */
#define EN 4     /* P4.2 mask */

void LCD_nibble_write(unsigned char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(unsigned char data);
void LCD_init(void);
void Clear_LCD();
void Home_LCD();
void Line_two_LCD();
void Write_char_LCD(unsigned char data);
void delayMs(int n);
void Write_string_LCD(unsigned char* data);
