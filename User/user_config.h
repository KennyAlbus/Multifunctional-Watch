#ifndef __USER_CONFIG_H_
#define __USER_CONFIG_H_

#include "stm32f10x.h"


/* Configuration information about Led_PWM */
#define    BREATH_STEP_CONFIG         (3)
#define    BREATH_INTERVAL_CONFIG     (10)
#define    BLINK_INTERVAL_CONFIG      (50)    /*uint of @BREATH_INTERVAL_CONFIG ms*/
#define    FLASH_COUNT_CONFIG         (3)
#define    PWM_DUTY_CYCLE_MAX         (100)

typedef enum
{
  LED_MODE_OFF = 0,
	LED_MODE_TORCH,
	LED_MODE_BREATH,
	LED_MODE_BLINK,
	LED_MODE_FLASH,
	LED_MODE_MAX,
}LedMode_t;

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
