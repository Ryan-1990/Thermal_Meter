/******************************************************************************\
*  Copyright (C) 2011 Nan Li, Tianjin University 
*  ALL RIGHTS RESERVED
*  Filename:SCI.c
*  Version: 1.0
*  Author: Nan Li
\******************************************************************************/






#include "SCI.h"


void PutChar(unsigned char ch)
{ 
  byte dummy;
  dummy = SCI1S1; 
  SCI1D = ch;
  while(!SCI1S1_TDRE);
  while(!SCI1S1_TC);
}                 


void PutStr(char msg[])
{
  byte ix=0;	   	
  byte dummy;	   		
  byte nxt_char; 	
  dummy = SCI1S1;   	
  nxt_char = msg[ix++];
  while(nxt_char != 0x00) 
  {
    SCI1D = nxt_char; 		
    nxt_char = msg[ix++];
    while(!SCI1S1_TDRE); 
  } 
  while(!SCI1S1_TC);
}