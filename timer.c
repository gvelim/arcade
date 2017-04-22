#define TM4C123GH6PM	1
#include <TM4C123.h>


void (*PeriodicTask)(void);   // user function

// ***************** Timer2_Init ****************
// Activate Timer2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2_Init(void(*task)(void), unsigned long period)
{ 
  unsigned long volatile delay;
  
	// page 338
	SYSCTL->RCGCTIMER |= 0x04;   // 0) activate timer2
	
  delay = SYSCTL->RCGCTIMER;

  PeriodicTask = task;          // user function

  TIMER2->CTL = 0x00000000;    // 1) disable timer2A during setup
  TIMER2->CFG = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2->TAMR = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2->TAILR = period-1;    // 4) reload value
  TIMER2->TAPR = 0;            // 5) bus clock resolution
  TIMER2->ICR = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER2->IMR = 0x00000001;    // 7) arm timeout interrupt

//  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
	NVIC_SetPriority( TIMER2A_IRQn, 4 );

// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
//  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
	NVIC_EnableIRQ( TIMER2A_IRQn );
}

void TIMER2A_Handler(void){
  TIMER2->ICR = 0x00000001;  // acknowledge
	
// run some background stuff here
  (*PeriodicTask)(); 
}
void Timer2A_Stop(void){ 
  TIMER2->CTL &= ~0x00000001; // disable
}
void Timer2A_Start(void){ 
  TIMER2->CTL |= 0x00000001;   // enable
}
