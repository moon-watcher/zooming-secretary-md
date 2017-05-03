#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/joyreader.h"
#include "../inc/common.h"
#include "../inc/music.h"
#include "../inc/helpers.h"
#include "../inc/game.h"
#include "../inc/display.h"


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
	displayOff(0);

		VDP_clearPlan( PLAN_B, 1 );
		VDP_drawImageEx( IMG_PAL, &gameOverScreenImg, IMG_ATTRIBUTES, 0, 0, 0, FALSE );

        preparePal ( PAL0, gameOverScreenImg.palette->data );

		color1 = gameOverScreenImg.palette->data[7];
		color2 = gameOverScreenImg.palette->data[6];

        prepareColor ( 7, COLOR_BLACK );
		prepareColor ( 6, COLOR_BLACK );

	displayOn(10);
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

    displayOff(10);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
