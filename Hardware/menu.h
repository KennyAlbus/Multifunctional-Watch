#ifndef __MENU_H_
#define __MENU_H_

#include <stdint.h>

void Peripheral_Init(void);
void Menu_App_Init(void);
void Show_Clock_UI(void);
void Show_Setting_UI(void);
int First_Page_Clock(void);
int Setting_Page_Info(void);
int Enter_Menu_Option(void);
int Timer_Page_Select(void);
int Torch(void);
int MPU6050(void);
uint8_t Game_Select_Page(void);
int Dynamic_Emoji(void);
int Gradienter(void);

#endif
