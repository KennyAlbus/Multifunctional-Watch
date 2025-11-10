#include "stm32f10x.h"                  // Device header
#include "user_config.h"
#include "Delay.h"



uint8_t Key_Num = 0;
uint16_t press_time = 0;

static uint8_t Key_GetState(void)
{
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)			
	{
		return 1;												
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)			
	{
		return 2;												
	}
	else if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0) && (press_time >= 1000))	//return 4 和 return 3 有顺序的区别。
	{
		return 4;												
	}
	else if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)			
	{
		return 3;												
	}
	else
	{
	  return 0;
	}
}
/**
  * @brief  Function of key long press-check.
            which is called by timer2 interrupt. /timer.c
  * @param  None.
  * @retval None
  */
void Key3_Tick(void)
{
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)
	{
	  press_time++;
	}
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 1)
	{
	  press_time = 0;
	}
}
/**
  * @brief  Function of key-check.
            which is called by timer2 interrupt per 20ms. /timer.c
  * @param  None.
  * @retval None
  */
void Key_Tick(void)
{
  static uint8_t cnt = 0;
	static uint8_t prev_state,curr_state;
	cnt++;
	if(cnt >= BUTTON_DEBOUNCE_MS)
	{
	  cnt = 0;
		prev_state = curr_state;
		curr_state = Key_GetState();
		if(prev_state != 0 && curr_state == 0)  //press and unpress one time.
		{
		  Key_Num = prev_state;
		}
	}
}
/**
  * @brief  Initialization of the key on the board.
  * @param  None.
  * @retval None
  */
void Key_Init(void)
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		
	

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						
}

#if 1
/**
  * @brief  Function of getting key number,will be returned in void Key_Tick(void);
  * @param  None.
  * @retval Key_Num will be returned,when the key is pressed.
            if Key_Num = 0,no key is pressed.
  */
uint8_t Key_GetNum(void)
{
  uint8_t key_temp = 0;
	if(!Key_Num)
	{
	  return 0;
	}
	key_temp = Key_Num;
	Key_Num = 0;
	return key_temp;
}
	
#else
/**
  * 函    数：按键获取键码
  * 参    数：无
  * 返 回 值：按下按键的键码值，范围：0~2，返回0代表没有按键按下
  * 注意事项：此函数是阻塞式操作，当按键按住不放时，函数会卡住，直到按键松手
  */
uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
	
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)			//读PB1输入寄存器的状态，如果为0，则代表按键1按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 1;												//置键码为1
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0)			//读PA4输入寄存器的状态，如果为0，则代表按键2按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 3;												//置键码为2
	}
	
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)			//读PA6输入寄存器的状态，如果为0，则代表按键3按下
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 2;												//置键码为3
	}
	
	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
#endif
