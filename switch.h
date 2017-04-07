#ifndef __SWITCH__
	#define __SWITCH__

#define SW_FIRE		0x02 // pin 2
#define SW_LEFT		0x08 // pin 4
#define SW_RIGHT	0x04 // pin 3


void Switch_Init(void);
unsigned long Switch_Input(void);
unsigned char isLeftPressed(void);
unsigned char isRightPressed(void);
unsigned char isFirePressed(void);
unsigned long waitForFire(void);
unsigned long waitForAnyButton(void);
	
#endif
