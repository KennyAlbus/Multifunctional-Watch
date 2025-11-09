#include "stm32f10x.h"                  // Device header
#include "uart_drv.h"
#include <stdio.h>



#ifndef   UART_LOG_ENABLE
#define   UART_LOG_ENABLE    (1)
#endif

#if  UART_LOG_ENABLE
#define  LOG_UART_INFO  log_debug
#else
#define  LOG_UART_INFO(...)
#endif


typedef struct
{
  USART_TypeDef* uart_no;
	uint16_t tx_pin;
	uint16_t rx_pin;
}uart_HwInfo_t;


static uart_HwInfo_t m_uart_info = {USART1,GPIO_Pin_9,GPIO_Pin_10};


static void uart_gpio_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  GPIO_InitTypeDef gpio_initStruct;
	gpio_initStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_initStruct.GPIO_Pin = m_uart_info.tx_pin;
	gpio_initStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_initStruct);
	
	gpio_initStruct.GPIO_Mode = GPIO_Mode_IPU;
	gpio_initStruct.GPIO_Pin = m_uart_info.rx_pin;
	gpio_initStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&gpio_initStruct);
}

static void uart_init(uint32_t brate)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	USART_InitTypeDef usart_initstruct;
	USART_StructInit(&usart_initstruct);
	usart_initstruct.USART_BaudRate = brate;
	USART_Init(m_uart_info.uart_no,&usart_initstruct);
	USART_Cmd(m_uart_info.uart_no,ENABLE);
}


void uart_drv_init(uint32_t baudrate)
{
  uart_gpio_init();
	uart_init(baudrate);
	//printf("[UART] baudrate:%d\n",baudrate);
}

int fputc(int ch,FILE *f)
{
  USART_SendData(m_uart_info.uart_no,(uint16_t)ch);
	while(SET != USART_GetFlagStatus(m_uart_info.uart_no,USART_FLAG_TXE));
	return ch;
}

