#include "LEDMat.h"
/* Function to initialize LED matrix*/
int MatrixBegin(){
	if(I2C_WriteReg1(LED_addr, Start_Osc)!=1){ /*0x70 is the address of device, ,0x21 turns on the oscillator*/
		return -1; //I2C has failed
	}
	/*TODO: BlinkRate Stuff*/
	/*TODO: Set Brightness Stuff*/
	
	
}