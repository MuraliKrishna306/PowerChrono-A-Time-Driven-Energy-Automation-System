/***	powerchrono.h	***/
#include "types.h"

#define DEVICE_PIN 5	//p0.5

void EditInfo(s32 *num , s32 max_val , s32 min_val ,u8 *range , u8 *str);
void DisplayRTCInfo(void);
void DisplayMenu(void);
void Edit_RTC_Time(void);
void Edit_OnOff_Time(void);
void Display_OnOff_Time(void);
