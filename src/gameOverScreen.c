#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/joyreader.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define COLOR_BLUE ( 0x0EA6 )
#define COLOR_BLACK ( 0x0000 )
#define IMG_ATTRIBUTES ( TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ) )
#define IMG_PAL APLAN


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawGameOverScreenGraphics( void )
{
	VDP_waitVSync( );

	VDP_setEnable( FALSE );

		VDP_clearPlan( VDP_PLAN_B, FALSE );
		VDP_drawImageEx( IMG_PAL, &gameOverScreenImg, IMG_ATTRIBUTES, 0, 0, TRUE, FALSE );

	VDP_setEnable( TRUE );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doTextBlinkingEffect( void )
{
	u8 frame;

	for ( frame = 0; frame < 150; frame++ )
	{
		VDP_waitVSync( );
		VDP_setPaletteColor( 1, ( frame & 8 ) ? COLOR_BLACK : COLOR_BLUE );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void waitForShowMessage( void )
{
	u16 frame;

	JoyReaderReset( );

	for ( frame = 0; frame < 500; frame++ )
	{
		VDP_waitVSync( );

		JoyReaderUpdate( );

		if ( PAD_1_PRESSED_START )
		{
			break;
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showGameOverScreen( void )
{
	drawGameOverScreenGraphics( );
	//TODO: music_play(mus_gameover);

	waitForShowMessage( );

	//TODO: music_stop();
	//sfx_play(SFX_START,0);

	doTextBlinkingEffect( );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
