#include "fsl_common.h"
#include "3140_accel.h"
#include "3140_serial.h"
#include "3140_i2c.h"

#include <stdio.h>

#include "3140_concur.h"
#include "utils.h"
#include "ADC.h"

#include "LEDMat.h"
#include "MatrixCol.h"

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
char firstColVal[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00};
  char secondColVal[8] = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
  char thirdColVal[8] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  char fourthColVal[8] = {0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
  char fifthColVal[8] = {0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00};
  char sixthColVal[8] = {0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00};
  char seventhColVal[8] = {0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00};
  char eigthColVal[8] = {0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00};
  char ninthColVal[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00};
  char tenthColVal[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
  char eleventhColVal[8] = {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
  char twelvethColVal[8] = {0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00};
  char thirteenthColVal[8] = {0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00};
  char fourteenthColVal[8] = {0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00};
  char fifteenthColVal[8] = {0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00};
  char sixteenthColVal[8] = {0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x00};
    
  matCol *firstCol = createCol(firstColVal);
  firstCol = shiftColIn(secondColVal, firstCol);
	firstCol = shiftColIn(thirdColVal, firstCol);
	firstCol = shiftColIn(fourthColVal, firstCol);
	firstCol = shiftColIn(fifthColVal, firstCol);
	firstCol = shiftColIn(sixthColVal, firstCol);
	firstCol = shiftColIn(seventhColVal, firstCol);
	firstCol = shiftColIn(eigthColVal, firstCol);
	firstCol = shiftColIn(ninthColVal, firstCol);
	firstCol = shiftColIn(tenthColVal, firstCol);
	firstCol = shiftColIn(eleventhColVal, firstCol);
	firstCol = shiftColIn(twelvethColVal, firstCol);
	firstCol = shiftColIn(thirteenthColVal, firstCol);
	firstCol = shiftColIn(fourteenthColVal, firstCol);
	firstCol = shiftColIn(fifteenthColVal, firstCol);
	firstCol = shiftColIn(sixteenthColVal, firstCol);
	toBuffer(firstCol);
  updateDisplay();
	firstCol = destroyCol(firstCol);
	toBuffer(firstCol);
  updateDisplay();
	firstCol = destroyCol(firstCol);
	toBuffer(firstCol);
  updateDisplay();
	firstCol = destroyCol(firstCol);
	toBuffer(firstCol);
  updateDisplay();
	firstCol = destroyCol(firstCol);
	toBuffer(firstCol);
  updateDisplay();
	firstCol = destroyCol(firstCol);
	toBuffer(firstCol);
  updateDisplay();
	
	
	
	

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
