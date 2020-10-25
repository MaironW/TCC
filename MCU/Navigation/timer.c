/*
 * timer.c
 *
 *  Created on: 28 de jan de 2019
 *      Author: mairon
 */

#include <stdint.h>
#include <stdbool.h>
#include "stdlib.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_uart.h"
#include "inc/hw_gpio.h"
#include "inc/hw_timer.h"
#include "inc/hw_types.h"
#include "driverlib/systick.c"
#include "driverlib/interrupt.c"
#include "driverlib/sysctl.c"
#include "driverlib/timer.c"
#include "driverlib/udma.c"
#include "driverlib/gpio.c"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/udma.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include <string.h>
#include <stdint.h>

#include "timer.h"

volatile uint32_t millis = 0;
volatile float dt;
volatile uint32_t now;
volatile uint32_t lastUpdate;
volatile float freq;

/*
  Interrupt handler for the timer
*/
void SysTickInt(void)
{
  uint32_t status=0;

  status = TimerIntStatus(TIMER5_BASE,true);
  TimerIntClear(TIMER5_BASE,status);
  millis++;

  // update time variables
  now = millis;
  dt = (now - lastUpdate)*1e-3;
  lastUpdate = now;
  freq = 1/dt;
}

/*
  Timer setup
*/
void TimerBegin(){

  //We set the load value so the timer interrupts each 1ms
  uint32_t Period;
  Period = SysCtlClockGet()/1000; //1ms

  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER5);
  SysCtlDelay(3);
  /*
    Configure the timer as periodic, by omission it's in count down mode.
    It counts from the load value to 0 and then resets back to the load value.
  REMEMBER: You need to configure the timer before setting the load and match
  */
  TimerConfigure(TIMER5_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER5_BASE, TIMER_A, Period -1);

  TimerIntRegister(TIMER5_BASE, TIMER_A, SysTickInt);

  /*
    Enable the timeout interrupt. In count down mode it's when the timer reaches
  0 and resets back to load. In count up mode it's when the timer reaches load
  and resets back to 0.
  */
  TimerIntEnable(TIMER5_BASE, TIMER_TIMA_TIMEOUT);

  TimerEnable(TIMER5_BASE, TIMER_A);
}


