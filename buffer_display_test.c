#include "fsl_common.h"
#include "3140_accel.h"
#include "3140_serial.h"
#include "3140_i2c.h"

#include <stdio.h>

#include "3140_concur.h"
#include "utils.h"
#include "ADC.h"

#include "LEDMat.h"

static char string[100];



// Blink the LED just because its running currently
void p2 (void)
{
	int i;
	for (i=0; i < 10; i++) {
		delay ();
    LEDBlue_Toggle();
	}
}

//Test Analog Accelerometer
void testAccel(void){
	int x = 0;
	for(int i=0; i<1000; i++){
		x= x + ADC_read16b(PTB10);
	}
	x=x/1000;
	
	int y=0;
	for(int j=0; j<1000; j++){
		y= y + ADC_read16b(PTB11);
	}
	y=y/1000;
	
	int z = 0;
	for(int k=0; k<1000; k++){
			z=z+ADC_read16b(ADC1_DM1);
	}
	z=z/1000;
	int yeet=0;
	yeet = yeet+1;
}

int main (void)
{
	
	int clock;

	clock=SystemCoreClock;

	LED_Initialize();
	ADC_Init();// Initialize ADC Module for ADC conversion

	//LEDRed_On();	

	I2C_ReleaseBus(); // Force Restart the bus, this is done via GPIO commands
									// and not via the onboard i2c module

	I2C_Init();				// Initialize I2C bus and Uart Serial Communications
	uart_init();
	
	MatrixBegin();
	
	uint8_t test_list[9] = {0x00,0x01,0x04,0x08,0x10,0x20,0x40,0x80,0xFF};
	for(int row = 0; row<16; row++){
		for(int col = 0; col<10; col++){
			clearBuffer();
			setBuffer(test_list[col],row);
			updateDisplay();
			delay();
		}
	}
	
	
	
	

	char string[100];

	if (process_create (testAccel,100) < 0) {
		return -1;
	}

	if (process_create (p2,32) < 0) {
		return -1;
	}
		
	process_start ();

	//LEDGreen_Toggle();

	while (1) ;

	return 0;
}
