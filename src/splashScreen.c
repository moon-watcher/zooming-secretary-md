#include <genesis.h>

#include "../res/rescomp.h"
#include "../inc/joyreader.h"
#include "../inc/hud.h"
#include "../inc/game.h"
#include "../inc/common.h"
#include "../inc/helpers.h"
#include "../inc/dev.h"
#include "../inc/display.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define COLOR_WHITE ( 0x0EEE )
#define COLOR_GREY ( 0x0666 )
#define COLOR_BLUE ( 0x0EA0 )
#define COLOR_NAVYBLUE ( 0x0E20 )


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
	VDP_clearPlan( PLAN_B, 1 );
	VDP_clearPlan( PLAN_A, 1 );

	prepareColor ( 12, COLOR_WHITE );
	prepareColor ( 13, COLOR_GREY );
	prepareColor ( 14, COLOR_BLUE );
	prepareColor ( 15, COLOR_NAVYBLUE );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawLevelName( void )
{
    VDP_setTextPalette(PAL0);
	VDP_drawTextBG( PLAN_A, levelNames[ lvl ], 17, 12 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showSplashScreen( void )
{
	displayOff(0);

	prepareScreen( );
	drawLevelName( );

	displayOn(10);

	if ( LEVEL_MODE_FLAG )
	{
		JoyReaderReset( );

		do
		{
			VDP_waitVSync( );

			JoyReaderUpdate( );

			if ( PAD_1_PRESSED_LEFT && ( lvl > 0 ) )
			{
                lvl--;
                drawLevelName( );
			}

			else if ( PAD_1_PRESSED_RIGHT && ( lvl < PLAYABLE_LEVELS ) )
            {
                lvl++;
                drawLevelName( );
			}

		} while( !PAD_1_PRESSED_BTN );
	}

	if ( lvl < PLAYABLE_LEVELS )
	{
		playMusic (MUSIC_LEVEL);
	}

	if ( !DEV )
    {
        waitSc(1);
    }

    musicStop();

    displayOff(10);
    VDP_waitVSync();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
