#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/spritedispatcher.h"
#include "../inc/joyreader.h"

#include "../inc/game.h"
#include "../inc/hud.h"
#include "../inc/disclaimer.h"
#include "../inc/titleScreen.h"
#include "../inc/splashScreen.h"
#include "../inc/gameOverScreen.h"
#include "../inc/debugScreen.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void initEnviroment( void )
{
	VDP_setScreenWidth320( );
	VDP_setScreenHeight224( );

	VDP_setPlanSize( 64, 64 );

	SPR_init( NULL );
	SPRD_reset( );

	JoyReaderInit( );

	VDP_loadFont( &zosFont, FALSE );

	SYS_setInterruptMaskLevel( 4 );
	//TODO: SYS_setVIntCallback( (_voidCallback*) VIntCallback );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


int main( void )
{
	initEnviroment( );
	//TODO: showDisclaimer( );

	TEST_MODE_FLAG = FALSE;

	while( TRUE )    // main-loop
	{
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
				lvl++;
			}
		}
	}

	return NULL; /* Never Reached */
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
