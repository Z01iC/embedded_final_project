#include "HeartBeat.h"
#include <math.h>

/*display Values for our LED matrix*/
matCol *displayValues = NULL;

float xg = 0x00;
float yg = 0x00;
float zg = 0x00;

/*Current Shape we want to pass into the display*/
char currentShape[8] = {};



accelData *initAccelData(void)
{
    accelData *newData = malloc(sizeof(accelData));
    newData->min = 100;
    newData->max = -100;
    newData->avg=0;
    newData->number=0;
		return newData; 
}

struct accelData *Xaccel = NULL;
struct accelData *Yaccel = NULL;
struct accelData *Zaccel = NULL;

void addAccelDataX(float newAccel){
    Xaccel->min = (newAccel < Xaccel->min ? newAccel:Xaccel->min);
		float i = Xaccel->max;
		float j = Xaccel->min;
		float k = Xaccel->avg;
    Xaccel->max = (newAccel > Xaccel->max ? newAccel:Xaccel->max);
    Xaccel->avg = (Xaccel->avg*(Xaccel->number)+newAccel)/((Xaccel->number)+1);
    Xaccel->number += 1;
}

void addAccelDataY(float newAccel){
    Yaccel->min = (newAccel < Yaccel->min ? newAccel:Yaccel->min);
    Yaccel->max = (newAccel > Yaccel->max ? newAccel:Yaccel->max);
    Yaccel->avg = (Yaccel->avg*(Yaccel->number)+newAccel)/((Yaccel->number)+1);
    Yaccel->number += 1;
}

void addAccelDataZ(float newAccel){
    Xaccel->min = (newAccel < Zaccel->min ? newAccel:Zaccel->min);
    Zaccel->max = (newAccel > Zaccel->max ? newAccel:Zaccel->max);
    Zaccel->avg = (Zaccel->avg*(Zaccel->number)+newAccel)/(Zaccel->number+1);
    Zaccel->number += 1;
}

void clearAccelData(){
    Zaccel->min = 100;
    Xaccel->max = -100;
    Xaccel->avg=0;
    Xaccel->number=0;

    Yaccel->min = 100;
    Yaccel->max = -100;
    Yaccel->avg=0;
    Yaccel->number=0;
    
    Zaccel->min = 100;
    Zaccel->max = -100;
    Zaccel->avg=0;
    Zaccel->number=0;
}

void startTimers(void)  {
//set up PIT timer but DON'T start it
  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts (for part 2) */
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

  //load a reasonable timing value for the interrupts
  PIT->CHANNEL[0].LDVAL = 0x1fff68;
  PIT->MCR = 0x0;
  PIT->CHANNEL[0].TCTRL |= 0x03;

  //clock frequency is 20.97MHz
  NVIC_EnableIRQ(PIT1_IRQn); /* enable PIT0 Interrupts (for part 2) */
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
  PIT->CHANNEL[1].LDVAL = 0x831;
  PIT->MCR = 0x0;
  PIT->CHANNEL[1].TCTRL |= 0x03;

  NVIC_SetPriority(PIT1_IRQn, 3);
  NVIC_SetPriority(PIT0_IRQn, 2);
  NVIC_SetPriority(SVCall_IRQn, 1);
}

int main (void)
{
	Xaccel = initAccelData();
	Yaccel = initAccelData();
	Zaccel = initAccelData();
	displayValues = createCol(Flatline);

    //Add 15 more flatlien frames
    for(int i = 0; i < 15; i++) {
        displayValues = shiftColIn(Flatline, displayValues);
    }
    LED_Initialize();	//Initialize Board LEDs
	ADC_Init();				// Initialize ADC Module for ADC conversion
	I2C_ReleaseBus(); // Force Restart the bus, this is done via GPIO commands
										// and not via the onboard i2c module
	I2C_Init();				// Initialize I2C bus and Uart Serial Communications
	uart_init();
	MatrixBegin();  //Initialize the LED Matrix

    // Set default flatline signal and update the display
    toBuffer(displayValues);
    //Update Display
    updateDisplay();
    //Calibrate the Accelerometer
    calibrateAccel();
    //Calibrate the Timers so the heart rate monitor starts
    startTimers();

    while(1){
        //update global data based on current accelerometer readings

    }
    

}

void copyArraytoShape(char arr[8])  {
    for(int i=0; i<8; i++){
        currentShape[i] = arr[i];
    }
}
//Sets the shape of the column that should be added to the waveform
//based on recent readings
void findShape(void){
  
    float Xresult = (fabs(Xaccel->min) > fabs(Xaccel->min)) ? Xaccel->min:Xaccel->max;
    float Yresult = (fabs(Yaccel->min) > fabs(Yaccel->min)) ? Yaccel->min:Yaccel->max;
    float Zresult = (fabs(Zaccel->min) > fabs(Zaccel->min)) ? Zaccel->min:Zaccel->max;
    
    if(Xresult>0.4){
        copyArraytoShape(POS4);
    }
    else if(Xresult>0.2){
        copyArraytoShape(POS3);
    }
    else if(Xresult>0.1){
        copyArraytoShape(POS2);
    }
    else if(Xresult>0.05){
        copyArraytoShape(POS1);
    }
    else if(Xresult>-0.05){
        copyArraytoShape(Flatline);
    }
    else if(Xresult>-0.1){
        copyArraytoShape(NEG1);
    }
    else if(Xresult>-0.2){
        copyArraytoShape(NEG2);
    }
    else{
        copyArraytoShape(NEG3);
    }
    
}

/* 
     PIT Interrupt Handler to deal with the 1ms timer to count the current system time
*/
void PIT1_IRQHandler(void)
{
	NVIC_DisableIRQ(PIT1_IRQn);
    //decide what sort of column to add based on global varaibles
	findShape(); //Update the current shape that should be passed in
    displayValues = shiftColIn(currentShape, displayValues);
    displayValues = destroyCol(displayValues);

    // Update the LED Matrix
    toBuffer(displayValues);

    //Disable IRQ when updating display
    //Update Display
    updateDisplay();

    //clears the accelerometer data
    clearAccelData();
    //Clear the interrupt flag
    PIT->CHANNEL[1].TFLG = 1;
	NVIC_EnableIRQ(PIT1_IRQn);
}

void PIT0_IRQHandler(void)
{
	NVIC_DisableIRQ(PIT0_IRQn);
    //Find ADC values for heartbeat!
    addAccelDataX(readAccelX());
    addAccelDataY(readAccelY());
    addAccelDataZ(readAccelZ());
    //Clear the interrupt flag
    PIT->CHANNEL[1].TFLG = 1;
	NVIC_EnableIRQ(PIT0_IRQn);
} //20.97MHz--> 699 clock cycles for 30 FPS
