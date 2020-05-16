#include "fsl_common.h"
#include "3140_accel.h"
#include "3140_serial.h"
#include "3140_i2c.h"

#include <stdio.h>

#include "3140_concur.h"
#include "utils.h"
#include "ADC.h"

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
	int x1 = ADC_read16b(PTB10);
	delay();
	int y1 = ADC_read16b(PTB11);
	delay();
	int z1 = ADC_read16b(PTC11);
	delay();
	int x2 = ADC_read16b(PTB10);
	delay();
	int y2 = ADC_read16b(PTB11);
	delay();
	int z2 = ADC_read16b(PTC11);
	delay();
	int x3 = ADC_read16b(PTB10);
	delay();
	int y3 = ADC_read16b(PTB11);
	delay();
	int z3 = ADC_read16b(PTC11);
	int hello = 0;
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

	uint8_t a = ACCEL_ReadWhoAmI();
	uint8_t b = ACCEL_getDefaultConfig();

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
