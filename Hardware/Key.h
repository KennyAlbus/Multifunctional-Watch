#ifndef __KEY_H
#define __KEY_H

#include <stdint.h>
#include "stm32f10x_gpio.h"


/* Configuration information in Key.c */
#define       KEY_MAX_NUM             (3)
#define       BUTTON_DEBOUNCE_MS      (20)
#define       KEY_LONG_PRESS_TIME     (1000)
#define       KEY_DOUBLE_TIME_MS      (0)

#define       KEY1_SINGLE_CLICK       (0x11)
#define       KEY2_SINGLE_CLICK       (0x12)
#define       KEY3_SINGLE_CLICK       (0x13)
#define       KEY1_DOUBLE_CLICK       (0x21)
#define       KEY2_DOUBLE_CLICK       (0x22)
#define       KEY3_DOUBLE_CLICK       (0x23)
#define       KEY1_REPEAT_HOLD        (0x41)
#define       KEY2_REPEAT_HOLD        (0x42)
#define       KEY3_REPEAT_HOLD        (0x43)
#define       KEY1_LONG_PRESS         (0x81)
#define       KEY2_LONG_PRESS         (0x82)
#define       KEY3_LONG_PRESS         (0x83)



#define       KEY_RETURN_VALUE        \
{ \
	{KEY1_SINGLE_CLICK,KEY1_DOUBLE_CLICK,KEY1_REPEAT_HOLD,KEY1_LONG_PRESS},\
	{KEY2_SINGLE_CLICK,KEY2_DOUBLE_CLICK,KEY2_REPEAT_HOLD,KEY2_LONG_PRESS},\
	{KEY3_SINGLE_CLICK,KEY3_DOUBLE_CLICK,KEY3_REPEAT_HOLD,KEY3_LONG_PRESS},\
}

typedef enum
{
  KEY_EVENT_SINGLE,
	KEY_EVENT_DOUBLE,
	KEY_EVENT_REPEAT_HOLD,
	KEY_EVENT_LONG_PRESS,
	KEY_EVENT_MAX
}key_event_t;
typedef enum
{
	KEY_RELEASE = 0,
  KEY_PRESSED,
}key_action_t;

typedef enum
{
  KEY_IS_IDLE,
	KEY_IS_PRESSED,
	KEY_IS_RELEASE,
	KEY_IS_DOUBLE_CLICK,
	KEY_IS_LONGPRESS,
}key_state_t;

typedef struct
{
	uint32_t clock;
  GPIO_TypeDef *port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
	key_state_t state;
}key_info_t;



uint8_t Key_GetNum(void);
void Key_Timer_Handler(void);
void Key_Init(void);

#endif
