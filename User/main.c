#include <stdio.h>
#include "stm32f10x.h"                  // Device header
#include "user_config.h"
#include "Delay.h"
#include "OLED.h"
#include "menu.h"
#include "Key.h"
#include "LED.h"
#include "game.h"
#include "PWM.h"
//#include "Timer.h"



/******Private Variation define******/
//static uint8_t key_num = 0;


/******Global Variation define******/
/*@*/
int page_switch_flag = 0;

int main(void)
{
	OLED_Init();
	OLED_Clear();
	Peripheral_Init();
	Menu_App_Init();

	while (1)
	{
		page_switch_flag = First_Page_Clock();
		if(page_switch_flag == FIRST_PAGE_MENU_CHOICE)
		{
		  Enter_Menu_Option();    //Menu
		}
		else if(page_switch_flag == FIRST_PAGE_SETTING_CHOICE)
		{
		  Setting_Page_Info();   //Setting
		}
//		for(uint8_t i = 0;i < 100;i++)
//		{
//		  PWM_SetCompare2(i);
//			Delay_ms(10);
//		}
//		for(uint8_t i = 0;i < 100;i++)
//		{
//		  PWM_SetCompare2(100-i);
//			Delay_ms(10);
//		}
	}
}
