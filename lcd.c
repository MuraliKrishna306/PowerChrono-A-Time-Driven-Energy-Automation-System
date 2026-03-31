//lcd.c
#include "types.h"
#include <LPC21xx.h>
#include "defines.h"
#include "lcd_defines.h"
#include "delay.h"

void WriteLCD(u8 byte)
{
	#if LCD_MODE==8
	//select write operation
	IOCLR0=1<<LCD_RW;
	//write any byte onto data pin(d0/p0.8-d7/p0.15)
	WRITEBYTE(IOPIN0,LCD_DATA,byte);
	//provide high to low pulse on en pin
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
	#elif LCD_MODE==4
  //statements to be added	
	#endif
}

void CmdLCD(u8 cmdByte)
{
	//select cmd/instruction reg
	IOCLR0=1<<LCD_RS;
	//Write cmd to hd44780 controller
	WriteLCD(cmdByte);
}

void InitLCD(void)
{
	//cfg p0.8-p0.15,p0.16,p0.17,p0.18 as gpio output pins
	IODIR0|=((0xFF<<LCD_DATA)|
	        (1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
	
	delay_ms(15);
	CmdLCD(0x30);
	delay_ms(4);
	delay_us(100);
	CmdLCD(0x30);
	delay_us(100);
	CmdLCD(0x30);
	CmdLCD(MODE_8BIT_2LINE);
	CmdLCD(DSP_ON_CUR_OFF);
	CmdLCD(CLEAR_LCD);
	CmdLCD(SHIFT_CUR_RIGHT);
}

void CharLCD(u8 asciiVal)
{
	//select data reg
	IOSET0=1<<LCD_RS;
	//write to ddram/disp via data reg
	WriteLCD(asciiVal);
}

void StrLCD(s8 *s)
{
	while(*s)
		CharLCD(*s++);
}

void U32LCD(u32 n)
{
	s32 i=0;
	u8 a[10];
	
	if(n==0)
	{
		CharLCD('0');
	}
	else
	{
		while(n>0)
		{
		  a[i++]=(n%10)+48;
      n/=10;			
		}
		for(--i;i>=0;i--)
		  CharLCD(a[i]);
	}
}

void S32LCD(s32 n)
{
  if(n<0)
  {
		CharLCD('-');
		n=-n;
	}		
	U32LCD(n);
}	

void F32LCD(f32 fn,u8 nDP)
{
	u32 n,i;
	if(fn<0.0)
	{
		CharLCD('-');
		fn=-fn;
	}
  n=fn;
  U32LCD(n);
  CharLCD('.');
  for(i=0;i<nDP;i++)
  {
		fn=(fn-n)*10;
		n=fn;
		CharLCD(n+48);
  }		
}

void BuildCGRAM(u8 *p,u8 nBytes)
{
	u32 i;
	//point to cgram start
	CmdLCD(GOTO_CGRAM_START);
	//select data reg
	IOSET0=1<<LCD_RS;
	
	//write to cgram via data reg
	for(i=0;i<nBytes;i++)
	{
    WriteLCD(p[i]);		
	}	
	//point back to ddram start
	CmdLCD(GOTO_LINE1_POS0);
}
