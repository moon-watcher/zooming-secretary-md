#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/joyreader.h"
#include "../inc/hud.h"
#include "../inc/game.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define COLOR_WHITE ( 0x0EEE )
#define COLOR_GREY ( 0x0666 )
#define COLOR_BLUE ( 0x0EA0 )
#define COLOR_NAVYBLUE ( 0x0E20 )

#define SPLASH_PALETTE PAL0
#define SPLASH_PAL APLAN
#define SPLASH_ATTRIBUTES ( TILE_ATTR( SPLASH_PALETTE, FALSE, FALSE, FALSE ) )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


const char *levelNames[ ] =
{
	" DAY:1 ", " DAY:2 ", " DAY:3 ",
	" DAY:4 ", " DREAM ", " DAY:5 ",
	" DAY:6 ", " DAY:7 ", "WEEKEND"
};


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

static void prepareScreen( void )
{
	VDP_waitVSync( );

	VDP_clearPlan( VDP_PLAN_B, FALSE );
	VDP_clearPlan( VDP_PLAN_A, FALSE );

	const u16 colors[ ] = { COLOR_WHITE, COLOR_GREY, COLOR_BLUE, COLOR_NAVYBLUE };
	VDP_setPaletteColors(12, (u16 *) colors , 4);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawLevelName( void )
{
	VDP_drawTextBG( APLAN, levelNames[ lvl ], SPLASH_ATTRIBUTES, 17, 12 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

void showSplashScreen( void )
{
	prepareScreen( );
	drawLevelName( );

	if ( TEST_MODE_FLAG )
	{
		JoyReaderReset( );

		do
		{
			VDP_waitVSync( );

			JoyReaderUpdate( );

			if ( PAD_1_PRESSED_LEFT )
			{
				if ( lvl > 0 )
				{
					lvl--;
					drawLevelName( );
				}
			}

			if ( PAD_1_PRESSED_RIGHT )
			{
				if ( lvl < PLAYABLE_LEVELS )
				{
					lvl++;
					drawLevelName( );
				}
			}

		} while( !PAD_1_PRESSED_BTN );
	}

	if ( lvl < PLAYABLE_LEVELS )
	{
		//TODO: music_play(mus_level);
	}

	u8 i;
	for ( i = 0; i < 50 * 3; i++ )
	{
		VDP_waitVSync( );
	}
}
