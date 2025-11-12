#ifndef __USER_CONFIG_H_
#define __USER_CONFIG_H_

#include "stm32f10x.h"

/* Configuration information in Key.c */
#define   BUTTON_DEBOUNCE_MS     (15)

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
	uint32_t clock;
  GPIO_TypeDef *port;
	uint16_t pin;
	GPIOMode_TypeDef mode;
	key_state_t state;
	uint16_t press_time;
}key_list_t;

/* Configuration information in main.c */
enum
{
  FIRST_PAGE_MENU_CHOICE = 1,
	FIRST_PAGE_SETTING_CHOICE,
};

/* Firmware Version */
#define   WATCH_VERSION_NUMBER   "V0.0.5"




#endif 
