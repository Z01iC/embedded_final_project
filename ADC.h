#ifndef __ADC_H__
#define __ADC_H__

/*These are the pins used for ADC in our project*/
#define ADC1_SE18 0b10010
#define ADC1_DM0 0b10011
#define ADC1_DP0 0b00000

void ADC_Init(void); /*Initializes ADC Module on the board*/
#endif