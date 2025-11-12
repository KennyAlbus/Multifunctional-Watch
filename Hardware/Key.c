#include "stm32f10x.h"                  // Device header
#include "user_config.h"
#include "Delay.h"
#include "Timer.h"




/******Private Variation define******/
static key_list_t key_info[] = 
{
  {RCC_APB2Periph_GPIOB,GPIOB,GPIO_Pin_1,GPIO_Mode_IPU,KEY_RELEASE,0},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_6,GPIO_Mode_IPU,KEY_RELEASE,0},
	{RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_4,GPIO_Mode_IPU,KEY_RELEASE,0},
	//add your key here.
};

#define       KEY_MAX_NUM     (sizeof(key_info)/sizeof(key_info[0]))

/******Global Variation define******/
uint8_t Key_Num = 0;



static uint8_t Key_State_Get(void)
{
  for(uint8_t i = 0;i < KEY_MAX_NUM;i++)
	{
		if((GPIO_ReadInputDataBit(key_info[i].port,key_info[i].pin) == KEY_PRESS) \
			  && key_info[i].press_time < 1000)
		{
			return (i+0x01);
		}
		else if((GPIO_ReadInputDataBit(key_info[i].port,key_info[i].pin) == KEY_PRESS) \
  			&& key_info[i].press_time >= 1000 && key_info[i].press_time < 3000)
		{
		  return (i+0x81);
		}
		else if((GPIO_ReadInputDataBit(key_info[i].port,key_info[i].pin) == KEY_PRESS) \
  			&& key_info[i].press_time >= 5000)
		{
		  return (i+0x41);
		}
	}
	return 0;
}

/**
  * @brief  Function of key long press-check.
            which is called by timer2 interrupt. /timer.c
  * @param  None.
  * @retval None
  */
void Key3_Tick(void)
{
  for(uint8_t i = 0;i < KEY_MAX_NUM;i++)
	{
	  if(GPIO_ReadInputDataBit(key_info[i].port,key_info[i].pin) == KEY_PRESS)
		{
		  key_info[i].press_time++;
		}
		else
		{
		  key_info[i].press_time = 0;
		}
	}
}

/**
  * @brief  Function of key-check.
            which is called by timer2 interrupt per @BUTTON_DEBOUNCE_MS ms. /timer.c
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
		curr_state = Key_State_Get();
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
	GPIO_InitTypeDef GPIO_InitStructure;
  for(uint8_t i = 0;i < KEY_MAX_NUM;i++)
	{
	  RCC_APB2PeriphClockCmd(key_info[i].clock,ENABLE);
		GPIO_InitStructure.GPIO_Mode = key_info[i].mode;
	  GPIO_InitStructure.GPIO_Pin = key_info[i].pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(key_info[i].port, &GPIO_InitStructure);
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
