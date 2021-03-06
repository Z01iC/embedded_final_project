/**
 * @file LEDMat.h
 * @author Hamilton Lee & Zoli Csaki
 * @copyright All rights reserved, 2020
 *
 * This file holds definitions for LED Matri library
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#include "3140_i2c.h"
#include "shared_structs.h"

#ifndef __LEDMat__
#define __LEDMat__

/*Buffer for the display data on the matrix*/
static uint8_t  displayBuffer[16];

#define LED_addr 0x70 /*I2C address of our LED matrix*/
#define Start_Osc 0x21 /*Starts oscillator for LED matrix*/
#define HT16K33_BLINK_CMD 0x80 /*Display Off no blinking*/
#define HT16K33_BLINK_DISPLAYON 0x01 /*OR with HT16K33_BLINK_CMD for display ON no blinking*/

/*OR with HT16K33_BLINK_CMD and HT16K33_DISPLAY_ON to set the blinking frequency*/
#define HT16K33_BLINK_OFF 0 
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3

/*Brightness values (OR with HT16K33_CMD_BRIGHTNESS to set brightness)*/
#define HT16K33_CMD_BRIGHTNESS 0xE0

#define LOW_MINUS 0x00
#define LOW 0x02
#define LOW_PLUS 0x04
#define MED_MINUS 0x05
#define MED 0x07
#define MED_PLUS 0x09
#define HIGH_MINUS 0x0B
#define HIGH 0x0C    
#define HIGH_PLUS 0x0E /*WARNING: High brightness draws a lot of current make sure you have sufficient source*/

int MatrixBegin(); /* Function to initialize LED matrix*/
int blinkRate(uint8_t b); /*Sets blinkrate for LED Matrix*/
int setBrightness(uint8_t b);/*Sets the brightness of the LED matrix, as specified by 'b'*/
int clearDisplay(void); /*Clears the entire LED display*/
int allOn(void); /*Turns on ALL LEDs on matrix*/
int updateDisplay(void); /*Draws the current buffer*/
void clearBuffer(void); /*Clears the display buffer*/
void setBuffer(uint8_t val, int index); /*Used to manually set buffer*/
void toBuffer(matCol* matrixCol); /*Writes a linked list of columns into the proper buffer format*/

#endif