/*Nathan Orloff
 * LCD.c
 * C file associated with its header file
 * creates functions for performing operations on the LCD
 * includes initializing, clearing, setting the cursor, writing to the LCD, and more
 *  */

#include "msp.h"


#define RS 1     /* P4.0 mask */
#define RW 2     /* P4.1 mask */
#define EN 4     /* P4.2 mask */

void delayMs(int n) {
    int i, j;

    for (j = 0; j < n; j++)
        for (i = 750; i > 0; i--);      /* Delay */
}

void LCD_init(void) {   /* initializes the LCD */
    P4->DIR = 0xFF;     /* make P4 pins output for data and controls */
    delayMs(30);                /* initialization sequence */
    LCD_nibble_write(0x30, 0);
    delayMs(10);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x30, 0);
    delayMs(1);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    delayMs(1);

    LCD_command(0x28);      /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);      /* move cursor right after each char */
    LCD_command(0x01);      /* clear screen, move cursor to home */
    LCD_command(0x0F);      /* turn on display, cursor blinking */
}

/* With 4-bit mode, each command or data is sent twice with upper
 * nibble first then lower nibble.
 */
void LCD_nibble_write(unsigned char data, unsigned char control) {
    data &= 0xF0;           /* clear lower nibble for control */
    control &= 0x0F;        /* clear upper nibble for data */
    P4->OUT = data | control;       /* RS = 0, R/W = 0 */
    P4->OUT = data | control | EN;  /* pulse E */
    delayMs(0);
    P4->OUT = data;                 /* clear E */
    P4->OUT = 0;
}

void LCD_command(unsigned char command) {   /* executes command given for the LCD */
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delayMs(4);         /* commands 1 and 2 need up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(unsigned char data) {     /* writes data to LCD */
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delayMs(1);
}

void Clear_LCD(){   /* clear the display */
    LCD_nibble_write(1 & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(1 << 4, 0);      /* then lower nibble */

    delayMs(4);         /* commands 1 and 2 need up to 1.64ms */

}

void Home_LCD(){    /* move the cursor to the top left of the LCD */
    LCD_nibble_write(0x80 & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(0x80 << 4, 0);      /* then lower nibble */

    delayMs(1);         /* all others 40 us */
}

void Write_char_LCD(unsigned char data){    /* write a character to the LCD */
    LCD_nibble_write(data & 0xF0, RS);    /* upper nibble first */
    LCD_nibble_write(data << 4, RS);      /* then lower nibble  */

    delayMs(1);
}

void Line_two_LCD(){        /* move the cursor to the bottom left of the LCD */
    LCD_nibble_write(0xC0 & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(0xC0 << 4, 0);      /* then lower nibble */

    delayMs(1);         /* all others 40 us */
}

void Write_string_LCD(unsigned char* letter){    /* writes a string to the LCD */
    int i = 0;
    while(letter[i]!= '\0'){                        /* while not at the end of the string */
        LCD_nibble_write(letter[i] & 0xF0, RS);    /* upper nibble first */
        LCD_nibble_write(letter[i] << 4, RS);      /* then lower nibble  */
        i++;
        delayMs(1);
    }

}




