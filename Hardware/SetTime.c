#include "user_config.h"
#include "MyRTC.h"
#include "Timer.h"
#include "OLED.h"
#include "Key.h"
#include "menu.h"



/******Macro definition******/
#define    SET_TIME_MINUS      0
#define    SET_TIME_PLUS       1


/******Private Variation define******/
static uint8_t m_key_num = 0;


/******Global Variation define******/
int ts_index = 0;   /*cursor setting index*/
//int setflag = 0;  /*cursor setting index*/


static void Show_SetTime_FirstUI(void)
{
  OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_Printf(0,16,OLED_8X16,"年:%04d",MyRTC_Time[0]);
	OLED_Printf(0,32,OLED_8X16,"月:%02d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"日:%02d",MyRTC_Time[2]);
	OLED_ShowImage(112,0,16,16,Home_Image);
}

static void Show_SetTime_SecondUI(void)
{
	OLED_ShowImage(0,0,16,16,Return_Image);
	OLED_Printf(0,16,OLED_8X16,"时:%02d",MyRTC_Time[3]);
	OLED_Printf(0,32,OLED_8X16,"分:%02d",MyRTC_Time[4]);
	OLED_Printf(0,48,OLED_8X16,"秒:%02d",MyRTC_Time[5]);
	OLED_ShowImage(112,0,16,16,Home_Image);
}

static void Change_RTC_Time(uint8_t index,uint8_t flag)
{
  switch(flag)
	{
	  case SET_TIME_PLUS:
		{
			MyRTC_Time[index]++;
			//MyRTC_SetTime();
		  break;
		}
		case SET_TIME_MINUS:
		{
			MyRTC_Time[index]--;
			//MyRTC_SetTime();
		  break;
		}
		default:
			break;
	}
}

static void Set_Year(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK || m_key_num == KEY1_REPEAT_HOLD)
		{
		  Change_RTC_Time(0,SET_TIME_MINUS);
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY2_SINGLE_CLICK || m_key_num == KEY2_REPEAT_HOLD)
		{
		  Change_RTC_Time(0,SET_TIME_PLUS);
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  return;
		}
		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,16,32,16);
		OLED_Update();
	}
}

static void Set_Month(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK || m_key_num == KEY1_REPEAT_HOLD)
		{
		  Change_RTC_Time(1,SET_TIME_MINUS);
			if(MyRTC_Time[1] < 1)
				MyRTC_Time[1] = 12;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY2_SINGLE_CLICK || m_key_num == KEY2_REPEAT_HOLD)
		{
		  Change_RTC_Time(1,SET_TIME_PLUS);
			if(MyRTC_Time[1] > 12)
				MyRTC_Time[1] = 1;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  return;
		}
		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}
}

static void Set_Day(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK || m_key_num == KEY1_REPEAT_HOLD)
		{
		  Change_RTC_Time(2,SET_TIME_MINUS);
			if(MyRTC_Time[2] < 1)
				MyRTC_Time[2] = 31;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY2_SINGLE_CLICK || m_key_num == KEY2_REPEAT_HOLD)
		{
		  Change_RTC_Time(2,SET_TIME_PLUS);
			if(MyRTC_Time[2] > 31)
				MyRTC_Time[2] = 1;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  return;
		}
		Show_SetTime_FirstUI();
		OLED_ReverseArea(24,48,16,16);
		OLED_Update();
	}
}

static void Set_Hour(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK || m_key_num == KEY1_REPEAT_HOLD)
		{
		  Change_RTC_Time(3,SET_TIME_MINUS);
			if(MyRTC_Time[3] < 0)
				MyRTC_Time[3] = 23;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY2_SINGLE_CLICK || m_key_num == KEY2_REPEAT_HOLD)
		{
		  Change_RTC_Time(3,SET_TIME_PLUS);
			if(MyRTC_Time[3] > 23)
				MyRTC_Time[3] = 0;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  return;
		}
		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,16,16,16);
		OLED_Update();
	}
}

static void Set_Minute(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK || m_key_num == KEY1_REPEAT_HOLD)
		{
		  Change_RTC_Time(4,SET_TIME_MINUS);
			if(MyRTC_Time[4] < 0)
				MyRTC_Time[4] = 59;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY2_SINGLE_CLICK || m_key_num == KEY2_REPEAT_HOLD)
		{
		  Change_RTC_Time(4,SET_TIME_PLUS);
			if(MyRTC_Time[4] > 59)
				MyRTC_Time[4] = 0;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  return;
		}
		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,32,16,16);
		OLED_Update();
	}
}

static void Set_Second(void)
{
  while(1)
	{
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK || m_key_num == KEY1_REPEAT_HOLD)
		{
		  Change_RTC_Time(5,SET_TIME_MINUS);
			if(MyRTC_Time[5] < 0)
				MyRTC_Time[5] = 59;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY2_SINGLE_CLICK|| m_key_num == KEY2_REPEAT_HOLD)
		{
		  Change_RTC_Time(5,SET_TIME_PLUS);
			if(MyRTC_Time[5] > 59)
				MyRTC_Time[5] = 0;
			MyRTC_SetTime();
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  return;
		}
		Show_SetTime_SecondUI();
		OLED_ReverseArea(24,48,16,16);
		OLED_Update();
	}
}

int Time_Setting_Page(void)
{
  while(1)
	{
		uint8_t ts_index_temp = 0xff;
	  m_key_num = Key_GetNum();
		if(m_key_num == KEY1_SINGLE_CLICK)
		{
		  ts_index--;
			if(ts_index < 0)
				ts_index = 7;
		}
		else if(m_key_num == KEY2_SINGLE_CLICK)
		{
		  ts_index++;
			if(ts_index > 7)
				ts_index = 0;
		}
		else if(m_key_num == KEY3_SINGLE_CLICK)
		{
		  OLED_Clear();
			OLED_Update();
			ts_index_temp = ts_index;
		}
		
		if(ts_index_temp == 0)
			return 0;
		if(ts_index_temp == 7)
		{
		  First_Page_Clock();
			ts_index = 0;
			return 0;
		}
		if(ts_index_temp == 1)
		{
		  Set_Year();
		}
		else if(ts_index_temp == 2)
		{
		  Set_Month();
		}
		else if(ts_index_temp == 3)
		{
		  Set_Day();
		}
		else if(ts_index_temp == 4)
		{
		  Set_Hour();
		}
		else if(ts_index_temp == 5)
		{
		  Set_Minute();
		}
		else if(ts_index_temp == 6)
		{
		  Set_Second();
		}
		switch(ts_index)
		{
		  case 0:
			{
				OLED_Clear();
			  Show_SetTime_FirstUI();
				OLED_ReverseArea(0,0,16,16);
				OLED_Update();
				break;
			}
			case 1:
			{
				OLED_Clear();
			  Show_SetTime_FirstUI();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
			}
			case 2:
			{
				OLED_Clear();
			  Show_SetTime_FirstUI();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;
			}
			case 3:
			{
				OLED_Clear();
			  Show_SetTime_FirstUI();
				OLED_ReverseArea(0,48,16,16);
				OLED_Update();
				break;
			}
			case 4:
			{
				OLED_Clear();
			  Show_SetTime_SecondUI();
				OLED_ReverseArea(0,16,16,16);
				OLED_Update();
				break;
			}
			case 5:
			{
				OLED_Clear();
			  Show_SetTime_SecondUI();
				OLED_ReverseArea(0,32,16,16);
				OLED_Update();
				break;
			}
			case 6:
			{
				OLED_Clear();
			  Show_SetTime_SecondUI();
				OLED_ReverseArea(0,48,16,16);
				OLED_Update();
				break;
			}
			case 7:
			{
				OLED_Clear();
			  Show_SetTime_SecondUI();
				OLED_ReverseArea(112,0,16,16);
				OLED_Update();
				break;
			}
			default:
				break;
		}
	}
}
