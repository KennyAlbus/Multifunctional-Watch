#ifndef __USER_CONFIG_H_
#define __USER_CONFIG_H_

#include "stm32f10x.h"


#define       KEY1_SINGLE_CLICK       (0x01)
#define       KEY2_SINGLE_CLICK       (0x02)
#define       KEY3_SINGLE_CLICK       (0x03)
#define       KEY1_DOUBLE_CLICK       (0x21)
#define       KEY2_DOUBLE_CLICK       (0x22)
#define       KEY3_DOUBLE_CLICK       (0x23)
#define       KEY1_LONG_PRESS         (0x81)
#define       KEY2_LONG_PRESS         (0x82)
#define       KEY3_LONG_PRESS         (0x83)

typedef enum
{
	KEY_PRESS = 0,
  KEY_RELEASE,
}key_state_t;

typedef struct
{
  GPIO_TypeDef *port;
	uint16_t pin;
	key_state_t state;
	uint16_t press_time;
}key_list_t;


/* Firmware Version */
#define   WATCH_VERSION_NUMBER   "V0.0.3"


/* Macro definition in Key.c */
#define   BUTTON_DEBOUNCE_MS     (15)


#endif 
