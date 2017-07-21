#include <genesis.h>

#include "../res/rescomp.h"
#include "../inc/joyreader.h"
#include "../inc/titleScreen.h"
#include "../inc/common.h"
#include "../inc/helpers.h"
#include "../inc/game.h"
#include "../inc/dev.h"
#include "../inc/display.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define COLOR_WHITE ( 0x0EEE )
#define COLOR_SHADOW ( 0x0666 )
#define COLOR_BLACK ( 0x0000 )
#define COLOR_SALMON ( 0x068E )
#define COLOR_DARKSALMON ( 0x0026 )
#define TXT_ATTRIBUTES ( TILE_ATTR( PAL0, FALSE, FALSE, FALSE ) )
#define IMG_ATTRIBUTES ( TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ) )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static const u16 secretCode[ ] =
{
	BUTTON_B, 		BUTTON_A,		BUTTON_B,		BUTTON_A,
	BUTTON_LEFT,	BUTTON_UP,		BUTTON_B,		BUTTON_A,
	NULL
};

static u8 secretCodeIndex;
static u8 isCheatCodeCompleted;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doCheatMessageBlinking( void )
{
    VDP_setTextPalette(PAL1);

	VDP_drawTextBG( PLAN_B, "UNLOCKED", 16, 26 );

	playSfx(SFX_START);

	u8 i;
	for ( i = 0; i < 72; i++ )
	{
		VDP_waitVSync( );

        VDP_setPaletteColor( 30, ( i & 4 ) ? COLOR_BLACK : COLOR_SALMON );
		VDP_setPaletteColor( 31, ( i & 4 ) ? COLOR_BLACK : COLOR_DARKSALMON );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doPressStartFasterBlinking( void )
{
	playSfx(SFX_START);

	u8 i;
	for ( i = 0; i < 72; i++ )
	{
		VDP_waitVSync( );

        VDP_setPaletteColor(  1, ( i & 4 ) ? COLOR_BLACK : COLOR_SHADOW );
		VDP_setPaletteColor( 15, ( i & 4 ) ? COLOR_BLACK : COLOR_WHITE );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void checkForCheatCodeCompleted( void )
{
	isCheatCodeCompleted = ( secretCode[ secretCodeIndex ] == NULL );

	if ( isCheatCodeCompleted )
	{
		doCheatMessageBlinking( );
		playSfx(SFX_BLA1);
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawTitleScreenGraphics( void )
{
    displayOff(0);

    const Image *images[] = { &titleScreenImg1, &titleScreenImg };
    Image *image = (Image*) images[CLASSIC_MODE_FLAG];

    VDP_drawImageEx( PLAN_B, image, IMG_ATTRIBUTES, 0, 0, FALSE, FALSE );

    preparePal ( PAL0, image->palette->data );

    prepareColor (  1, COLOR_BLACK );
    prepareColor ( 15, COLOR_BLACK );
    prepareColor ( 30, COLOR_SALMON );
    prepareColor ( 31, COLOR_DARKSALMON );

    displayOn(10);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


u8 showTitleScreen( void )
{
	secretCodeIndex = 0;
	isCheatCodeCompleted = FALSE;

	drawTitleScreenGraphics( );

	u8 frame = 0;

	do
	{
		VDP_waitVSync( );

		// 'Press Start' Blinking Effect...
		VDP_setPaletteColor(  1, ( frame++ & 32 ) ? COLOR_BLACK : COLOR_SHADOW );
		VDP_setPaletteColor( 15, ( frame++ & 32 ) ? COLOR_BLACK : COLOR_WHITE );
		VDP_setPaletteColor( 31, COLOR_DARKSALMON ); // cheatcode shadow color

		JoyReaderUpdate( );

		if ( PAD_1_PRESSED )
		{
			if ( PAD_1_PRESSED == secretCode[ secretCodeIndex ] )
			{
			    VDP_setTextPalette(PAL1);

				VDP_drawTextBG( PLAN_B, "`", 16 + secretCodeIndex, 26 );

				secretCodeIndex++;
				checkForCheatCodeCompleted( );
			}
			else
			{
				secretCodeIndex = 0;
			}
		}

		if ( DEV )
        {
            drawUInt(secretCodeIndex, 0,0,3);
        }

	} while( !PAD_1_PRESSED_START && !isCheatCodeCompleted );

	if( !isCheatCodeCompleted )
	{
		doPressStartFasterBlinking( );
	}

	displayOff ( 10 );

	return isCheatCodeCompleted;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
