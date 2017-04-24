#define TM4C123GH6PM	1
#include <TM4C123.h>
#include "samples.h"
#include "sound.h"

struct Sound
{
	const unsigned char* samples;	// reference to a sample set
	unsigned long	  		 count;		// count of samples
	S_NAME s_name;								// available sounds enumerator
};

#define NO_SOUNDS 8
struct Sound sounds[NO_SOUNDS] =
{
	{fastinvader4,1098,S_FASTINVADER4},
	{fastinvader3,1054,S_FASTINVADER3},
	{fastinvader2,1042,S_FASTINVADER2},
	{fastinvader1,982,S_FASTINVADER1},
	{invaderkilled,3377,S_INVADERKILLED},
	{explosion,2000,S_EXPLOSION},
	{shoot,4080,S_SHOOT},
	{highpitch,1802,S_HIGHPITCH},	
};


// Establish timer for creating the sampling clock @11khz 
void Timer1A_Init(unsigned long period)
{ 
  unsigned long volatile delay;
  
	SYSCTL->RCGCTIMER |= 0x02;   // 0) activate timer1
	
  delay = SYSCTL->RCGCTIMER;

  TIMER1->CTL = 0x00000000;    // 1) disable timer1A during setup
  TIMER1->CFG = 0x00000000;    // 2) configure for 32-bit mode
  TIMER1->TAMR = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER1->TAILR = period-1;    // 4) reload value
  TIMER1->TAPR = 0;            // 5) bus clock resolution
  TIMER1->ICR = 0x00000001;    // 6) clear timer1A timeout flag
  TIMER1->IMR = 0x00000001;    // 7) arm timeout interrupt

//  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x20000000; // 2) priority 1
	NVIC_SetPriority( TIMER1A_IRQn, 1 );

// vector number 39, interrupt number 23
	NVIC_EnableIRQ( TIMER1A_IRQn );
}

void Timer1A_Stop(void){ 
  TIMER1->CTL &= ~0x00000001; // disable
}
void Timer1A_Start(void){ 
  TIMER1->CTL |= 0x00000001;   // enable
}

// **************DAC_Init*********************
// Initialize 3-bit DAC
// Input: none
// Output: none

void DAC_Init(void){unsigned long volatile delay;
	
  SYSCTL->RCGC2 |= 0x02;		// activate port B
  delay = SYSCTL->RCGC2;    // allow time to finish activating
	
	GPIOB->LOCK = 0x4C4F434;	 // 2) unlock GPIO Port E
	(*(unsigned int*)GPIOB->CR) = 0x07;					 // allow changes to PB4-0

  GPIOB->AMSEL &= ~0x07;      // no analog
  GPIOB->PCTL &= ~0x00000FFF; // regular GPIO function
  GPIOB->DIR |= 0x07;      // make PB2-0 out
  GPIOB->AFSEL &= ~0x07;   // disable alt funct on PB2-0
  GPIOB->DEN |= 0x07;      // enable digital I/O on PB2-0
} 

// **************DAC_Out*********************
// output to DAC
// Input: 3-bit data, 0 to 7
// Output: none

static void DAC_Out(unsigned long data){
  GPIOB->DATA = data;
}

void Sound_Init()
{
	DAC_Init();
	//The bus clock is 80MHz, or 12.5ns/cycle. 
	// To interrupt at 11.025 kHz, the interrupt must occur every 80,000,000/11,025 cycles, 
	// which is about 7256. To make this happen we call Timer_Init with the parameter 7256
	Timer1A_Init(7256);
}

struct {
	unsigned long idx;    					// index into array
	const unsigned char* samples;		// pointer to array
	unsigned long sample_count;			// number of points (0 means no sound)
} cur_sound;

void Sound_Play(S_NAME sound_name)
{
	int i;
	// setup sound requested
	for(i=0; i < NO_SOUNDS; i++)
		if(sound_name == sounds[i].s_name) 
			{
				cur_sound.samples = sounds[i].samples;
				cur_sound.sample_count = sounds[i].count;
				cur_sound.idx = 0;
				Timer1A_Start();
				break;
			}
}

// function defined in "startup_TM4C123.s" 
void TIMER1A_Handler(void)
{
  TIMER1->ICR = 0x00000001;  // acknowledge

	// samples remain ?
	if(cur_sound.sample_count)
	{
		// push sample to output
		DAC_Out(cur_sound.samples[cur_sound.idx]>>4); 
		// get ready to pick the next sample
		cur_sound.idx++;
		// reduce sample remaining by 1
		cur_sound.sample_count--;
	}
	else
		// samples finished hence stop the music
		Timer1A_Stop();
}

