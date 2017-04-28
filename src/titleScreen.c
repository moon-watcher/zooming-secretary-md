#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/joyreader.h"
#include "../inc/titleScreen.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define COLOR_WHITE ( 0x0EEE )
#define COLOR_BLACK ( 0x0000 )
#define COLOR_SALMON ( 0x068E )
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
	VDP_drawTextBG( BPLAN, "UNLOCKED", TXT_ATTRIBUTES, 16, 26 );

	//TODO: sfx_play(SFX_START,0);

	u8 i;
	for ( i = 0; i < 72; i++ )
	{
		VDP_waitVSync( );

		VDP_setPaletteColor( 2, ( i & 4 ) ? COLOR_BLACK : COLOR_SALMON );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doPressStartFasterBlinking( void )
{
	//TODO: sfx_play(SFX_START,0);

	u8 i;
	for ( i = 0; i < 72; i++ )
	{
		VDP_waitVSync( );

		VDP_setPaletteColor( 1, ( i & 4 ) ? COLOR_WHITE : COLOR_BLACK );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void checkForCheatCodeCompleted( void )
{
	isCheatCodeCompleted = ( secretCode[ secretCodeIndex ] == NULL );

	if ( isCheatCodeCompleted )
	{
		doCheatMessageBlinking( );
		//TODO: sfx_play(SFX_BLA1,1);
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawTitleScreenGraphics( void )
{
	VDP_setEnable( FALSE );

		VDP_drawImageEx( BPLAN, &titleScreenImg, IMG_ATTRIBUTES, 0, 0, TRUE, FALSE );

	VDP_setEnable( TRUE );
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

		JoyReaderUpdate( );

		// 'Press Start' Blinking Effect...
		VDP_setPaletteColor( 1, ( frame++ & 32 ) ? COLOR_BLACK : COLOR_WHITE );


		if ( PAD_1_PRESSED )
		{
			if ( PAD_1_ACTIVE == secretCode[ secretCodeIndex ] )
			{
				VDP_drawTextBG( BPLAN, "`", TXT_ATTRIBUTES, 16 + secretCodeIndex, 26 );

				secretCodeIndex++;
				checkForCheatCodeCompleted( );
			}
			else
			{
				secretCodeIndex = 0;
			}
		}

	} while( !PAD_1_PRESSED_START && !isCheatCodeCompleted );

	if( !isCheatCodeCompleted )
	{
		doPressStartFasterBlinking( );
	}

	return isCheatCodeCompleted;
}

