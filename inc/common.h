#ifndef _COMMON_H_
#define _COMMON_H_

// Directions available in-game
typedef enum
{
   DIR_RIGHT    = 0,
   DIR_LEFT     = 1,
   DIR_UP       = 2,
   DIR_DOWN     = 3,
   DIR_ALL      = 4,
   DIR_NONE     = 5

} dir;

//TODO: Implement this in SGDK (pull request vía twitter)
#define ABS(n) ( ((n)<0) ? (0-(n)) : (n) )

//Used for simulating float type
#define FP_BITS 4

// Tile to Pixel
#define TILE_BITS 3



enum
{
	SFX_ANSWER,
	SFX_BLA1,
	SFX_BLA2,
	SFX_COFFEE,
	SFX_COFFEE_READY,
	SFX_EXPLODE,
	SFX_FALL,
	SFX_KNOCK,
	SFX_LOSE,
	SFX_PAUSE,
	SFX_RING,
	SFX_START,
	SFX_STEP_1,
	SFX_STEP_2,
	SFX_STEP_3,
	SFX_STEP_4,
    SFX_TELEPORT,
    SFX_TOPIC,
    SFX_WISDOM,

	SFX_MAX
};


enum
{
	MUSIC_NULL,
	MUSIC_GAME,
	MUSIC_DREAM,
	MUSIC_CLEAR,
	MUSIC_NOBONUS,
	MUSIC_LEVEL,
	MUSIC_GAMEOVER,
	MUSIC_WELLDONE,

	MUSIC_MAX
};



#endif // _COMMON_H_
