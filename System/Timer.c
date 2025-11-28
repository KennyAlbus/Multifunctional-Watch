#include "stm32f10x.h"                  // Device header
#include "Key.h"
#include "LED.h"


static void (*p_timer_timer_handler)(void);
static void (*p_counter_timer_handler)(void);
static void (*p_stopwatch_timer_handler)(void);
static void (*p_dinoScore_timer_handler)(void);
static void (*p_ledMode_timer_handler)(void);


static uint32_t m_run_time;

void Timer_Timer_Callback(void (*p_func0)(void),
	                        void (*p_func1)(void),
	                        void (*p_func2)(void))
{
  p_timer_timer_handler = p_func0;
	p_counter_timer_handler = p_func1;
	p_stopwatch_timer_handler = p_func2;
}

void Led_Timer_Callback(void(*p_func)(void))
{
  p_ledMode_timer_handler = p_func;
}

void DinoScore_Timer_Callback(void (*pfunc)(void))
{
  p_dinoScore_timer_handler = pfunc;
}

void Timer_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);
}


void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		Key_Timer_Handler();
		p_timer_timer_handler();
		p_counter_timer_handler();
		p_stopwatch_timer_handler();
		p_ledMode_timer_handler();
		p_dinoScore_timer_handler();
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

uint32_t Run_Time_Get(void)
{
  return m_run_time;
}
