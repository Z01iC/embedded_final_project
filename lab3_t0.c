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


// Process that reads and then transmits a string over UART
void p1 (void)
{
	
	SRAWDATA accelDat;
	int a;
	
	for (a = 0; a < 100; a++)
	{
		ACCEL_getAccelDat(&accelDat);
		sprintf(string, "ACCEL: ( %d, %d, %d ) S: %d \n\r",accelDat.x,accelDat.y,accelDat.z,accelDat.s);
		uart_putString(string);
	}
		
}

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
	
	//uint8_t a = ACCEL_ReadWhoAmI();
	//uint8_t b = ACCEL_getDefaultConfig();

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
