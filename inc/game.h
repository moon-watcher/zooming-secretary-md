#ifndef _GAME_H_
#define _GAME_H_


    // Used for 'Level Select Cheat'
    u8 GOD_MODE_FLAG;
    u8 LEVEL_MODE_FLAG;
    u8 EXIT_MODE_FLAG;
    u8 CLASSIC_MODE_FLAG;
    u8 MUSIC_MODE_FLAG;
	u8 SFX_MODE_FLAG;
	u8 DEMO_MODE_FLAG;


    // Public Functions
    u8 game_play( void );
    void game_done( void );


#endif // _GAME_H_
