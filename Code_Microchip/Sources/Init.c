/******************************************************************************\
*  Copyright (C) 2013 LI Nan, Tianjin University 
*  ALL RIGHTS RESERVED
*  Filename:Init.c
*  Version: 1.0
*  Author: LI Nan
\******************************************************************************/






#include "Init.h"
#include "OLED.h"



void SetBusCLK_16M(void)
{
  ICGC1 = 0x78;
  ICGC2 = 0x00;  
  ICGTRM=0xa8;
  SOPT_COPE=0;
  SOPT2=0x00;
}

void Port_Init(void)
{   
  //OLED
  PTBDD = 0X0F;
  PTBDS = 0x0F;
  PTBD=0X00;
  PTDDD_PTDDD2 = 1;
  
  //laser
  PTDDD_PTDDD0 = 1;
  PTDD_PTDD0 = 0;
  
  //LED
  PTEDD_PTEDD2 = 1;
  PTED_PTED2 = 0;
  
  //TN901
  PTADD_PTADD0=0;
  PTADD_PTADD1=0;
  PTDDD_PTDDD1=1;
}


void SCI_Init(void) 
{
  SCI1BD = 16000000/16/9600;
  SCI1C2= 0X2C;        //开接收中断, 允许发送接收
} 


void Init(void)
{
  SOPT = 0X30; 
  SetBusCLK_16M(); 
  Port_Init();
  SCI_Init(); 
  LCD_Init(); 
  LCD_P14x16Str(15,0,"自动温度检测仪");
}
