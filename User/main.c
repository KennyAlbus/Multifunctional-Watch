#include <stdio.h>
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "menu.h"
#include "Key.h"
#include "LED.h"
#include "game.h"



/******Private Variation define******/
//static uint8_t key_num = 0;


/******Global Variation define******/
/*@*/
int page_switch_flag = 0;

int main(void)
{
	/*OLED≥ı ºªØ*/
	OLED_Init();
	OLED_Clear();
	Peripheral_Init();
	Menu_App_Init();

	while (1)
	{
		page_switch_flag = First_Page_Clock();
		if(page_switch_flag == 1)
		{
		  Enter_Menu_Option();    //Menu
		}
		else if(page_switch_flag == 2)
		{
		  Setting_Page_Info();   //Setting
		}
	}
}
