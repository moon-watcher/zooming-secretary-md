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
#define SPRITE_X  ( 8 + 2 ) * 8
#define SPRITE_Y  ( 2 + 5 ) * 8


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u16 color1;
static u16 color2;
static Sprite *sprFired;
static Sprite *sprHired;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawGameOverScreenGraphics( void )
{
	displayOff(0);

        resetScreen();
        SYS_disableInts();
		VDP_drawImageEx( IMG_PAL, &gameOverScreenImg, IMG_ATTRIBUTES, 0, 0, 0, FALSE );
		SYS_enableInts();

        preparePal ( PAL0, gameOverScreenImg.palette->data );

		color1 = gameOverScreenImg.palette->data[7];
		color2 = gameOverScreenImg.palette->data[6];

        prepareColor ( 7, COLOR_BLACK );
		prepareColor ( 6, COLOR_BLACK );

        if ( !CLASSIC_MODE_FLAG )
        {
            SYS_disableInts();
            VDP_drawImageEx( IMG_PAL, &gameOverScreenImg, IMG_ATTRIBUTES, 0, 8, 0, 0 );
            VDP_drawImageEx( PLAN_B, &firedBgImg, TILE_ATTR_FULL( PAL1, 0, 0, 0, 100 ), 8, 2, 0, 0 );
            VDP_drawImageEx( PLAN_A, &firedFgImg, TILE_ATTR_FULL( PAL2, 1, 0, 0, 500 ),12, 2, 0, 0 );
            SYS_enableInts();

            preparePal ( PAL1, firedBgImg.palette->data );
            preparePal ( PAL2, firedFgImg.palette->data );

            VDP_loadTileSet ( hiredBgImg.tileset,  90, 0 );
            VDP_loadTileSet ( hiredFgImg.tileset, 750, 0 );

            SPR_init(0,0,0);
            sprFired = SPR_addSprite( (SpriteDefinition*) &firedSprDef, SPRITE_X, SPRITE_Y, TILE_ATTR(PAL3,0,0,0) );
            sprHired = SPR_addSprite( (SpriteDefinition*) &hiredSprDef,        0,      -50, TILE_ATTR(PAL3,0,0,0) );
            preparePal( PAL3, firedSprDef.palette->data);
            SPR_update();
            VDP_waitVSync();
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

    VDP_setMap(PLAN_B, hiredBgImg.map, TILE_ATTR_FULL( PAL1, 1, 0, 0,  90 ), 15, 16);
    VDP_setMap(PLAN_A, hiredFgImg.map, TILE_ATTR_FULL( PAL2, 1, 0, 0, 750 ), 12,  2);
    VDP_setPalette( PAL1, hiredBgImg.palette->data );
    VDP_setPalette( PAL2, hiredFgImg.palette->data );

    SPR_setPosition ( sprFired,        0,      -50 );
    SPR_setPosition ( sprHired, SPRITE_X, SPRITE_Y );

    SYS_enableInts();

    SPR_update();
    VDP_waitVSync();
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
    SPR_clear();
    SPR_reset();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
