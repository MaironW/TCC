/*
 * timer.h
 *
 *  Created on: 28 de jan de 2019
 *      Author: mairon
 */

#ifndef TIMER_H_
#define TIMER_H_

extern volatile uint32_t millis;

void SysTickInt(void);
void TimerBegin();


#endif /* TIMER_H_ */
