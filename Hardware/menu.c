#include "stm32f10x.h"                  // Device header
#include "user_config.h"
#include <stdlib.h>
#include <math.h>
#include "MyRTC.h"
#include "Timer.h"
#include "Delay.h"
#include "uart_drv.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"
#include "PWM.h"
#include "SetTime.h"
#include "MPU6050.h"
#include "game.h"
#include "battery.h"



/******Macro definition******/
#define    POINT_MENU_OPTION          1
#define    POINT_SETTING_OPTION       2


/******Enumeration definition******/
typedef enum
{
  ROLLBACK_FUNC = 0,
  STOPWATCH_FUNC,
  TORCH_FUNC,
	MPU6050_FUNC,
	GRADIENTER_FUNC,
	TV_FUNC,
	GAME_FUNC,
}menu_list_t;


/******Private Variation define******/
static uint8_t m_key_num = 0;


/******Private Function declare******/
//void Show_Screen_Ctrl(void);
//void Show_Fontsize_Ctrl(void);
static void Timer_Timer_handler(void);
static void Counter_Timer_Handler(void);
static void StopWatch_Timer_Handler(void);
static void LedMode_Switch_Timer_Handler(void);
static void Menu_To_Function(void);


/******Global Variation define******/
int csflag = 1;   /*cursor setting flag*/
int setflag = 0;  /*cursor setting flag*/


void Peripheral_Init(void)
{
	//uart_drv_init(115200);
  MyRTC_Init();
	Timer_Init();
	Key_Init();
	LED_Init();
	PWM_Init();
	MPU6050_Init();
	Battery_Init();
}
void Menu_App_Init(void)
{
  Timer_Timer_Callback(Timer_Timer_handler,Counter_Timer_Handler,StopWatch_Timer_Handler);
	Led_Timer_Callback(LedMode_Switch_Timer_Handler);
	Game_App_Init();
}



/***************************First Page UI****************************/
LedMode_t ledmode_flag = LED_MODE_OFF;

void Show_Clock_UI(void)
{
	Show_Battery_UI();
	MyRTC_ReadTime();
  OLED_Printf(0,0,OLED_6X8,"%d-%d-%d",MyRTC_Time[0],MyRTC_Time[1],MyRTC_Time[2]);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",MyRTC_Time[3],MyRTC_Time[4],MyRTC_Time[5]);
	OLED_ShowString(0,48,"菜单",OLED_8X16);
	OLED_ShowString(96,48,"设置",OLED_8X16);
}


int First_Page_Clock(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)     //last choice
		{
		  csflag--;
			if(csflag <= 0)
				csflag = 2;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)  //next choice
		{
		  csflag++;
			if(csflag >= 3)
				csflag = 1;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)   //confirm
		{
			OLED_Clear();
		  OLED_Update();
			return csflag;
		}
//		else if(m_key_num == KEY3_LONG_PRESS)
//		{
//		  GPIO_SetBits(GPIOB,GPIO_Pin_12);
//			GPIO_ResetBits(GPIOB,GPIO_Pin_13);
//		}
		else if(m_key_num == KEY3_LONG_PRESS)
		{
		  ledmode_flag++;
			if(ledmode_flag >= LED_MODE_MAX)
				ledmode_flag = LED_MODE_OFF;
		}
		
		switch(csflag)
		{
		  case POINT_MENU_OPTION:
			{
				Show_Clock_UI();
				OLED_ReverseArea(0,48,32,16);
				OLED_Update();
			  break;
			}
			case POINT_SETTING_OPTION:
			{
				Show_Clock_UI();
				OLED_ReverseArea(96,48,32,16);
				OLED_Update();
			  break;
			}
			default:
				break;
		}
	}
}

/***************************Setting Page UI****************************/
//static uint32_t App_Version_Get(void);
static void Show_Setting_UI(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_ShowImage(112,0,16,16,Home_Image);
	OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
	OLED_ShowString(0,32,"屏幕亮度",OLED_8X16);
	OLED_ShowString(0,48,"字体大小",OLED_8X16);
}

static void Show_Setting_UI2(void)
{
  OLED_ShowString(0,0,"固件版本号",OLED_8X16);
}

//static void Show_Screen_Ctrl(void)
//{
//	OLED_ShowImage(0,0,16,16,Return_Image);
//	OLED_ShowImage(112,0,16,16,Home_Image);
//  OLED_ShowString(0,16,"Brightness Control",OLED_6X8);
//	OLED_ShowImage(0,32,128,16,Ctrl_Image);
//}

int Setting_Page_Info(void)
{
  while(1)
	{
		uint8_t setflag_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  setflag--;
			if(setflag < 0)
				setflag = 5;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  setflag++;
			if(setflag > 5)
				setflag = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			setflag_temp = setflag;
		}
		else
		{
		  ;
		}
		if((setflag_temp == 0)||(setflag_temp == 5))
				return 0;
		else if(setflag_temp == 1)
		{
			Time_Setting_Page();
		}
		else if(setflag_temp == 2)
		{
			//Show_Screen_Ctrl();
		}
		else if(setflag_temp == 3)
		{
			//Show_Fontsize_Ctrl();
		}
		else if(setflag_temp == 4)
		{
			OLED_Clear();
			OLED_ShowString(40,24,WATCH_VERSION_NUMBER,OLED_8X16);
			OLED_Update();
			while(Key_GetNum() != 3);
		}
		switch(setflag)
		{
		  case 0:
			{
			  Show_Setting_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			}
			case 1:
			{
				Show_Setting_UI();
				OLED_ReverseArea(0,16,96,16);
				OLED_Update();
			  break;
			}
			case 2:
			{
			  Show_Setting_UI();
				OLED_ReverseArea(0,32,64,16);
				OLED_Update();
				break;
			}
			case 3:
			{
				OLED_Clear();
				Show_Setting_UI();
				OLED_ReverseArea(0,48,64,16);
				OLED_Update();
			  break;
			}
			case 4:
			{
				OLED_Clear();
				Show_Setting_UI2();
				OLED_ReverseArea(0,0,80,16);
				OLED_Update();
			  break;
			}
			case 5:
			{
				OLED_Clear();
				Show_Setting_UI();
				OLED_ReverseArea(112,0,16,16);
				OLED_Update();
			  break;
			}
			default:
				break;
		}
	}
}

/***************************StopWatch Function UI****************************/
int hour,min,sec;
int milisec;
uint8_t timer_begin_flag;
/** 
  * @brief  Flag which define whether stopwatch begin or not.
            stopwatch_begin_flag:1  -- begin counting.
            stopwatch_begin_flag:0  -- stop counting.
  */
uint8_t stopwatch_begin_flag = 0;

/** 
  * @brief  Flag which define whether stopwatch clear or not.
            stopwatch_clear_flag:1  -- ready to clear.
            stopwatch_clear_flag:0  -- cleared.
  */
uint8_t stopwatch_clear_flag = 0;

/** 
  * @brief  Flag which define whether stopwatch begin or not.
            stopwatch_begin_flag:1  -- begin counting.
            stopwatch_begin_flag:0  -- stop counting.
  */
uint8_t counter_begin_flag = 0;

int timer_index = 0;
int sw_index = 0;
int clock_index = 0;

typedef enum
{
  TIMER_COUNT_STOP,
	TIMER_COUNT_START,
	TIMER_COUNT_CLEAR
}stopwatch_flag_t;

typedef enum
{
  RETURN_PREV_MENU = 0,
	COUNT_BEGIN,
	COUNT_STOP,
	COUNT_CLEAR,
	RETURN_HOME_PAGE,
}stopwatch_list_t;

enum
{
  TIMER_RETURN_CHOICE,
  TIMER_TIMER_CHOICE,
	TIMER_STOPWATCH_CHOICE,
	TIMER_COUNTER_CHOICE,
	TIMER_ALARMCLOCK_CHOICE,
	TIMER_RANDNUM_CHOICE,
};

static void Show_Timer_Choice(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_ShowString(0,16,"计时器",OLED_8X16);
	OLED_ShowString(0,32,"秒表",OLED_8X16);
	OLED_ShowString(0,48,"定时器",OLED_8X16);
}

static void Show_Timer_Choice_P2(void)
{
  OLED_ShowString(0,0,"闹钟",OLED_8X16);
	OLED_ShowString(0,16,"随机数生成",OLED_8X16);
}

static void Show_Timer_UI(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_ShowImage(112,0,16,16,Home_Image);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",hour,min,sec);
	OLED_ShowString(0,44,"开始",OLED_8X16);
	OLED_ShowString(48,44,"停止",OLED_8X16);
	OLED_ShowString(96,44,"清除",OLED_8X16);
}

static void Show_StopWatch_UI(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_ShowImage(112,0,16,16,Home_Image);
	OLED_Printf(16,16,OLED_12X24,"%02d:%02d:%02d",min,sec,milisec);
	OLED_ShowString(28,48,"开始/暂停",OLED_8X16);
}

static void Show_Counter_UI(void)//设置定时时间
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_Printf(20,24,OLED_8X16,"%02d:%02d:%02d:%02d",hour,min,sec,milisec);
	OLED_ShowString(28,48,"开始/暂停",OLED_8X16);
}

static void Timer_Timer_handler(void)
{
	if(TIMER_COUNT_START == timer_begin_flag)
	{
		static uint16_t count = 0;
		count++;
		if(count >= 1000)
		{
			count = 0;
			sec++;
			if(sec > 59)
			{
				sec = 0;
				min++;
				if(min > 59)
				{
					min = 0;
					hour++;
					if(hour > 99)
					{
						hour = 0;
						min = 0;
						sec = 0;
					}
				}
			}
		}
	}
	else if(TIMER_COUNT_CLEAR == timer_begin_flag)
	{
		timer_begin_flag = TIMER_COUNT_STOP;
	  hour = 0;
		min = 0;
		sec = 0;
	}
}

static void StopWatch_Timer_Handler(void)
{
  static uint16_t sw_cnt;
	sw_cnt++;
	if(stopwatch_begin_flag == 1)
	{
		if(stopwatch_clear_flag == 1)
		{
			stopwatch_clear_flag = 0;
		  min = 0;
			sec = 0;
			milisec = 0;
		}
		else
		{
		  if(sw_cnt >= 10)
			{
				sw_cnt = 0;
				milisec++;
				if(milisec >= 100)
				{
					milisec = 0;
					sec++;
					if(sec >= 60)
					{
						sec = 0;
						min++;
						if(min >= 60)
						{
							min = 0;
							sec = 0;
							milisec = 0;
						}
					}
				}
			}
		}
	}
	else
	{
	  stopwatch_clear_flag = 1;
	}
}

static void Counter_Timer_Handler(void)
{
  static uint16_t t_cnt;
	t_cnt++;
	if(counter_begin_flag == 1)
	{
		if(t_cnt > 10)
		{
		  t_cnt = 0;
			milisec--;
			if(milisec < 0)
			{
			  milisec = 99;
				sec--;
				if(sec < 0)
				{
				  sec = 59;
					min--;
					if(min < 0)
					{
					  min = 59;
						hour--;
						if(hour < 0)
						{
							hour = 0;
							min = 0;
							sec = 0;
							milisec = 0;
						  counter_begin_flag = 0;
							OLED_Clear();
							OLED_ShowString(10,20,"Time's up",OLED_12X24);
							OLED_Update();
							Delay_s(3);
							OLED_Clear();
							OLED_Update();
						}
					}
				}
			}
		}
	}
}

/**
  * @brief  Count time from zero second,such as 1s,2s,3s...59s.
  * @param  None.
  * @retval Return value is used to go back to the previous menu level.
  */
static int Counter_Timer(void)
{
  while(1)
	{
		uint8_t timer_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  timer_index--;
			if(timer_index < 0)
				timer_index = 4;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  timer_index++;
			if(timer_index > 4)
				timer_index = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			timer_index_temp = timer_index;
		}
		
		//confirm operation
		if(RETURN_PREV_MENU == timer_index_temp)
		{
		  hour = 0;
			min = 0;
			sec = 0;
			return 0;
		}
		if(RETURN_HOME_PAGE == timer_index_temp)
		{
		  First_Page_Clock();
			return 0;
		}
		if(COUNT_BEGIN == timer_index_temp)
		{
		  timer_begin_flag = TIMER_COUNT_START;
		}
		if(COUNT_STOP == timer_index_temp)
		{
		  timer_begin_flag = TIMER_COUNT_STOP;
		}
		if(COUNT_CLEAR == timer_index_temp)
		{
		  timer_begin_flag = TIMER_COUNT_CLEAR;
		}
		
		switch(timer_index)
		{
		  case RETURN_PREV_MENU:
			{
				Show_Timer_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
			  break;
			}
			case COUNT_BEGIN:
			{
				Show_Timer_UI();
				OLED_ReverseArea(0,44,32,16);
				OLED_Update();
			  break;
			}
			case COUNT_STOP:
			{
				Show_Timer_UI();
				OLED_ReverseArea(48,44,32,16);
				OLED_Update();
			  break;
			}
			case COUNT_CLEAR:
			{
				Show_Timer_UI();
				OLED_ReverseArea(96,44,32,16);
				OLED_Update();
			  break;
			}
			case RETURN_HOME_PAGE:
			{
			  Show_Timer_UI();
				OLED_ReverseArea(112,0,16,16);
				OLED_Update();
			  break;
			}
			default:
				break;
		}
		
	}
}

static int StopWatch(void)
{
  while(1)
	{
		uint8_t sw_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  sw_index--;
			if(sw_index < 0)
				sw_index = 2;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  sw_index++;
			if(sw_index > 2)
				sw_index = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			sw_index_temp = sw_index;
		}
		if(sw_index_temp == 0)
		{
		  min = 0;
			sec = 0;
			milisec = 0;
			return 0;
		}
//		if(sw_index_temp == 2)
//		{
//		  First_Page_Clock();
//			return 0;
//		}
		if(sw_index_temp == 1)
		{
		  if(!stopwatch_begin_flag)
				stopwatch_begin_flag = 1;
			else
				stopwatch_begin_flag = 0;
		}
		switch(sw_index)
		{
		  case 0:
			{
				Show_StopWatch_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
			  break;
			}
			case 1:
			{
				Show_StopWatch_UI();
				OLED_ReverseArea(28,48,72,16);
				OLED_Update();
			  break;
			}
			case 2:
			{
				Show_StopWatch_UI();
				OLED_ReverseArea(112,0,16,16);
				OLED_Update();
			  break;
			}
			default:
				break;
		}
	}
}

int cm_index = 0;

static int Counter_Time_Set(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  if(cm_index == 1)
			{
				hour--;
				if(hour < 0)
					hour = 99;
			}
			else if(cm_index == 2)
			{
			  min--;
				if(min < 0)
					min = 99;
			}
			else if(cm_index == 3)
			{
				sec--;
				if(sec < 0)
					sec = 99;
			}
			else if(cm_index == 4)
			{
				milisec--;
				if(milisec < 0)
					milisec = 99;
			}
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  if(cm_index == 1)
			{
				hour++;
				if(hour > 99)
					hour = 0;
			}
			else if(cm_index == 2)
			{
			  min++;
				if(min > 99)
					min = 0;
			}
			else if(cm_index == 3)
			{
				sec++;
				if(sec > 99)
					sec = 0;
			}
			else if(cm_index == 4)
			{
				milisec++;
				if(milisec > 99)
					milisec = 0;
			}
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  return 0;
		}
		switch(cm_index)
		{
		  case 0:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			}
			case 1:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(20,24,16,16);
				OLED_Update();
				break;
			}
			case 2:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(44,24,16,16);
				OLED_Update();
				break;
			}
			case 3:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(68,24,16,16);
				OLED_Update();
				break;
			}
			case 4:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(92,24,16,16);
				OLED_Update();
				break;
			}
			case 5:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(28,48,72,16);
				OLED_Update();
				break;
			}
			default:
				break;
		}
	}
}

static int Counter_Machine(void)
{
  while(1)
	{
		uint8_t cm_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  cm_index--;
			if(cm_index < 0)
				cm_index = 5;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  cm_index++;
			if(cm_index > 5)
				cm_index = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			cm_index_temp = cm_index;
		}
		//confirm operation
		if(cm_index_temp == 0)
		{
		  hour = 0;
			min = 0;
			sec = 0;
			milisec = 0;
			return 0;
		}
		if(cm_index_temp == 1 || cm_index_temp == 2 || cm_index_temp == 3 ||
			 cm_index_temp == 4)
		{
		  Counter_Time_Set();
		}
		if(cm_index_temp == 5)
		{
		  //begin or stop.
			if(!counter_begin_flag)
			  counter_begin_flag = 1;
			else
				counter_begin_flag = 0;
		}
		switch(cm_index)
		{
		  case 0:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			}
			case 1:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(20,24,16,16);
				OLED_Update();
				break;
			}
			case 2:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(44,24,16,16);
				OLED_Update();
				break;
			}
			case 3:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(68,24,16,16);
				OLED_Update();
				break;
			}
			case 4:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(92,24,16,16);
				OLED_Update();
				break;
			}
			case 5:
			{
			  Show_Counter_UI();
				OLED_ReverseArea(28,48,72,16);
				OLED_Update();
				break;
			}
			default:
				break;
		}
	}
}

uint8_t random_start_flag = 0;

static void Generate_Random_Number(void)
{
  int randnum = 0;
	while(1)
	{
		m_key_num = Key_GetNum();
		if(m_key_num == KEY2_SINGLE_CLICK)
		{
			if(!random_start_flag)
				random_start_flag = 1;
		  else
				random_start_flag = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			return;
		}
		if(random_start_flag == 1)
		  randnum = rand();
		OLED_ShowNum(46,20,randnum,3,OLED_12X24);
		OLED_Update();
	}
}

int Timer_Page_Select(void)
{
  while(1)
	{
		uint8_t clock_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  clock_index--;
			if(clock_index < 0)
				clock_index = 5;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  clock_index++;
			if(clock_index > 5)
				clock_index = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			clock_index_temp = clock_index;
		}
		if(clock_index_temp == TIMER_RETURN_CHOICE)
			return 0;
		if(clock_index_temp == TIMER_TIMER_CHOICE)
		{
		  Counter_Timer();
		}
		if(clock_index_temp == TIMER_STOPWATCH_CHOICE)
		{
		  StopWatch();
		}
		if(clock_index_temp == TIMER_COUNTER_CHOICE)
		{
		  Counter_Machine();
		}
		if(clock_index_temp == TIMER_RANDNUM_CHOICE)
		{
		  Generate_Random_Number();
		}
		switch(clock_index)
		{
		  case TIMER_RETURN_CHOICE:
			{
				OLED_Clear();
			  Show_Timer_Choice();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			}
		  case TIMER_TIMER_CHOICE:
			{
				//OLED_Clear();
			  Show_Timer_Choice();
				OLED_ReverseArea(0,16,48,16);
				OLED_Update();
				break;
			}
			case TIMER_STOPWATCH_CHOICE:
			{
				//OLED_Clear();
			  Show_Timer_Choice();
				OLED_ReverseArea(0,32,32,16);
				OLED_Update();
				break;
			}
			case TIMER_COUNTER_CHOICE:
			{
				OLED_Clear();
			  Show_Timer_Choice();
				OLED_ReverseArea(0,48,48,16);
				OLED_Update();
				break;
			}
			case TIMER_ALARMCLOCK_CHOICE:
			{
				OLED_Clear();
				Show_Timer_Choice_P2();
				OLED_ReverseArea(0,0,32,16);
				OLED_Update();
				break;
			}
			case TIMER_RANDNUM_CHOICE:
			{
				OLED_Clear();
				Show_Timer_Choice_P2();
				OLED_ReverseArea(0,16,80,16);
				OLED_Update();
				break;
			}
			default:
				break;
		}
	}
}

/***************************Torch Page UI****************************/
int torch_index;
static uint16_t duty_cycle;
static uint8_t led_breath_direction;
static uint8_t blink_cnt,flash_cnt;
const uint8_t step = BREATH_STEP_CONFIG;

enum
{
  RETURN_PREV_MENU0 = 0,
	TORCH_ON,
	TORCH_OFF,
	RETURN_HOME_UI
};

static void Show_Torch_UI(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_ShowImage(112,0,16,16,Home_Image);
	OLED_ShowString(0,48,"ON",OLED_8X16);
	OLED_ShowString(104,48,"OFF",OLED_8X16);
}

static void Led_Torch_Off(void)
{
  Pwm_DutyCycle_Set(0);
}

static void Led_Torch_On(void)
{
  Pwm_DutyCycle_Set(100);
}

/**
  * @brief  Implementation of breath led Based on PWM,
            duty cycle value:0~100.
  * @param  None.
  * @retval None
  */
static void Breath_Led_Switch(void)
{
  if(!led_breath_direction)
	{
	  if(duty_cycle < 100 - step)
		{
		  duty_cycle += step;
		}
		else
		{
		  duty_cycle = 100;
			led_breath_direction = 1;
		}
	}
	else
	{
	  if(duty_cycle > step)
		{
		  duty_cycle -= step;
		}
		else
		{
		  duty_cycle = 0;
			led_breath_direction = 0;
		}
	}
	Pwm_DutyCycle_Set(duty_cycle);
}

static void Blink_Led_Switch(void)
{
  static uint8_t blink_state;
	blink_cnt++;
	if(blink_cnt >= BLINK_INTERVAL_CONFIG)
	{
	  blink_cnt = 0;
		blink_state = !blink_state;
		Pwm_DutyCycle_Set(blink_state ? 100 : 0);
	}
}

static void Flash_Led_Switch(void)
{
  static uint8_t flash_state,flash_times;
	flash_cnt++;
	if(flash_cnt >= 20)
	{
	  flash_cnt = 0;
		flash_state = !flash_state;
		if(flash_state)
		{
		  flash_times++;
			if(flash_times > FLASH_COUNT_CONFIG*2)
			{
			  flash_times = 0;
				Pwm_DutyCycle_Set(0);
				return;
			}
		}
		Pwm_DutyCycle_Set(flash_state ? 100 : 0);
	}
}

/**
  * @brief  Led effect switch function,which is called by TIM2
            it has 5 mode in total.
  * @param  None.
  * @retval None
  */
static void LedMode_Switch_Timer_Handler(void)
{
	if(ledmode_flag >= LED_MODE_MAX)
		return;
	static uint8_t timer_cnt;
	timer_cnt++;
	if(timer_cnt >= BREATH_INTERVAL_CONFIG)
	{
	  timer_cnt = 0;
		switch(ledmode_flag)
		{
			case LED_MODE_OFF:
			{
				Led_Torch_Off();
				blink_cnt = 0;
				flash_cnt = 0;
				duty_cycle = 0;
				led_breath_direction = 0;
				break;
			}
			case LED_MODE_TORCH:
			{
				Led_Torch_On();
				break;
			}
			case LED_MODE_BREATH:
			{
				Breath_Led_Switch();
				break;
			}
			case LED_MODE_BLINK:
			{
			  Blink_Led_Switch();
				break;
			}
			case LED_MODE_FLASH:
			{
			  Flash_Led_Switch();
				break;
			}
			default:
				break;
		}
	}
}

int Torch(void)
{
  while(1)
	{
		uint8_t torch_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  torch_index--;
			if(torch_index < 0)
				torch_index = RETURN_HOME_UI;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  torch_index++;
			if(torch_index > RETURN_HOME_UI)
				torch_index = RETURN_PREV_MENU0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
//			OLED_Clear();
//			OLED_Update();
		  torch_index_temp = torch_index;
		}
		
		if(torch_index_temp == RETURN_PREV_MENU0)
		{
		  LED1_OFF();
			return 0;
		}
		if(torch_index_temp == RETURN_HOME_UI)
		{
			LED1_OFF();
			OLED_Clear();
			OLED_Update();
		  First_Page_Clock();
			return 0;
		}
		
		switch(torch_index)
		{
		  case RETURN_PREV_MENU0:
			{
				Show_Torch_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
			  break;
			}
			case TORCH_ON:
			{
				Show_Torch_UI();
				LED1_ON();
				OLED_ShowImage(48,16,32,32,Torch_Image[0]);
				OLED_ReverseArea(0,48,16,16);
				OLED_Update();
			  break;
			}
			case TORCH_OFF:
			{
				Show_Torch_UI();
				LED1_OFF();
				OLED_ShowImage(48,16,32,32,Torch_Image[1]);
				OLED_ReverseArea(104,48,24,16);
				OLED_Update();
			  break;
			}
			case RETURN_HOME_UI:
			{
				Show_Torch_UI();
				OLED_ReverseArea(112,0,16,16);
				OLED_Update();
			  break;
			}
			default:
				break;
		}
		
	}
}

/***************************MPU6050 Page UI****************************/
int16_t ax,ay,az,gx,gy,gz;//MPU6050测得的三轴加速度和角速度
float roll_g,pitch_g,yaw_g;//陀螺仪解算的欧拉角
float roll_a,pitch_a;//加速度计解算的欧拉角
float roll,pitch,yaw;//互补滤波后的欧拉角
float a = 0.9;//互补滤波器系数
float delta_t = 0.005;//采样周期
double pi = 3.1415927;



static void MPU6050_Calculation(void)
{
  Delay_ms(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
	
	//通过陀螺仪解算欧拉角
	roll_g = roll+(float)gx*delta_t;
	pitch_g = pitch+(float)gy*delta_t;
	yaw_g = yaw+(float)gz*delta_t;
	
	//通过加速度计解算欧拉角
	pitch_a = atan2((-1)*ax,az)*180/pi;
	roll_a = atan2(ay,az)*180/pi;
	
	//通过互补滤波器进行数据融合
	roll = a*roll_g+(1-a)*roll_a;
	pitch = a*pitch_g+(1-a)*pitch_a;
	yaw = a*yaw_g;
}

static void Show_MPU6050_UI(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_Printf(0,16,OLED_8X16,"Roll: %.2f",roll);
	OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",pitch);
	OLED_Printf(0,48,OLED_8X16,"Yaw:  %.2f",yaw);
}

int MPU6050(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		MPU6050_Calculation();
		Show_MPU6050_UI();
		OLED_ReverseArea(0,0,16,16);
		OLED_Update();
	}
}

/***************************Game Page UI****************************/
int game_index;

typedef enum
{
	GAME_RETURN,
  GAME_GOOGLE_DINOSAUR,
	GAME_SNAKE,
	GAME_MATCH_THREE,
}game_info_t;

static void Show_Game_UI(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_ShowString(0,16,"谷歌小恐龙",OLED_8X16);
	OLED_ShowString(0,32,"贪吃蛇",OLED_8X16);
	OLED_ShowString(0,48,"消消乐",OLED_8X16);
}

uint8_t Game_Select_Page(void)
{
  while(1)
	{
		uint8_t game_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  game_index--;
			if(game_index < 0)
				game_index = 3;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  game_index++;
			if(game_index > 3)
				game_index = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			game_index_temp = game_index;
		}
		//confirm operation
		if(GAME_RETURN == game_index_temp)
			return 0;
		if(GAME_GOOGLE_DINOSAUR == game_index_temp)
		{
		  dinosaur_game_init();
			DinoGame_Animation();
		}
		switch(game_index)
		{
		  case GAME_RETURN:
			{
			  Show_Game_UI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			}
			case GAME_GOOGLE_DINOSAUR:
			{
			  Show_Game_UI();
				OLED_ReverseArea(0,16,80,16);
				OLED_Update();
				break;
			}
			case GAME_SNAKE:
			{
				Show_Game_UI();
			  OLED_ReverseArea(0,32,48,16);
				OLED_Update();
				break;
			}
			case GAME_MATCH_THREE:
			{
			  Show_Game_UI();
				OLED_ReverseArea(0,48,48,16);
				OLED_Update();
				break;
			}
			default:
				break;
		}
	}
}


/***************************Dynamic emoji Page UI****************************/

static void Show_Emoji_UI(void)
{
	//Close eye
	for(uint8_t i = 0;i < 3;i++)
	{
	  OLED_Clear();
		OLED_ShowImage(30,10+i,16,16,Eyebrow_Image[0]);
		OLED_ShowImage(82,10+i,16,16,Eyebrow_Image[1]);
		OLED_DrawEllipse(40,32,6,6-i,OLED_FILLED);
		OLED_DrawEllipse(88,32,6,6-i,OLED_FILLED);
		OLED_ShowImage(54,40,20,20,Mouth_Image);
	  OLED_Update();
		Delay_ms(300);
	}
	//Open eye
	for(uint8_t i = 0;i < 3;i++)
	{
	  OLED_Clear();
		OLED_ShowImage(30,12-i,16,16,Eyebrow_Image[0]);
		OLED_ShowImage(82,12-i,16,16,Eyebrow_Image[1]);
		OLED_DrawEllipse(40,32,6,4+i,OLED_FILLED);
		OLED_DrawEllipse(88,32,6,4+i,OLED_FILLED);
		OLED_ShowImage(54,40,20,20,Mouth_Image);
	  OLED_Update();
		Delay_ms(100);
	}
	Delay_ms(500);
}

int Dynamic_Emoji(void)
{
  while(1)
	{
		m_key_num = Key_GetNum();
		if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			return 0;
		}
	  Show_Emoji_UI();
	}
}

/***************************Gradienter Page UI****************************/

static void Show_Gradienter_UI(void)
{
	MPU6050_Calculation();
  OLED_DrawCircle(64,32,30,OLED_UNFILLED);
	OLED_DrawCircle(64-roll,32+pitch,4,OLED_FILLED);
}

int Gradienter(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			return 0;
		}
		OLED_Clear();
		Show_Gradienter_UI();
		OLED_Update();
	}
}


/***************************Menu Selection Page UI****************************/
int menu_index = 0;
uint8_t prev_selection;
uint8_t target_selection;
uint8_t x_pre = 48;
uint8_t speed_step = 4;
uint8_t move_flag = 0;

static void Show_Menu_Animation(void)
{
  OLED_Clear();
	OLED_ShowImage(42,10,44,44,Select_Image);
	if(prev_selection < target_selection)  //left move
	{
	  x_pre -= speed_step;
		if(x_pre == 0)
		{
		  prev_selection++;
			move_flag = 0;
			x_pre = 48;
		}
	}
	if(prev_selection > target_selection)  //right move
	{
	  x_pre += speed_step;
		if(x_pre == 96)
		{
		  prev_selection--;
			move_flag = 0;
			x_pre = 48;
		}
	}
	if(prev_selection >= 1)
	{
	  OLED_ShowImage(x_pre-48,16,32,32,Menu_Icon[prev_selection-1]);
	}
	if(prev_selection >= 2)
	{
	  OLED_ShowImage(x_pre-96,16,32,32,Menu_Icon[prev_selection-2]);
	}
	OLED_ShowImage(x_pre,16,32,32,Menu_Icon[prev_selection]);
	OLED_ShowImage(x_pre+48,16,32,32,Menu_Icon[prev_selection+1]);
	OLED_ShowImage(x_pre+96,16,32,32,Menu_Icon[prev_selection+2]);
	
	OLED_Update();
}

static void Set_Selection(uint8_t move_flag,uint8_t prev_select,uint8_t target_select)
{
  if(move_flag == 1)
	{
	  prev_selection = prev_select;
	  target_selection = target_select;
	}
	Show_Menu_Animation();
}

static void Menu_To_Function(void)
{
	for(uint8_t i = 0;i <= 6;i++)
	{
		OLED_Clear();
	  if(prev_selection >= 1)
		{
			OLED_ShowImage(x_pre-48,16+8*i,32,32,Menu_Icon[prev_selection-1]);
		}
		OLED_ShowImage(x_pre,16+8*i,32,32,Menu_Icon[prev_selection]);
		OLED_ShowImage(x_pre+48,16+8*i,32,32,Menu_Icon[prev_selection+1]);
		
		OLED_Update();
	}
}

int Enter_Menu_Option(void)
{
	move_flag = 1;
	uint8_t direct_flag = 2;   /*if set 1,last choice;if set 2,next choice*/
  while(1)
	{
		uint8_t menu_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
			direct_flag = 1;
			move_flag = 1;
		  menu_index--;
			if(menu_index < 0)
				menu_index = 6;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
			direct_flag = 2;
			move_flag = 1;
		  menu_index++;
			if(menu_index > 6)
				menu_index = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			menu_index_temp = menu_index;
		}
		//confirm and jump
		if(menu_index_temp == ROLLBACK_FUNC)
			return 0;
		if(menu_index_temp == STOPWATCH_FUNC)
		{
		  Menu_To_Function();
			Timer_Page_Select();
		}
		if(menu_index_temp == TORCH_FUNC)
		{
			Menu_To_Function();
		  Torch();
		}
		if(MPU6050_FUNC == menu_index_temp)
		{
			Menu_To_Function();
		  MPU6050();
		}
		if(GAME_FUNC == menu_index_temp)
		{
			Menu_To_Function();
		  Game_Select_Page();
		}
		if(TV_FUNC == menu_index_temp)
		{
			Menu_To_Function();
		  Dynamic_Emoji();
		}
		if(GRADIENTER_FUNC == menu_index_temp)
		{
		  Menu_To_Function();
			Gradienter();
		}
		
		if(menu_index == ROLLBACK_FUNC)
		{
		  if(direct_flag == 1)
				Set_Selection(move_flag,1,0);
			else if(direct_flag == 2)
				Set_Selection(move_flag,0,0);
		}
		else
		{
		  if(direct_flag == 1)
				Set_Selection(move_flag,menu_index+1,menu_index);
			else if(direct_flag == 2)
				Set_Selection(move_flag,menu_index-1,menu_index);
		}
	}
}



