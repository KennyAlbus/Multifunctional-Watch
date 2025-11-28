#include "stm32f10x.h"                  // Device header
#include "user_config.h"
#include "Delay.h"
#include "Timer.h"
#include "Key.h"
#include <stdio.h>




/******Private Variation define******/
static key_info_t key_list[] = 
{
  {RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_1,GPIO_Mode_IPU,KEY_IS_IDLE},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_6,GPIO_Mode_IPU,KEY_IS_IDLE},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_4,GPIO_Mode_IPU,KEY_IS_IDLE},
	//add your key here.
};

static const uint8_t key_value[KEY_MAX_NUM][KEY_EVENT_MAX] = KEY_RETURN_VALUE;


/******Global Variation define******/
uint8_t Key_Num[KEY_MAX_NUM] = {0};



static uint8_t Key_Action_Get(uint8_t index)
{
  if(GPIO_ReadInputDataBit(key_list[index].port,key_list[index].pin) == 0)
	{
	  return KEY_PRESSED;
	}
	return KEY_RELEASE;
}

/**
  * @brief  Function of key-check.
            which is called by timer2 interrupt per @BUTTON_DEBOUNCE_MS ms. /timer.c
  * @param  None.
  * @retval None
  */
void Key_Timer_Handler(void)
{
	static uint8_t curr_state[KEY_MAX_NUM];
	//static uint8_t prev_state;
	static uint8_t state[KEY_MAX_NUM];
	static uint16_t cnt;
	static uint32_t time,time_record;
	uint8_t i;
	time++;
	cnt++;
	if(cnt >= BUTTON_DEBOUNCE_MS)
	{
	  cnt = 0;
		//prev_state = curr_state;
		for(i = 0;i < KEY_MAX_NUM;i++)
		{
			curr_state[i] = Key_Action_Get(i);
			if(state[i] == KEY_IS_IDLE)
			{
				if(curr_state[i] == KEY_PRESSED)
				{
					time_record = time;
					state[i] = KEY_IS_PRESSED;
				}
			}
			else if(state[i] == KEY_IS_PRESSED)
			{
				if(curr_state[i] == KEY_RELEASE)
				{
					time_record = time;
					state[i] = KEY_IS_RELEASE;
				}
				else if((time - time_record) >= KEY_LONG_PRESS_TIME)
				{
					time_record = time;
					Key_Num[i] = key_value[i][KEY_EVENT_LONG_PRESS];
					state[i] = KEY_IS_LONGPRESS;
				}
			}
			else if(state[i] == KEY_IS_RELEASE)
			{
				if(curr_state[i] == KEY_PRESSED)
				{
					Key_Num[i] = key_value[i][KEY_EVENT_DOUBLE];
					state[i] = KEY_IS_DOUBLE_CLICK;
				}
				else if((time - time_record) > KEY_DOUBLE_TIME_MS)
				{
					Key_Num[i] = key_value[i][KEY_EVENT_SINGLE];
					state[i] = KEY_IS_IDLE;
				}
			}
			else if(state[i] == KEY_IS_DOUBLE_CLICK)
			{
				if(curr_state[i] == KEY_RELEASE)
				{
					state[i] = KEY_IS_IDLE;
				}
			}
			else if(state[i] == KEY_IS_LONGPRESS)
			{
				if(curr_state[i] == KEY_RELEASE)
				{
					state[i] = KEY_IS_IDLE;
				}
				else if((time-time_record) % 100 == 0)
				{
					Key_Num[i] = key_value[i][KEY_EVENT_REPEAT_HOLD];
				}
			}
		}
	}
}

#if 1
/**
  * @brief  Function of getting key number,will be returned in void Key_Tick(void);
  * @param  None.
  * @retval Key_Num will be returned when the key is pressed.
            if Key_Num = 0,no key is pressed.
  */
uint8_t Key_GetNum(void)
{
  uint8_t key_temp = 0;
	for(uint8_t i = 0;i < KEY_MAX_NUM;i++)
	{
	  if(!Key_Num[i])
		{
		  continue;
		}
		key_temp = Key_Num[i];
		Key_Num[i] = 0;
	}
	return key_temp;
}
	
#endif

/**
  * @brief  Initialization of the key on the board.
  * @param  None.
  * @retval None
  */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  for(uint8_t i = 0;i < KEY_MAX_NUM;i++)
	{
	  RCC_APB2PeriphClockCmd(key_list[i].clock,ENABLE);
		GPIO_InitStructure.GPIO_Mode = key_list[i].mode;
	  GPIO_InitStructure.GPIO_Pin = key_list[i].pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(key_list[i].port, &GPIO_InitStructure);
	}		
}


