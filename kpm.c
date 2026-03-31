//kpm.c
#include "types.h"
#include <LPC21xx.h>
#include "delay.h"
#include "defines.h"
#include "kpm_defines.h"
#include "lcd_defines.h"
#include "lcd.h"

/*
u32 kpmLUT[4][4]=
{
	{'1','2','3','4'},
	{'5','6','7','8'},
	{'9','0','*','#'},
	{13,14,15,16}
};
*/

u32 kpmLUT[4][4]=
{
	{'1','2','3','A'},
	{'4','5','6','B'},
	{'7','8','9','C'},
	{'*','0','#','D'}
};

void Init_KPM(void)
{																																
	//cfg row lines as gpio out 
	WRITENIBBLE(IODIR1,ROW0,15);
	//cfg col lines as gpio in,and they be default
	//WRITENIBBLE(IODIR1,COL0,0);
}

u32  ColScan(void)
{
	u32 status;
	status=READNIBBLE(IOPIN1,COL0);
	return (status < 15) ? 0 : 1;
}

u32  RowCheck(void)
{
	u32 r;
	for(r=0;r<=3;r++)
	{
		//starting from row0 check if key was pressed
		//in that row
		WRITENIBBLE(IOPIN1,ROW0,~(1<<r));
		if(ColScan()==0)
		{
			break;
		}
	}
	//re-initialize all rows ground
	WRITENIBBLE(IOPIN1,ROW0,0);
	return r;
}

u32  ColCheck(void)
{
	u32 c;
	for(c=0;c<=3;c++)
	{
		if(READBIT(IOPIN1,COL0+c)==0)
			break;
	}
	return c;
}

u32  KeyScan(void)
{
	u32 r,c,keyV;
	//wait for any key press
	while(ColScan());
	//if key pressed detected 
	//identify row in which key was pressed
	r=RowCheck();
	//identify col in which key was pressed
	c=ColCheck();
	keyV=kpmLUT[r][c];
	while(ColScan()==0);
	delay_ms(250);
	return keyV;
}

void  ReadNum(u32 *num,u32 *lastKey)
{
	u8 keyV;
	*num=0;
	while(1)
	{
		 keyV=KeyScan();
		 *lastKey=keyV;
		 if(keyV>='0' && keyV<='9')
		 {	 
		   *num = (*num*10)+(keyV-48);
			 while(ColScan()==0);
			 CmdLCD(GOTO_LINE2_POS0);
			 U32LCD(*num);			 
		 }
		 else if(keyV=='#')
		 {
		 	*num = *num/10;
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("          ");
			CmdLCD(GOTO_LINE2_POS0);
			U32LCD(*num);
		 }
     else if(keyV=='*')
     {
			 CmdLCD(GOTO_LINE2_POS0+8);
       CharLCD(*lastKey); 
			 while(ColScan()==0);
			 break;
     }			 
	}
}
