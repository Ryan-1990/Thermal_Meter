#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */ 
#include "Init.h"
#include "OLED.h"
#include "Delay.h"
#include "SCI.h"
#include "TN901.h"


#define LED PTED_PTED4 
#define laser PTDD_PTDD0

#define OL 'L'
#define CL 'l'
#define OS 'S'
#define CS 's'
#define BT 'B'
#define CT 'b'


extern unsigned char table_mbtemp[];

byte Rec = 0;
byte begin_flag = 0;
byte screen_flag = 1;



void main(void) 
{
  DisableInterrupts;
  Init();
  test();
  EnableInterrupts;
  
  for(;;)  
  {   
    if(begin_flag)
    {
      begin_flag = 0;
      test();
      test();
      if(screen_flag)
      {
        LCD_P6x8Str(40,3,table_mbtemp);
      }
      PutStr(table_mbtemp);
    }
  }
}


interrupt 17 void SCI1()
{ 
  DisableInterrupts;
  LED = ~LED;
  while(!SCI1S1_RDRF);
  Rec = SCI1D;
  
  if(Rec == OL) laser = 1;
  if(Rec == CL) laser = 0;
  
  if(Rec == OS) 
  {
    screen_flag = 1;
    test();
    test();
    LCD_P14x16Str(15,0,"×Ô¶¯ÎÂ¶È¼ì²âÒÇ"); 
    LCD_P6x8Str(40,3,table_mbtemp);
  }
  if(Rec == CS) 
  {
    screen_flag = 0; 
    LCD_clear_all();
  }
  
  if(Rec == BT) begin_flag = 1;
  if(Rec == CT) begin_flag = 0;
  EnableInterrupts;
}







































