#ifndef __SOUND__
	#define __SOUND__
	
typedef enum {
	S_FASTINVADER3,
	S_FASTINVADER2,
	S_FASTINVADER1,
	S_INVADERKILLED,
	S_EXPLOSION,
	S_SHOOT
} S_NAME;

void Sound_Init(void );
void Sound_Play(S_NAME sound_name);

#endif
