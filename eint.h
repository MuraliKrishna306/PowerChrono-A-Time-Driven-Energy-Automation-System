/******  eint.h  ******/

#include "types.h"
void Init_Interrupt(void);
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

void DisplayRTCInfo(void);
void DisplayMenu(void);
void Edit_RTC_Time(void);
void Edit_OnOff_Time(void);
void Display_OnOff_Time(void);

