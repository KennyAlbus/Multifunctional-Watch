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



/* Configuration information in main.c */
enum
{
  FIRST_PAGE_MENU_CHOICE = 1,
	FIRST_PAGE_SETTING_CHOICE,
};

/* Firmware Version */
#define   WATCH_VERSION_NUMBER   "V0.0.6"




#endif 
