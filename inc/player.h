#ifndef _PLAYER_H_
#define _PLAYER_H_


	// Defines
	#define IS_PLAYER_KNOCKED ( playerKnockedDuration != 0 )
	#define IS_PLAYER_WISDOM ( playerWisdomDuration != 0 )

	// Public Data
    Sprite   *playerSprite;
    s16      playerFixedPositionX;
    s16      playerFixedPositionY;
    u8       isPlayerSlowed;
    u8       playerKnockedDuration;     // Used for knocked AI (DAY 6)
    u8       player_knocked_ani; // Used for knocked Animation (DAY 6)
    u8       player_dir;         // Player current direction
    u8       player_flip;         // Player current orientation
    u16      playerWisdomDuration;      // Used for BONUS purposes
    u8       isPlayerCatched;


    //Public functions
    void    playerReset    ( void );
    void    player_init     ( s16 x, s16 y );
    void    player_step     ( u16 state );


#endif // _PLAYER_H_

