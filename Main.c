/******  Mini-Project  ******/

#include <LPC214x.h>
#include "lcd.h"
#include "lcd_defines.h"
#include "pin_connect_block.h"
#include "kpm.h"
#include "eint.h"
#include "rtc.h"
#include "types.h"
#include "powerchrono.h"

extern u8 BulbONOFF[16];
extern u32 flag;

int main()
{
	s8 Key;
  
	RTC_Init();					// Initialize RTC 		
	InitLCD();					// Initialize the LCD
	Init_KPM();					// Initialize KPM
	Init_Interrupt();	  // Initialize Interrupt 

	BuildCGRAM(BulbONOFF,16);		//CGgram Build
	IODIR0|=1<<DEVICE_PIN;			//Configure P0.5 pin as Device Pin

  while(1)
	{
		DisplayRTCInfo();
		if(flag==1)
		{
			DisplayMenu();  // Menu  Display 1.
			Key=KeyScan();
			if(Key==49)
			{
				Edit_RTC_Time();
			}
			else if(Key==50)
			{
				Edit_OnOff_Time();
			}
			else if(Key==51)
			{
					flag=0;
					CmdLCD(CLEAR_LCD); 
			}
		}
		else if(flag==2)
		{
			Display_OnOff_Time();
		}
  	}
}
