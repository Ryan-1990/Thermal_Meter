/******************************************************************************\
*  Copyright (C) 2012 Nan Li, Tianjin University 
*  ALL RIGHTS RESERVED
*  Filename: OLED.h
*  Version: 1.0
*  Author: Nan Li
\******************************************************************************/




#include "derivative.h"
#include "Delay.h"
#include <stdio.h>
 

#define D0  PTDD_PTDD2 
#define D1	PTBD_PTBD2
#define RST PTBD_PTBD1
#define DC  PTBD_PTBD0

/*
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
#define Max_Column	128
#define Max_Row		  64
*/

#define	Brightness	0xFF 
#define X_WIDTH 128
#define Y_WIDTH 64


 
void LCD_Init(void);
void LCD_P6x8Str(byte x,byte y,byte ch[]);
void LCD_P14x16Str(byte x,byte y,byte ch[]);
void LCD_Fill(byte dat);
void LCD_clear(void);
void LCD_clear_line(unsigned int line);
void LCD_clear_all(void);