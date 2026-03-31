#include <LPC21xx.h>

#include "types.h"
#include "lcd.h"
#include "lcd_defines.h"
#include "delay.h"
#include "eint.h"
#include "kpm.h"
#include "eint_defines.h"

#include "pin_function_defines.h"

#include "pin_connect_block.h"


u32 gcount;
u32 flag=0;


void Init_Interrupt(void)

{

   //cfg p0.1 pin as EINT0 input pin

   CfgPortPinFunc(0,1,PIN_FUNC4);

   //cfg VIC peripheral

   //ccfg EINT0 as irq types,default all are IRQ types

   //VICIntSelect=0;

   //Enable EINT0 via VIC

   VICIntEnable=1<<EINT0_VIC_CHNO;

   //cfg EINT0 as v.irq with highest priority and allow to load eint_isr addr

   VICVectCntl0=(1<<5)|EINT0_VIC_CHNO;

   //load ein0_isr addr

   VICVectAddr0=(u32)eint0_isr;

   //cfg external interrupt peripheral allow/enable EINT0,default all are enable

   //EXTINT=0;

   //cfg EINT0 as falling edge triggered

   //default all are falling edges

   //EXIPOLAR=0;

}

void eint0_isr(void)__irq

{
	flag=1;
   //isr activity starts

   IOPIN0^=1<<EINT0_STATUS_LED;

   //isr activity end


   //clear EINT0 status in External Interrupt peripherial

   EXTINT=1<<0;


   //clear EINT0 status in VIC

   VICVectAddr=0;

}
