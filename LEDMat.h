/**
 * @file 3140_i2c.h
 * @author Hamilton Lee & Zoli Csaki
 * @copyright All rights reserved, 2020
 *
 * This file holds function prototypes for concurrency functions
 * @warning This file is offered AS IS and WITHOUT ANY WARRANTY, without
 * even the implied warranties of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.
 */
#include "3140_i2c.h"

#ifndef __LEDMat__
#define __LEDMat__

#define LED_addr 0x70 /*I2C address of our LED matrix*/
#define Start_Osc 0x21 /*Starts oscillator for LED matrix*/
int MatrixBegin(); /* Function to initialize LED matrix*/

#endif