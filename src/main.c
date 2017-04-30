#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/spritedispatcher.h"
#include "../inc/joyreader.h"

#include "../inc/dev.h"
#include "../inc/game.h"
#include "../inc/hud.h"
#include "../inc/disclaimer.h"
#include "../inc/titleScreen.h"
#include "../inc/splashScreen.h"
#include "../inc/gameOverScreen.h"
#include "../inc/debugScreen.h"
#include "../inc/segaScreen.h"
#include "../inc/creditsScreen.h"
#include "../inc/playonretroScreen.h"
#include "../inc/music.h"
#include "../inc/sfx.h"
#include "../inc/helpers.h"
#include "../inc/tempo.h"
#include "../inc/psg.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static _voidCallback *VIntCallback ( )
{
    psg_callback ( );
	XGM_setMusicTempo ( getMusicTempo() );

	return 0;
}


static void initDisplay ( )
{
    VDP_waitVSync();

	VDP_resetScreen();
	VDP_setPlanSize ( 64, 64 );
}


static void initEnviroment( void )
{
    initDisplay ( );

	sfxInit ( );
	initSfx ( );
	musicInit();
	SPR_init( 80, 0, 0 );
	SPRD_reset( );
	initMusicTempo ();

	JoyReaderInit( );

	VDP_loadFont( &zosFont, FALSE );

	SYS_setInterruptMaskLevel( 4 );
	SYS_setVIntCallback ( (_voidCallback*) VIntCallback );

	DEV             = 1; // 0;

	GOD_MODE_FLAG   = 1; // 0;
    LEVEL_MODE_FLAG = 1; // 0;
	EXIT_MODE_FLAG  = 1; // 0;
	MUSIC_MODE_FLAG = 0; // 1;
	SFX_MODE_FLAG   = 1; // 1;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


//#include "../res/psg/step1.h"
//#include "../res/psg/step2.h"
void psgtest()
{
//    psg_play( (u8*) step1_data, 0 ); waitSc(3);
}

int main( void )
{
	initEnviroment( );
	psgtest();

    if ( !DEV )
    {
        showDisclaimer ();
        //showSega();
    }

	while( TRUE )    // main-loop
	{
	    initDisplay ( );
	    initMusicTempo ();

        if ( !DEV )
        {
            showPlayonretro ( );
            showCredits ();
        }

		u8 isCheatCodeActivated;

		do
		{
			isCheatCodeActivated = showTitleScreen( );

			if( isCheatCodeActivated )
			{
				showDebugScreen( );
			}

		} while( isCheatCodeActivated );


		hudReset( );
		while( lvl <= PLAYABLE_LEVELS )    // Incl. weekend
		{
			showSplashScreen( );

			if ( lvl < PLAYABLE_LEVELS )
			{
				u8 isLevelCompleted = game_play( );

				if ( isLevelCompleted == 255 )
                {
                    break;
                }

				if ( isLevelCompleted || IS_LEVEL_BONUS )
				{
					lvl++;
				}
				else
				{
					showGameOverScreen( );
				}
			}
			else    // Wow! Weekend
			{
				game_done( );
                break;
			}
		}
	}

	return NULL; /* Never Reached */
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
