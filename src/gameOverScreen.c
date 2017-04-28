#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/joyreader.h"
#include "../inc/common.h"
#include "../inc/music.h"
#include "../inc/helpers.h"
#include "../inc/game.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define COLOR_BLUE ( 0x0EA6 )
#define COLOR_BLACK ( 0x0000 )
#define IMG_ATTRIBUTES ( TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ) )
#define IMG_PAL PLAN_A




/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u16 color1;
static u16 color2;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawGameOverScreenGraphics( void )
{
	VDP_waitVSync( );

	VDP_setEnable( FALSE );

		VDP_clearPlan( PLAN_B, 1 );
		VDP_drawImageEx( IMG_PAL, &gameOverScreenImg, IMG_ATTRIBUTES, 0, 0, TRUE, FALSE );

		color1 = VDP_getPaletteColor(7);
		color2 = VDP_getPaletteColor(6);

        VDP_setPaletteColor( 7, COLOR_BLACK );
		VDP_setPaletteColor( 6, COLOR_BLACK );

	VDP_setEnable( TRUE );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doTextBlinkingEffect( void )
{
	u8 frame;

	for ( frame = 0; frame < 150; frame++ )
	{
		VDP_waitVSync( );
		VDP_setPaletteColor( 7, ( frame & 8 ) ? COLOR_BLACK : color1 );
		VDP_setPaletteColor( 6, ( frame & 8 ) ? COLOR_BLACK : color2 );
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

		if ( PAD_1_PRESSED_START | PAD_1_PRESSED_BTN )
		{
			break;
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showGameOverScreen( void )
{
	drawGameOverScreenGraphics( );
	playMusic(MUSIC_GAMEOVER);

	waitForShowMessage( );

	musicStop();
	playSfx(SFX_START);

    doTextBlinkingEffect( );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
