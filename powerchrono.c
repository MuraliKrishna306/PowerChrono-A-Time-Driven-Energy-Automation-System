/*** powerchrono.c	***/
#include <LPC214x.h>
#include "delay.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "pin_connect_block.h"
#include "kpm.h"
#include "eint.h"
#include "rtc.h"
#include "types.h"
#include "powerchrono.h"

#define DEVICE_PIN 5	//p0.5

s32 on_hour,on_min,on_sec,off_hour,off_min,off_sec;
s32 hour,min,sec,date,month,year,day;

u8 BulbONOFF[16]={0x00,0x0E,0x11,0x11,0x0E,0x0E,0x1F,0x00,0x00,0x0E,0x1F,0x1F,0x0E,0x0E,0x1F,0x00};

u8 Disp[7][6]={"Hr " , "Min " , "Sec " , "Day " , "Date " , "Mnth " , "Year"};
u8 Range[6][7]={"(0-23)"  , "(0-59)" , "(0-6)" , "(1-31)" , "(1-12)" , "\0"};
u8 Msg[2][9]={"Entr " , "Invalid "};
 
extern u32 flag;
/* Function: DisplayRTCInfo(void)
   Purpose : Display isplay RTC Time Info on The Screen
 */ 
void DisplayRTCInfo(void)
{
//	CmdLCD(CLEAR_LCD);
	GetRTCTimeInfo(&hour,&min,&sec);
	DisplayRTCTime(hour,min,sec);				// Get and display the current time on LCD
	GetRTCDateInfo(&date,&month,&year);
	DisplayRTCDate(date,month,year);		// Get and display the current Date on LCD
	CmdLCD(GOTO_LINE2_POS0+11);
	StrLCD("DEV:");
	GetRTCDay(&day);
	DisplayRTCDay(day);									// Get and display the current Week on LCD

	if(HOUR>=on_hour && HOUR<=off_hour && MIN>=on_min   && MIN<off_min ) //|| (sec>=on_sec && sec<=off_sec) )
		{
			IOSET0=1<<DEVICE_PIN;
			CmdLCD(GOTO_LINE2_POS0+15);	//Device ON Condition
			CharLCD(1);
		}
		else
		{
			IOCLR0=1<<DEVICE_PIN;
			CmdLCD(GOTO_LINE2_POS0+15);	//Device OFF Condition
			CharLCD(0);

		}
}

/* Function: DisplayMenu(void)
   Purpose : Display Menu on the LCD Screen
*/
void DisplayMenu(void)
{
	CmdLCD(CLEAR_LCD);		  	//1. Edit Time
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("1.E.TIME");

	CmdLCD(GOTO_LINE1_POS0+9);	//3. Exit
	StrLCD("3.EXIT");

	CmdLCD(GOTO_LINE2_POS0);	//2. Edit On / Off Time
	StrLCD("2.E.ONOFFTIME");
}


/* Function: Edit_RTC_Time()
   Purpose : Allows user to edit Hour, Minute, Second of RTC
*/
void Edit_RTC_Time(void)
{
	u8 dummy;
EDIT_RTC:	CmdLCD(CLEAR_LCD);
	CmdLCD(GOTO_LINE1_POS0);
	StrLCD("1.H 2.M 3.S 4.DY");	 //Edit RTC Time Info  1. HOUR  2. MINUTE  3. SECONDS  4. DAY
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("5.DA 6.M 7.Y 8.E");	 //Edit RTC Time Info  5. DATE  6. MONTH  7. YEAR  8. EXIT
	dummy=(u8)KeyScan();

	CmdLCD(CLEAR_LCD);
//	StrLCD("EDIT RTC TIME");
	delay_ms(1000);

	 delay_ms(200);
	 if(dummy=='1')			   	// 1. Edit HOURS
	 {
	    EditInfo(&hour , 0 , 23 , Range[0] , Disp[0]);
	 }

	 else if(dummy=='2')	  // 2. Edit MINUTES
	 {
	    EditInfo(&min , 0 , 59 , Range[1] , Disp[1]);
	 }

	 else if(dummy=='3')	  // 3. Edit SECONDS
	 {
	    EditInfo(&sec , 0 , 59 , Range[1] , Disp[2]);
	 }

	 else if(dummy=='4')    //  4. Edit DAY
	 {
	    EditInfo(&day , 0 , 6 , Range[2] , Disp[3]);
	 }

	 else if(dummy=='5')    //  5.  Edit DATE
	 {
	    EditInfo(&date , 1 , 31 , Range[3] , Disp[4]);
	 }

	 else if(dummy=='6')    //  6.  Edit MONTH
	 {
	    EditInfo(&month , 1 , 12 , Range[4] , Disp[5] );
	 }

	 else if(dummy=='7')    //  7.  Edit YEAR
	 {
	    EditInfo(&year , 0 , 4085 , Range[5] , Disp[6] );
	 }

	 if(dummy!='8')
	 goto EDIT_RTC;

	 else if(dummy=='8')
	 {
	    if(hour<24 && min<60 && sec<60)
	        SetRTCTimeInfo(hour,min,sec);
    	if(date<=31 && month<=12 && year<=4895)
        	SetRTCDateInfo(date,month,year);
    	if(day<7)
    		SetRTCDay(day);
    	CmdLCD(CLEAR_LCD);
    	StrLCD("RTC UPDATED!");
    	delay_ms(2500);
    	CmdLCD(CLEAR_LCD);
    	flag=0;
	    return;
	}
}

/* Function: Edit_OnOff_Time()
   Purpose : Allows user to set ON and OFF time for device
*/
void Edit_OnOff_Time(void)
{
	u8 keyV,Key;
EDIT: while(1)
{
	CmdLCD(CLEAR_LCD);
	StrLCD("1.ON TIME");
	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("2.OFF TIME");
	CmdLCD(GOTO_LINE1_POS0+11);
	StrLCD("3.EXIT");

	keyV = (u8)KeyScan();
	delay_ms(200);
	switch(keyV)
	{
		case '1':   // ON TIME
ON:			CmdLCD(CLEAR_LCD);
			StrLCD("1.Hr 2.Min 3.Sec");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("4.Exit");
			Key=(u8)KeyScan();
			delay_ms(200);
			CmdLCD(CLEAR_LCD);
			if(Key=='1')             // Edit ON Hour
			{					    
		        EditInfo(&on_hour , 0 , 23 , Range[0] , Disp[0]);
			}

			else if(Key=='2')       // Edit ON Minute
			{    
		       	EditInfo(&on_min , 0 , 59 , Range[1] , Disp[1]);
			}

				else if(Key=='3')   // Edit ON Seconds
			{     
		       	EditInfo(&on_sec , 0 , 59 , Range[1] , Disp[2]);
			}

			else if(Key=='4')
			{
				goto EDIT;
			}

			else goto ON;

		case '2':   // OFF TIME
OFF:			CmdLCD(CLEAR_LCD);
			StrLCD("1.Hr 2.Min 3.Sec");
			CmdLCD(GOTO_LINE2_POS0);
			StrLCD("4.Exit");
			Key=(u8)KeyScan();
			delay_ms(200);
			CmdLCD(CLEAR_LCD);
			if(Key=='1')        // Edit OFF Hour
			{					
		       	EditInfo(&off_hour , 0 , 23 , Range[0] , Disp[0]);
			}

			else if(Key=='2')   // Edit OFF Minute
			{
		       	EditInfo(&off_min , 0 , 59 , Range[1] , Disp[1]);
			}

			else if(Key=='3')   // Edit OFF Seconds
			{
		       	EditInfo(&off_sec , 0 , 59 , Range[1] , Disp[2]);
			}

			else if(Key=='4')
			{
				goto EDIT;
			}

			else goto OFF;

			break;

		case '3':	CmdLCD(CLEAR_LCD);
					flag=0;
					return;

		default:
			CmdLCD(CLEAR_LCD);
			StrLCD("INVALID OPTION!");
			delay_ms(1000);     //  if(KeyScan());
			goto EDIT;
	}
	}
}

/* Function: Display_OnOff_Time()
   Purpose : Displays the stored ON/OFF time on LCD
*/
void Display_OnOff_Time(void)
{
	CmdLCD(CLEAR_LCD);
	StrLCD("ON:");
	U32LCD(on_hour);
	CharLCD(':');
	U32LCD(on_min);

	CmdLCD(GOTO_LINE2_POS0);
	StrLCD("OFF:");
	U32LCD(off_hour);
	CharLCD(':');
	U32LCD(off_min);
	delay_s(3);
	flag=0;
}  

void EditInfo(s32 *num , s32 min_val , s32 max_val ,u8 *range , u8 *str)
{
	u8 dummy;
    do
	 {
	 	if(*num>max_val || *num<min_val)			   // Check the Input is within the Range
	 	{
	        CmdLCD(GOTO_LINE1_POS0);
	    	StrLCD((s8*)Msg[1]);     // Invalid msg
	    	StrLCD((s8 *)str);        // Input
	    	CharLCD(':');
	    	CmdLCD(GOTO_LINE2_POS0);
	    	U32LCD(*num);
	    	delay_ms(1000);
	        CmdLCD(CLEAR_LCD);
	    }
	    StrLCD((s8*)Msg[0]);     // Enter msg
	    StrLCD((s8*)str);        // Input
	    StrLCD((s8*)range);      //  Range
	    ReadNum((u32*)&num, (u32*)&dummy);
	}while(*num>max_val || *num<min_val);
}
