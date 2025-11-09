#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"



/***************************Show Battery UI****************************/
/** 锂电池充满电量为4.1V，视为100%，3.3V为起始电压，视为0%
		1kΩ和4kΩ分压，AD引脚检测到的电压值范围是：3.3x0.8~4.1x0.8，即 2.64~3.28.
		对应AD值范围是2.64/3.3*4095~3.28/3.3*4095,即3276~4095(3.28V看作3.3V)
		所以电池容量为4095-3276 = 819.
**/
float Vbat;
int battery_capacity;
uint16_t ad_value;

void Show_Battery_UI(void)
{
	int sum = 0;
	for(uint16_t i = 0;i < 3000;i++)
	{
		ad_value = AD_GetValue();
		sum += ad_value;
	}
	ad_value = sum/3000;
	battery_capacity = (ad_value-3276)*100/819;
	if(battery_capacity < 0)
		battery_capacity = 0;
	if(battery_capacity > 100)
		battery_capacity = 100;
	OLED_ShowNum(85,4,battery_capacity,3,OLED_6X8);
	OLED_ShowChar(103,4,'%',OLED_6X8);
	if(battery_capacity == 100)
		OLED_ShowImage(110,0,16,16,Battery_Image);
	if(battery_capacity >= 10 && battery_capacity < 100)
	{
	  OLED_ShowImage(110,0,16,16,Battery_Image);
		OLED_ClearArea((112+battery_capacity/10),5,(10-battery_capacity/10),6);
		OLED_ClearArea(85,4,6,8);
	}
	else //when battery level < 10%
	{
	  OLED_ShowImage(110,0,16,16,Battery_Image);
		OLED_ClearArea(112,5,10,6);
		OLED_ClearArea(85,4,12,8);
	}
}

void Battery_Init(void)
{
	AD_Init();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_initstructure;
	gpio_initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_initstructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	gpio_initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_initstructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

