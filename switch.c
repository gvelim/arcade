#define TM4C123GH6PM	1
#include <TM4C123.h>
#include "switch.h"

// ******* Required Hardware I/O connections*******************
// Left button connected to PE3
// Right button connected to PE2
// fire button connected to PE1


#define PA5	 (*((volatile unsigned long *)0x40004080))


void Switch_Init(void)
{ 
	volatile unsigned long delay;
	SYSCTL->RCGC2 |= 0x00000010;	// 1) activate clock for Port E
	delay = SYSCTL->RCGC2;				// allow time for clock to start
			
	GPIOE->LOCK = 0x4C4F434B;	 // 2) unlock GPIO Port E
	(*(unsigned int*)GPIOE->CR) = 0x1F;					 // allow changes to PF4-0

	
	GPIOE->AMSEL &= ~0x0E;				// 3) disable analog on PE(1,2,3)
	GPIOE->PCTL &= ~0x0000FFF0; 	// 4) PCTL GPIO on PE(1,2,3)
	GPIOE->DIR &= ~0x0E;					// 5) direction PA5 input
	GPIOE->AFSEL &= ~0x0E;				// 6) PE(1,2,3) regular port function
	GPIOE->DEN |= 0x0E;				 	// 7) enable PA5 digital port
}

unsigned long Switch_Input(void)
{
	//return PA5; 
	return GPIOE->DATA; // return 0x20(pressed) or 0(not pressed)
}

unsigned char isLeftPressed(void)
{
	return (GPIOE->DATA&SW_LEFT); // 0x20(pressed) or 0(not pressed)
}

unsigned char isRightPressed(void)
{
	return (GPIOE->DATA&SW_RIGHT); // 0x20(pressed) or 0(not pressed)
}

unsigned char isFirePressed(void)
{
	return (GPIOE->DATA&SW_FIRE); // 0x20(pressed) or 0(not pressed)
}

unsigned long waitForFire(void)
{
	while(!isFirePressed());	// 0x20(pressed) or 0(not pressed)
	while(isFirePressed());
	return 0x1; 
}

unsigned long waitForAnyButton(void)
{
	while(!GPIOE->DATA & (SW_FIRE|SW_RIGHT|SW_LEFT));	// 0x20(pressed) or 0(not pressed)
	return GPIOE->DATA & (SW_FIRE|SW_RIGHT|SW_LEFT);
}
