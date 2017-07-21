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

        resetScreen();
		VDP_drawImageEx( IMG_PAL, &gameOverScreenImg, IMG_ATTRIBUTES, 0, 0, 0, FALSE );

        preparePal ( PAL0, gameOverScreenImg.palette->data );

		color1 = gameOverScreenImg.palette->data[7];
		color2 = gameOverScreenImg.palette->data[6];

        prepareColor ( 7, COLOR_BLACK );
		prepareColor ( 6, COLOR_BLACK );

        if ( !CLASSIC_MODE_FLAG )
        {
            VDP_drawImageEx( IMG_PAL, &gameOverScreenImg, IMG_ATTRIBUTES, 0, 8, 0, FALSE );
            VDP_drawImageEx( PLAN_B, &firedBgImg, TILE_ATTR_FULL( PAL1, FALSE, FALSE, FALSE, 100 ), 8, 2, 0, FALSE );
            VDP_drawImageEx( PLAN_A, &firedFgImg, TILE_ATTR_FULL( PAL2, FALSE, FALSE, FALSE, 500 ),12, 2, 0, FALSE );

            preparePal ( PAL1, firedBgImg.palette->data );
            preparePal ( PAL2, firedFgImg.palette->data );

            VDP_loadTileSet ( hiredBgImg.tileset,  90, 0 );
            VDP_loadTileSet ( hiredFgImg.tileset, 750, 0 );
        }

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

		if ( PAD_1_PRESSED_ABCS )
		{
			break;
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void hiredAgain()
{
    if ( CLASSIC_MODE_FLAG )
    {
        return;
    }

    VDP_waitVSync();
    SYS_disableInts();

    VDP_setMap(PLAN_B, hiredBgImg.map, TILE_ATTR_FULL( PAL1, FALSE, FALSE, FALSE,  90 ), 15, 16);
    VDP_setMap(PLAN_A, hiredFgImg.map, TILE_ATTR_FULL( PAL2, FALSE, FALSE, FALSE, 750 ), 12,  2);
    VDP_setPalette( PAL1, hiredBgImg.palette->data );
    VDP_setPalette( PAL2, hiredFgImg.palette->data );

    SYS_enableInts();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showGameOverScreen( void )
{
	drawGameOverScreenGraphics( );
	playMusic(MUSIC_GAMEOVER);

	waitForShowMessage( );

	musicStop();
	playSfx(SFX_START);

    hiredAgain();
    doTextBlinkingEffect( );

    displayOff(10);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
