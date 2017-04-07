#ifndef __TIMER2A__
	#define __TIMER2A__
	
void Timer2_Init(void(*task)(void), unsigned long period);
void Timer2A_Stop(void);
void Timer2A_Start(void);

#endif
