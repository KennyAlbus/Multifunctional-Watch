#ifndef __TIMER_H
#define __TIMER_H



void Timer_Timer_Callback(void (*p_func0)(void),void (*p_func1)(void));
void StopWatch_Timer_Callback(void (*p_func)(void));
void DinoScore_Timer_Callback(void (*pfunc)(void));
void Timer_Init(void);

#endif
