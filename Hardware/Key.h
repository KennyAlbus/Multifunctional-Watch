#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>

//extern uint8_t Key_Num;
//extern uint16_t press_time;

void Key_Init(void);
uint8_t Key_GetNum(void);
void Key3_Tick(void);
void Key_Tick(void);

#endif
