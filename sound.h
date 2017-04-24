#ifndef __SOUND__
	#define __SOUND__

// the 256 comes from the length of the sine wave table
// bus is 80 MHz
// Period =  80000000/16/Freq=5000000/Freq
#define C1   2389   // 2093 Hz
#define B1   2531   // 1975.5 Hz
#define BF1  2681   // 1864.7 Hz
#define A1   2841   // 1760 Hz
#define AF1  3010   // 1661.2 Hz
#define G1   3189   // 1568 Hz
#define GF1  3378   // 1480 Hz
#define F1   3579   // 1396.9 Hz
#define E1   3792   // 1318.5 Hz
#define EF1  4018   // 1244.5 Hz
#define D1   4257   // 1174.7 Hz
#define DF1  4510   // 1108.7 Hz
#define C    4778   // 1046.5 Hz
#define B    5062   // 987.8 Hz
#define BF   5363   // 932.3 Hz
#define A    5682   // 880 Hz
#define AF   6020   // 830.6 Hz
#define G    6378   // 784 Hz
#define GF  6757   // 740 Hz
#define F   7159   // 698.5 Hz
#define E   7584   // 659.3 Hz
#define EF  8035   // 622.3 Hz
#define D   8513   // 587.3 Hz
#define DF  9019   // 554.4 Hz
#define C0  9556   // 523.3 Hz
#define B0  10124   // 493.9 Hz
#define BF0 10726   // 466.2 Hz
#define A0  11364   // 440 Hz
#define AF0 12039   // 415.3 Hz
#define G0  12755   // 392 Hz
#define GF0 13514   // 370 Hz
#define F0  14317   // 349.2 Hz
#define E0  15169   // 329.6 Hz
#define EF0 16071   // 311.1 Hz
#define D0  17026   // 293.7 Hz
#define DF0 18039   // 277.2 Hz
#define C7  19111   // 261.6 Hz
#define B7  20248   // 246.9 Hz
#define BF7 21452   // 233.1 Hz
#define A7  22727   // 220 Hz
#define AF7 24079   // 207.7 Hz
#define G7  25511   // 196 Hz
#define GF7 27027   // 185 Hz
#define F7  28635   // 174.6 Hz
#define E7  30337   // 164.8 Hz
#define EF7 32141   // 155.6 Hz
#define D7  34052   // 146.8 Hz
#define DF7 36077   // 138.6 Hz
#define C6  38223   // 130.8 Hz

typedef enum {
	S_FASTINVADER4,
	S_FASTINVADER3,
	S_FASTINVADER2,
	S_FASTINVADER1,
	S_INVADERKILLED,
	S_EXPLOSION,
	S_SHOOT,
	S_HIGHPITCH
} S_NAME;

void Sound_Init(void );
void Sound_Play(S_NAME sound_name);

#endif
