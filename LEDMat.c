/**
 * @file LEDMat.c
 * @author Hamilton Lee & Zoli Csaki
 * @copyright All rights reserved, 2020
 *
 * This file holds functions for LED Matri library
 * 
 * References: https://github.com/adafruit/Adafruit_LED_Backpack
 * 
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */

#include "LEDMat.h"

/* Function to initialize LED matrix*/

/*PINS USED FOR i2C:
PTE24 is SCLK
PTE25 is SDA
*/

/*8x16 Matrix 
For ADAFRUIT 1.2' LED Matrix: https://www.adafruit.com/product/2035
Driver: HT16K33
VCC: 5V 
*/
int MatrixBegin(){
	if(I2C_WriteReg1(LED_addr, Start_Osc)!=1)	{ /*0x70 is the address of device, ,0x21 turns on the oscillator*/
		return -1; //I2C has failed
	}
	/*Set default blinking to NO*/
	if(blinkRate(0)!=1)	{
		return -1;
	}
	if(I2C_WriteReg1(LED_addr, 0xA0)!=1){
		return -1;
	}
	/*Set default brightness of LED to low*/
	if(setBrightness(LOW_MINUS)!=1){
		return -1;
	}
	if(clearDisplay()!=1){
		return -1;
	}
	return 1;
	
}

/* Function to set blink rate of LED matrix*/
int blinkRate(uint8_t b) {
	if(b > 3) b=0; /*Turn off if not sure*/
	/*Set the blink rate for the LED Matrix*/
	if(I2C_WriteReg1(LED_addr, HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1)) !=1 ){
		return -1;
	}
	return 1;
}

/*Set the brightness of the LED matrix*/
int setBrightness(uint8_t b) {
  if (b > 15) b = 15;

  if(I2C_WriteReg1(LED_addr, HT16K33_CMD_BRIGHTNESS | b)!=1){
	  return -1;
  }
  return 1; 
}

/*Clears LED Matrix Display*/
int clearDisplay(void){
	for (uint8_t i=0; i<16; i++) {
		/*i represents the row address, 0x00 writes all 8 LEDs in the row to 0*/
		/*i from 0-7 are the left LED backpacks rows*/
		/*i from 8-15 are the right LED backpacks rows*/
		if(I2C_WriteReg(LED_addr, i, 0x00)!=1){
			return -1;
		}
  }
	return 1;
}

/*Turns on ALL LEDs on matrix*/
int allOn(void)	{
	/*0xFF means all LEDs in row are ON*/
	for	(uint8_t i=0; i<16; i++)	{
		if(I2C_WriteReg(LED_addr, i, 0xFF)!=1){
			return -1;
		}
	}
	return 1;
}

/*Draws the current buffer*/
int updateDisplay(void)	{
	for(uint8_t i=0; i<16; i++) {
		if(I2C_WriteReg(LED_addr, i, displayBuffer[i])!=1){
			return -1;
		}
	}
}

/*Clears the display buffer, all 0x00*/
void clearBuffer(void)	{
	for(int i = 0; i<16; i++)	{
		displayBuffer[i] = 0x00;
	}
}

/*updates the current buffer based on the input linked list of columns. 
This function first clears the buffer recursively iterates through the matCol,
and then sets the buffer according to the  elements of column*/
void toBuffer_help(matCol* matrixCol, char curr_col){
	//if the column is on the first LED matrix
	if(curr_col<=7){
		for(int i=0; i<8; i++){
			displayBuffer[i] =  displayBuffer[i] + (matrixCol->col[i] << curr_col);
		}
	}
	//if the column is on the second LED matrix
	else{
		for(int i=8; i<16;i++){
			displayBuffer[i] = displayBuffer[i] + (matrixCol->col[i] << (curr_col-8));
		}
	}

	if(curr_col==15){
		return;
	}
	else{
		toBuffer_help(matrixCol->next, curr_col+1);
	}
}

/*Writes a linked list of columns into the proper buffer format*/
void toBuffer(matCol* matrixCol){
	clearBuffer();
	toBuffer_help(matrixCol, 0);
}


