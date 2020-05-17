#include "HeartBeat.h"

/*display Values for our LED matrix*/
matCol *displayValues = NULL;

float xg = 0x00;
float yg = 0x00;
float zg = 0x00;

struct accelData *thousandReadingsX = NULL; /*The last 1000 readings of the accel ADC data*/
struct accelData *thousandReadingsY = NULL; /*The last 1000 readings of the accel ADC data*/
static int lenListX = 0; /*length of the thousandReadingsX list*/
static int lenListY = 0; /*length of the thousandReadingsY list*/
static int lenListZ = 0; /*length of the thousandReadingsZ list*/

/*adds the most recent accelerometer reading to global list of 1000 recent readings*/
void enqueue(float newVal, accelData *head, int lenList){
    lenlist += 1;
    struct accelData *newAddition = malloc(sizeof(accelData));
    newAddition->currAccel = newVal;
    newAddition->next=thousandReadings;
    
    head=newAddition;
}

/*Remove value from the accelerometer readings and maintains length 1000*/
void dequeue(accelData *thousandReadings, int lenList){
    /*If we don't need to dequeue*/
    if(lenlist <= 1000 || thousandReadings == NULL) {
        return;
    }
    /*Else free the memory and set the previous one's next to NULL*/
    accelData *curr = thousandReadings;
    accelData *prev = NULL;
    while(curr->next != NULL)   {
        prev = curr;
        curr = curr->next;
    }
    prev->next = NULL;
    free(curr);
    lenlist -= 1;
    return;
}

void startTimers(void)  {
//set up PIT timer but DON'T start it
  NVIC_EnableIRQ(PIT0_IRQn); /* enable PIT0 Interrupts (for part 2) */
  SIM->SCGC6 = SIM_SCGC6_PIT_MASK;

  //load a reasonable timing value for the interrupts
  PIT->CHANNEL[0].LDVAL = 0xc8;
  PIT->MCR = 0x0;
  PIT->CHANNEL[0].TCTRL |= (1 << 1);

  //clock frequency is 20.97MHz
  NVIC_EnableIRQ(PIT1_IRQn); /* enable PIT0 Interrupts (for part 2) */
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
  PIT->CHANNEL[1].LDVAL = 0x2bb;
  PIT->MCR = 0x0;
  PIT->CHANNEL[1].TCTRL |= 0x03;

  //Setting priorities for interrupts
  NVIC_SetPriority(PIT1_IRQn, 3);
  NVIC_SetPriority(PIT0_IRQn, 2);
  NVIC_SetPriority(SVCall_IRQn, 1);
}

int main (void)
{
	displayValues = createCol(Flatline);

    //Add 15 more flatlien frames
    for(int i = 0; i < 15; i++) {
        displayValues = shiftColIn(Flatline);
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

    //Disable IRQ when updating display
    uint32_t m;
	m = __get_PRIMASK();
	__disable_irq();
    //Update Display
    updateDisplay();
    __set_PRIMASK(m);
    //Calibrate the Accelerometer
    calibrateAccel();
    //Calibrate the Timers so the heart rate monitor starts
    startTimers();

    while(1){
        //update global data based on current accelerometer readings

    }
    

}
//Returns the shape of the column that should be added to the waveform
//based on recent readings
char[8] findShape(){
    return NULL;
}

/* 
     PIT Interrupt Handler to deal with the 1ms timer to count the current system time
*/
void PIT1_IRQHandler(void)
{
    
    //Clear the interrupt flag
    PIT->CHANNEL[1].TFLG = 1;

    //decide what sort of column to add based on global varaibles
    displayValues = shiftColIn(findShape());
    displayValues = destroyCol(displayValues);

    // Update the LED Matrix
    toBuffer(displayValues);

    //Disable IRQ when updating display
    uint32_t m;
	m = __get_PRIMASK();
	__disable_irq();
    //Update Display
    updateDisplay();
    __set_PRIMASK(m);
    
}

void PIT0_IRQHandler(void)
{
    
    //Clear the interrupt flag
    PIT->CHANNEL[0].TFLG = 1;
    //Find ADC values for heartbeat!
    enqueue(readAccelX(),thousandReadingsX, lenListX);
    enqueue(readAccelY(),thousandReadingsY, lenListY);
    enqueue(readAccelZ(),thousandReadingsZ, lenListZ);
    dequeue(thousandReadingsX,lenListX);
    dequeue(thousandReadingsX,lenListY);
    dequeue(thousandReadingsX,lenListZ);
} //20.97MHz--> 699 clock cycles for 30 FPS
