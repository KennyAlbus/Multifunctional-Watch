#include <stdlib.h>
#include <math.h>
#include "OLED.h"
#include "Key.h"
#include "Timer.h"
#include "Delay.h"




/******Private Variation define******/
static uint8_t m_key_num = 0;
static double pi = 3.1415927;

uint32_t score;
uint16_t ground_pos;
uint16_t barrier_pos;
uint16_t cloud_pos;
uint16_t dino_pos;
uint16_t jump_pos;
uint16_t jump_t;
uint8_t barrier_index;
uint8_t cloud_index;
uint8_t dino_jump_flag = 0;  //0:run  1:jump

enum
{
  DINOSAUR_RUN,
	DINOSAUR_JUMP,
};

typedef struct
{
  uint8_t min_x;
	uint8_t min_y;
  uint8_t max_x;
	uint8_t max_y;
}Ojection_Pos_t;

static Ojection_Pos_t barrier;
static Ojection_Pos_t dinosaur;

//extern uint8_t OLED_DisplayBuf[8][128];


static void Show_Score(void)
{
  OLED_ShowNum(98,0,score,5,OLED_6X8);
}

static void Show_Ground(void)
{
	if(ground_pos < 128)
	{
	  for(uint8_t i = 0;i < 128;i++)
		{
			OLED_DisplayBuf[7][i] = Ground_Image[i+ground_pos];
		}
	}
  else   //>=128
	{
	  for(uint8_t i = 0;i < 255-ground_pos+1;i++)   //后半块地（长度128）一直向左移。
		{
		  OLED_DisplayBuf[7][i] = Ground_Image[i+ground_pos];
		}
		for(uint8_t i = 255-ground_pos;i < 128;i++)  //前半块地（长度128）左移进屏幕。
		{
		  OLED_DisplayBuf[7][i] = Ground_Image[i-(255-ground_pos)];
		}
	}
}

static void Show_Barrier(void)
{
	if(barrier_pos > 143)
		barrier_index = rand()%6;
	OLED_ShowImage(127-barrier_pos,44,16,18,Barrier_Image[barrier_index]);
	
	barrier.min_x = 127-barrier_pos;
	barrier.max_x = barrier.min_x+16;
	barrier.min_y = 44;
	barrier.max_y = barrier.min_y+18;
}

static void Show_Cloud(void)
{
  if(cloud_pos > 199)
		cloud_index = rand()%3;
	OLED_ShowImage(127-cloud_pos,8,16,8,Cloud_Image[cloud_index]);
}

static void Show_Dinosaur(void)
{
	m_key_num = Key_GetNum();
	if(m_key_num == 1)
		dino_jump_flag = DINOSAUR_JUMP;
	jump_pos = 28*sin((float)(pi*jump_t/1000));
  if(dino_jump_flag == DINOSAUR_RUN)
	{
	  if(dino_pos%2 == 0)
			OLED_ShowImage(0,44,16,18,Dino_Image[0]);
		else
			OLED_ShowImage(0,44,16,18,Dino_Image[1]);
	}
	else
	{
	  OLED_ShowImage(0,44-jump_pos,16,18,Dino_Image[2]);
	}
	
	dinosaur.min_x = 0;
	dinosaur.max_x = 16;
	dinosaur.min_y = 44-jump_pos;
	dinosaur.max_y = dinosaur.min_y+18;
}

static uint8_t Is_Colliding(Ojection_Pos_t *a,Ojection_Pos_t *b)
{
  if((a->max_x > b->min_x)&&(a->min_x < b->max_x)
		&&(a->min_y < b->max_y)&&(b->min_y < a->max_y))
	{
	  OLED_Clear();
		OLED_ShowString(28,24,"Game Over",OLED_8X16);
		OLED_Update();
		Delay_s(1);
		return 1;
	}
	return 0;
}

static void DinoScore_Tick(void)
{
  static uint16_t count;
	static uint16_t ground_cnt,cloud_cnt;
	count++;
	ground_cnt++;
	cloud_cnt++;
	if(ground_cnt >= 20)
	{
	  ground_cnt = 0;
		ground_pos++;
		barrier_pos++;
		if(ground_pos > 255)
			ground_pos = 0;
		if(barrier_pos > 144)
			barrier_pos = 0;
	}
	if(cloud_cnt > 50)
	{
	  cloud_cnt = 0;
		cloud_pos++;
		dino_pos++;
		if(cloud_pos > 200)
			cloud_pos = 0;
		if(dino_pos > 65535)
			dino_pos = 0;
	}
	if(count >= 100)
	{
	  count = 0;
		score++;
		if(score > 99999)
			score = 0;
	}
	if(DINOSAUR_JUMP == dino_jump_flag)
	{
	  jump_t++;
		if(jump_t >= 1000)
		{
		  jump_t = 0;
			dino_jump_flag = DINOSAUR_RUN;
		}
	}
}

void DinoGame_Animation(void)
{
	while(1)
	{
	  OLED_Clear();
		Show_Score();
		Show_Ground();
		Show_Barrier();
		Show_Cloud();
		Show_Dinosaur();
		OLED_Update();
		uint8_t collide_flag = Is_Colliding(&barrier,&dinosaur);
		if(collide_flag)
		{
		  OLED_Clear();
			OLED_Update();
			return;
		}
	}
}

void dinosaur_game_init(void)
{
  score = 0;
	ground_pos = 0;
	barrier_pos = 0;
	cloud_pos = 0;
	jump_pos = 0;
}
void Game_App_Init(void)
{
  DinoScore_Timer_Callback(DinoScore_Tick);
}
