/*Nathan Orloff
 * EE/CPE 329
 * This program runs a digital lock system. the user types in 4-digit codes on
 * a keypad and tries to unlock the lock by matching the code being used to lock
 * the device. The state of the device and the input from the user are displayed
 * on the LCD display in nibble mode*/
#include "keypad.h"
#include "LCD.h"
#include "msp.h"
#include <stdint.h>
#include <stdio.h>

int check(char a[], char b[]);
void set_code(void);



/*creates states to be used in a FSM*/
enum states{reset, locked, unlocked, confirm_old, type_new, confirm_new, locked_out};



char code[] = {'1', '1', '1', '1'}; //holds current code for digital lock, initial code is "1111"
char input[4];  //holds the user input from the keypad
char new_code[4];   //holds the new code when in process of changing the code



void main(void)
{
    int PS; //present state
    int NS = reset; //next state, initially equal to reset state

    int incorrect_count = 0;    //count of incorrect code attempts
    int end = 0;        //once in unlocked state this turns to 1 and ends the program
    int wait = 0;

    LCD_init();         //initialize the LCD
    Clear_LCD();        //Clear the Display
    Home_LCD();         //set cursor at first position of first line
    keypad_init();      // setup gpio pins for keypad

    while(end == 0){    //while not unlocked run program
        PS = NS;    //update the present state

        switch(PS){
            case reset: //reset state
                Clear_LCD();        //Clear the Display
                Home_LCD();
                NS = locked;    //reset state always goes to locked state
                break;
            case locked:    //locked state
                Write_string_LCD("Locked");
                Line_two_LCD();
                Write_string_LCD("Enter Key ");
                int in = 0;     //variable used to count user input
                uint8_t ikey;

                int star_detected = 0;  //if * is detected turns to 1
                int pound_detected = 0; //if # is detected turns to 1
                /* while statement gets the user input 4 times as long as the input is not
                 * * or # it will write the input to the display and to the input matrix
                 * does not continue if * or # are the input*/
                while(in < 4 && star_detected == 0 && pound_detected == 0){
                    ikey = keypad_getkey(); //gets user input as integer
                    char ckey = int_to_char(ikey);  //gets user integer as a char
                    if(ikey != 255 && wait == 0){ /*if a key is pressed, write value to display once*/
                        if(ckey == '*'){
                            star_detected = 1;
                        }
                        if(ckey == '#'){
                            pound_detected = 1;
                        }
                        Write_char_LCD(ckey);
                        input[in] = ckey;   //assign user input into input matrix
                        wait = 1;   //wait causes the input to only be displayed once
                        in++;
                    }else if (ikey == 255){ /*key must be released before able to write to display again*/
                        wait = 0;
                    }
                }

                if(star_detected == 0 && pound_detected == 0){
                    int correct_code = check(input, code);
                    if(correct_code == 0){NS = unlocked;}   //if code is correct next state is unlocked
                    else{
                        incorrect_count++;  //if incorrect, incorrect count increased and next state reset
                        NS = reset;
                    }
                }

                if(star_detected == 1){ //if a * was input, next state reset
                    NS = reset;
                }
                if(pound_detected == 1){    //if # was input next state confirm old
                    NS = confirm_old;
                }
                if(incorrect_count == 5){   //if 5 incorrect attempts were input next state locked out
                    NS = locked_out;
                }
                break;
            case unlocked:  //unlocked state
                Clear_LCD();    //reset display
                Home_LCD();
                Write_string_LCD("Unlocked");
                end = 1;    //will end the program
                break;
            case confirm_old:   //confirm old code state, first part of changing the code
                Clear_LCD();    //reset display
                Home_LCD();
                Write_string_LCD("Confirm Old Code");
                Line_two_LCD();
                Write_string_LCD("Enter Key ");
                in = 0;
                //uint8_t ikey;

                star_detected = 0;
                pound_detected = 0;
                /* while statement gets the user input 4 times as long as the input is not
                 * * or # it will write the input to the display and to the input matrix
                 * does not continue if * or # are the input*/
                while(in < 4 && star_detected == 0 && pound_detected == 0){
                    ikey = keypad_getkey(); //gets user input as an integer
                    char ckey = int_to_char(ikey);  //gets user input as a character
                    if(ikey != 255 && wait == 0){ /*if a key is pressed, write value to display once*/
                        if(ckey == '*'){
                            star_detected = 1;
                        }
                        if(ckey == '#'){
                            pound_detected = 1;
                        }
                        Write_char_LCD(ckey);
                        input[in] = ckey;  //assign user input into input matrix
                        wait = 1;   //causes each input to only be displayed once
                        in++;
                     }else if (ikey == 255){ /*key must be released before able to write to display again*/
                            wait = 0;
                        }
                 }

                 if(star_detected == 0 && pound_detected == 0){
                     int correct_code = check(input, code);
                     if(correct_code == 0){NS = type_new;}  //if input matches the code, next state is type new
                     else{
                         NS = reset;    //if not a match, next state is reset
                     }
                 }

                 if(star_detected == 1){    //if input is * next state is reset
                     NS = reset;
                 }
                 if(pound_detected == 1){   //if input is # next state is confirm old
                     NS = confirm_old;
                 }

                break;
            case type_new:  //type new code state, second part of changing the code
                Clear_LCD();    //reset display
                Home_LCD();
                Write_string_LCD("Enter New Code");
                Line_two_LCD();
                Write_string_LCD("Enter Key ");
                in = 0;


                star_detected = 0;
                /* while statement gets the user input 4 times as long as the input is not
                 * * it will write the input to the display and to the new_code matrix
                 * does not continue if * is the input*/
                while(in < 4 && star_detected == 0){
                    ikey = keypad_getkey(); //gets user input as integer
                    char ckey = int_to_char(ikey);  //gets user input at character
                    if(ikey != 255 && wait == 0){ /*if a key is pressed, write value to display once*/
                        if(ckey == '*'){
                            star_detected = 1;
                        }

                        if(ckey != '#'){    //does nothing if # is input, ignores #
                            Write_char_LCD(ckey);
                            new_code[in] = ckey;    //stores user input to the new_code matrix
                            wait = 1;   //causes each input to only be displayed once
                            in++;
                        }
                    }else if (ikey == 255){ /*key must be released before able to write to display again*/
                            wait = 0;
                    }
                }

                NS = confirm_new;   //next state is confirm new unless * is detected

                if(star_detected == 1){ //next state is reset if * is detected
                    NS = reset;
                }

                break;
            case confirm_new:   //confirm the new code state, final part of changing the code
                Clear_LCD();    //reset display
                Home_LCD();
                Write_string_LCD("Confirm New Code");
                Line_two_LCD();
                Write_string_LCD("Enter Key ");
                in = 0;
                /* while statement gets the user input 4 times
                 * it will write the input to the display and to the input matrix*/
                while(in < 4 && star_detected == 0 && pound_detected == 0){
                    ikey = keypad_getkey(); //get user input as an integer
                    char ckey = int_to_char(ikey);  //get user input as a character
                    if(ikey != 255 && wait == 0){ /*if a key is pressed, write value to display once*/

                        if(ckey != '*' && ckey != '#'){ //ignore * and #
                            Write_char_LCD(ckey);
                            input[in] = ckey;   //store character in input matrix
                            wait = 1;   //causes each input to only be displayed once
                            in++;
                        }
                    }else if (ikey == 255){ /*key must be released before able to write to display again*/
                            wait = 0;
                    }
                }


                /*if the input matches the new_code previously input
                 * the new_code is set as the code for the digital lock
                 * the next state is set to reset
                 * if the input does not match the new_code then the
                 * next state is set the type_new*/
                if(check(input, new_code) == 0){
                    set_code();
                    NS = reset;
                }else{
                    NS = type_new;
                }

                break;
            case locked_out:    //locked out state
                Clear_LCD();    //resets display
                Home_LCD();
                Write_string_LCD("Locked Out");
                incorrect_count = 0;    //resets the user attempts to zero
                delayMs(5000);      //locks user out for 5 seconds
                NS = reset;     //next state is the reset state
                break;
        }

    }
}

/* checks to see if two char matrices of length 4 contain
 * the same characters in the same order. If all characters
 * are the same and in the same order, the function returns 0
 * if they are unequal then the function returns 1
 **/
int check(char a[], char b[]){
    int not_equal = 0;
    int i;
    for(i = 0; i < 4; i++){
        if(a[i] != b[i]){
            not_equal = 1;
        }
    }
    return not_equal;
}

/* The char values to unlock the digital lock is set to the
 * char values the user input*/
void set_code(void){
    int i;
    for(i = 0; i < 4; i++){
        code[i] = input[i];
    }
}


















