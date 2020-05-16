#ifndef __ADC_H__
#define __ADC_H__

/*These are the pins used for ADC in our project*/
#define PTB10 0b01110 /*Accelerometer X (ADC1_SE14)*/
#define PTB11 0b01111  	/*Accelerometer Y (ADC1_SE15)*/ 
#define PTC11 0b00111  /*Accelerometer Z (ADC1_SE7b)*/

void ADC_Init(void); /*Initializes ADC Module on the board*/
unsigned int ADC_read16b(char PIN); /*Read 16 bit ADC converted value*/
#endif