/******  eint.c  ******/

#include<LPC214x.h>
#include "types.h"
#include "eint_defines.h"
#include "pin_connect_block.h"
#include "pin_function_defines.h"

u32 flag=0;

void eint0_isr(void) __irq
{
	flag=1;
		//isr activity starts
	IOPIN0^=1<<EINT0_STATUS_LED;		//isr activity end
			//clear EINT0 status in External Interrupt Peripheral
	EXTINT=1<<0;
			//clear EINT0 status in VIC;
	VICVectAddr=0;
}

void eint1_isr(void) __irq
{
	flag=2;
   	IOPIN0^=1<<EINT1_VIC_CHNO;		//isr activity end
                        //isr activity end
                        //clear EINT1 status in External Interrupt Peripheral
        EXTINT=1<<1;
                        //clear EINT1 status in VIC;
        VICVectAddr=0;
}

void Init_Interrupt(void)
{
	 CfgPortPinFunc(0,1,PIN_FUNC4);
//	 CfgPortPinFunc(0,EINT1_VIC_CHNO,PIN_FUNC4);
	 VICIntSelect =0;
	 VICIntEnable  = 1 << EINT0_VIC_CHNO | (1 << EINT1_VIC_CHNO);
	 VICVectCntl0=(1<<5)|EINT0_VIC_CHNO;
	 VICVectAddr0  = (u32)eint0_isr;
	  EXTMODE |=1<<0;
	 EXTPOLAR=0;
}
