/*
 * timer.h
 *
 *  Created on: 28 de jan de 2019
 *      Author: mairon
 */

#include <stdint.h>

#ifndef TIMER_H_
#define TIMER_H_

extern volatile uint32_t millis;
extern volatile float dt;
extern volatile uint32_t now;
extern volatile uint32_t lastUpdate;
extern volatile float freq;

void SysTickInt(void);
void TimerBegin();


#endif /* TIMER_H_ */
