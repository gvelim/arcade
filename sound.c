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

//***************************************************
// The sounds order should match the S_NAME ENUM order
// as the ENUM values are used for array indexes 
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

struct SoundChannel {
	unsigned long idx;    					// index into array
	const unsigned char* samples;		// pointer to array
	unsigned long sample_count;			// number of points (0 means no sound)
};

struct SoundChannel	cur_sound[MAX_CHANNELS] =
{
	{0,0,0},	// channel 1, use for bg game sound
	{0,0,0},	// channel 2, use for laser and ship sounds
	{0,0,0},	// channel 3, use for alien sounds
	{0,0,0},	// channel 4, not used
};

void Sound_Init()
{
	DAC_Init();
	//The bus clock is 80MHz, or 12.5ns/cycle. 
	// To interrupt at 11.025 kHz, the interrupt must occur every 80,000,000/11,025 cycles, 
	// which is about 7256. To make this happen we call Timer_Init with the parameter 7256
	
	// as we have 4 channels go round in 11Khz each, the interrup should be 7256/4
	Timer1A_Init(7256/MAX_CHANNELS);
}

void Sound_Play(S_NAME sound_name, SC_CHANNEL channel)
{
	if( sound_name > S_HIGHPITCH ) return;
	if( channel > MAX_CHANNELS-1 ) return;
	
	// setup sound requested
	cur_sound[channel].samples = sounds[sound_name].samples;
	cur_sound[channel].sample_count = sounds[sound_name].count;
	cur_sound[channel].idx = 0;

	Timer1A_Start();
}

// function defined in "startup_TM4C123.s" 
void TIMER1A_Handler(void)
{
	static unsigned char cidx=0, framecount=0;
	
  TIMER1->ICR = 0x00000001;  // acknowledge

	// select active channel for connecting to output; should be 0,1,2,3
	cidx = framecount++ % MAX_CHANNELS; 
	
	// samples remain on active channel?
	if(cur_sound[cidx].sample_count)
	{
		// push sample to output
		DAC_Out(cur_sound[cidx].samples[cur_sound[cidx].idx]>>4); 
		// get ready to pick the next sample
		cur_sound[cidx].idx++;
		// reduce sample remaining by 1
		cur_sound[cidx].sample_count--;
	}
	
	// if all channels counters are zero then no more sound to play
	if( cur_sound[0].sample_count+
			cur_sound[1].sample_count+
			cur_sound[2].sample_count+
			cur_sound[3].sample_count == 0 )
	{
		// samples finished hence stop the music
		Timer1A_Stop();
	}
	
}

