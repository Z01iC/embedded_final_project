#include <MK64F12.h>
#include "ADC.h"

/*----------------------------------------------------------------------------
  Function that initializes ADC Module on K64f
 *----------------------------------------------------------------------------*/
void ADC_Init(void)	{
	SIM->SCGC5    |= (1 <<  10);  /* Enable Clock to Port B & C */ 
	SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK; /*Enable the ADC1 Clock*/
	ADC1_SC1A |= ADC_SC1_ADCH(0b11111); /*Module is Default set to OFF*/
	ADC1_CFG1 |= ADC_CFG1_MODE(0b11); /*Set Mode to 16 bit conversion single ended (diff = 0)*/
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; /*Enable the ADC1 Clock*/
}

/*----------------------------------------------------------------------------
Return the ADC value read in the input: PIN
 *----------------------------------------------------------------------------*/
unsigned int ADC_read16b(char PIN)	{
	ADC1_SC1A = ADC_SC1_ADCH(PIN) & ADC_SC1_ADCH_MASK; /*Set the pin for ADC1 to read from and start conversion*/
	while(ADC1_SC2 & ADC_SC2_ADACT_MASK); /*ADACT set to 1 means conversion is active so wait until it's over*/
	while(!(ADC1_SC1A & ADC_SC1_COCO_MASK)); /*Wait for Conversion to Finish*/
	return ADC1_RA; /*Return the Value in the register*/

}

/*----------------------------------------------------------------------------
Converts X ADC value to float in g
 *----------------------------------------------------------------------------*/
float getXg(unsigned int val)	{
	float xg = ((float)val-(float)32813)/(float)6626;
	return -1*xg;
}

/*----------------------------------------------------------------------------
Converts Y ADC value to float in g
 *----------------------------------------------------------------------------*/
float getYg(unsigned int val)	{
	float yg = ((float)val-(float)32278)/(float)6537;
	return -1*yg;
}

/*----------------------------------------------------------------------------
Converts Z ADC value to float in g
 *----------------------------------------------------------------------------*/
float getZg(unsigned int val)	{
	float zg = ((float)val-(float)34372)/(float)6809;
	return -1*zg;
}