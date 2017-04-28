#ifndef _HUD_H_
#define _HUD_H_

    #define PLAYABLE_LEVELS 8     //Amount of playable levels (excl. Weekend)
    #define BONUS_LEVEL_INDEX   4
	#define IS_LEVEL_BONUS ( lvl == BONUS_LEVEL_INDEX)

    u8 lvl;          // Current level
    u8 call_max;     // Total of answered calls needed to hit the current level
    u8 totalOfMissedCalls;
    u8 totalOfAnsweredCalls;
    u8 miss_max;     // Maximum of missed calls allowed
    u8 BONUS_FLAG;   //Used for computing extra missed calls bonus

    // Public functions
    void hudReset( void );
    void hudInitialize( void );
    void hudUpdate( void );

#endif // _HUD_H_


