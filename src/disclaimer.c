#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/disclaimer.h"

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IMG_ATTRIBUTES( palette ) TILE_ATTR_FULL( palette, FALSE, FALSE, FALSE, tileIndex )

#define TEXTDISCLAIMER_FADESTEPS 8
#define TEXTDISCLAIMER_NUMCOLORS 3

#define DISCLAIMERBG_FADESTEPS 7
#define DISCLAIMERBG_NUMCOLORS 12

#define DISCLAIMERLOGO_FADESTEPS 8
#define DISCLAIMERLOGO_NUMCOLORS 13

#define DISCLAIMERLOGOLED_FADESTEPS 12
#define DISCLAIMERLOGOLED_NUMCOLORS 6


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static const u16 textDisclaimerFadeColors[ TEXTDISCLAIMER_FADESTEPS ][ TEXTDISCLAIMER_NUMCOLORS ] =
{
		{ 0x0000, 0x0000, 0x0000 },
		{ 0x0222, 0x0222, 0x0222 },
		{ 0x0444, 0x0444, 0x0444 },
		{ 0x0666, 0x0666, 0x0666 },
		{ 0x0888, 0x0888, 0x0888 },
		{ 0x0AAA, 0x0AAA, 0x0AAA },
		{ 0x0CCC, 0x0CCC, 0x0AAA },
		{ 0x0EEE, 0x0CCC, 0x0AAA }
};

static const u16 disclaimerBGFadeColors[ DISCLAIMERBG_FADESTEPS ][ DISCLAIMERBG_NUMCOLORS ] =
{
		{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x0040, 0x0020, 0x0000, 0x0000, 0x0000, 0x0000, 0x0004, 0x0002, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x0060, 0x0040, 0x0020, 0x0000, 0x0000, 0x0000, 0x0006, 0x0004, 0x0002, 0x0000, 0x0000, 0x0000 },
		{ 0x0080, 0x0060, 0x0040, 0x0020, 0x0000, 0x0000, 0x0008, 0x0006, 0x0004, 0x0002, 0x0000, 0x0000 },
		{ 0x00A0, 0x0080, 0x0060, 0x0040, 0x0020, 0x0000, 0x000A, 0x0008, 0x0006, 0x0004, 0x0002, 0x0000 },
		{ 0x00C0, 0x00A0, 0x0080, 0x0060, 0x0040, 0x0020, 0x000C, 0x000A, 0x0008, 0x0006, 0x0004, 0x0002 },
};

static const u16 disclaimerLogoFadeColors[ DISCLAIMERLOGO_FADESTEPS ][ DISCLAIMERLOGO_NUMCOLORS ] =
{
		{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0080, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x000A, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00A0, 0x0080, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x000C, 0x000A, 0x0008, 0x0000, 0x0000, 0x0000, 0x0000, 0x00C0, 0x00A0, 0x0080, 0x0000, 0x0000, 0x0000 },
		{ 0x000E, 0x000C, 0x000A, 0x0008, 0x0000, 0x0000, 0x0000, 0x00E0, 0x00C0, 0x00A0, 0x0080, 0x0000, 0x0000 },
		{ 0x022E, 0x000E, 0x000C, 0x000A, 0x0008, 0x0000, 0x0000, 0x04E4, 0x00E0, 0x00C0, 0x00A0, 0x0080, 0x0000 },
		{ 0x044E, 0x022E, 0x000E, 0x000C, 0x000A, 0x0008, 0x0000, 0x06E6, 0x04E4, 0x00E0, 0x00C0, 0x00A0, 0x0080 },
		{ 0x066E, 0x044E, 0x022E, 0x000E, 0x000C, 0x000A, 0x0008, 0x06E6, 0x04E4, 0x00E0, 0x00C0, 0x00A0, 0x0080 }
};

u8 disclaimerLogoLedIndex;

static const u16 disclaimerLogoLedFadeColors[ DISCLAIMERLOGOLED_FADESTEPS ][ DISCLAIMERLOGOLED_NUMCOLORS ] =
{
		{ 0x08E8, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x06E6, 0x08E8, 0x0000, 0x0000, 0x0000, 0x0000 },
		{ 0x04E4, 0x06E6, 0x08E8, 0x0000, 0x0000, 0x0000 },
		{ 0x00E0, 0x04E4, 0x06E6, 0x08E8, 0x0000, 0x0000 },
		{ 0x00C0, 0x00E0, 0x04E4, 0x06E6, 0x08E8, 0x0000 },
		{ 0x00A0, 0x00C0, 0x00E0, 0x04E4, 0x06E6, 0x08E8 },
		{ 0x0000, 0x00A0, 0x00C0, 0x00E0, 0x04E4, 0x06E6 },
		{ 0x0000, 0x0000, 0x00A0, 0x00C0, 0x00E0, 0x04E4 },
		{ 0x0000, 0x0000, 0x0000, 0x00A0, 0x00C0, 0x00E0 },
		{ 0x0000, 0x0000, 0x0000, 0x0000, 0x00A0, 0x00C0 },
		{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00A0 },
		{ 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 },
};


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void waitHz ( u16 hz )
{
    while ( hz-- )
    {
        VDP_waitVSync();
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerLogoLedUpdate ( void )
{

	VDP_setPaletteColors( 33, (u16 *) disclaimerLogoLedFadeColors[ disclaimerLogoLedIndex ], DISCLAIMERLOGOLED_NUMCOLORS );

	disclaimerLogoLedIndex++;

	if( disclaimerLogoLedIndex >= DISCLAIMERLOGOLED_FADESTEPS )
	{
		disclaimerLogoLedIndex = 0;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doTextDisclaimerFadeIn( void )
{
	u8 i;

	for( i = 1; i < TEXTDISCLAIMER_FADESTEPS; i++ )
	{
		waitHz(6);

		VDP_setPaletteColors( 13, (u16 *) textDisclaimerFadeColors[ i ], TEXTDISCLAIMER_NUMCOLORS );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerBGFadeIn( void )
{
	u8 i;

	for( i = 1; i < DISCLAIMERBG_FADESTEPS; i++ )
	{
		waitHz(4);

		VDP_setPaletteColors( 1, (u16 *) disclaimerBGFadeColors[ i ], DISCLAIMERBG_NUMCOLORS );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerLogoFadeIn( void )
{
	u8 i;

	for( i = 1; i < DISCLAIMERLOGO_FADESTEPS; i++ )
	{
		waitHz(4);

		VDP_setPaletteColors( 17, (u16 *) disclaimerLogoFadeColors[ i ], DISCLAIMERLOGO_NUMCOLORS );
		doDisclaimerLogoLedUpdate( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerBGFadeOut( void )
{
	s8 i;

	for( i = (DISCLAIMERBG_FADESTEPS - 2); i >= 0; i-- )
	{
		waitHz(4);

		VDP_setPaletteColors( 1, (u16 *) disclaimerBGFadeColors[ ( u8 ) i ], DISCLAIMERBG_NUMCOLORS );
		doDisclaimerLogoLedUpdate ( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doTextDisclaimerFadeOut( void )
{
	s8 i;

	for( i = (TEXTDISCLAIMER_FADESTEPS - 2 ); i >= 0 ; i-- )
	{
		waitHz(4);

		VDP_setPaletteColors( 13, (u16 *) textDisclaimerFadeColors[ ( u8 ) i ], TEXTDISCLAIMER_NUMCOLORS );
		doDisclaimerLogoLedUpdate ( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerLogoFadeOut( void )
{
	s8 i;

	for( i = (DISCLAIMERLOGO_FADESTEPS -2) ; i >= 0 ; i-- )
	{
		waitHz(4);

		VDP_setPaletteColors( 17, (u16 *) disclaimerLogoFadeColors[ ( u8 ) i ], DISCLAIMERLOGO_NUMCOLORS );
		doDisclaimerLogoLedUpdate( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

static void drawDisclaimerGraphics( void )
{
	u16 tileIndex;

	VDP_waitVSync( );

	VDP_setEnable( FALSE );

		tileIndex = TILE_USERINDEX;
		VDP_drawImageEx( PLAN_B, &disclaimerBGImage, IMG_ATTRIBUTES( PAL0 ), 0, 0, TRUE, FALSE );

		tileIndex += disclaimerBGImage.tileset->numTile;
		VDP_drawImageEx( PLAN_A, &disclaimerLogoImage, IMG_ATTRIBUTES( PAL1 ), 9, 9, TRUE, FALSE );

		tileIndex += disclaimerLogoImage.tileset->numTile;
		VDP_drawImageEx( PLAN_B, &disclaimerLogoLedImage, IMG_ATTRIBUTES( PAL2 ), 26, 9, TRUE, FALSE );

	VDP_setEnable( TRUE );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showDisclaimer( void )
{
	drawDisclaimerGraphics( );
	doTextDisclaimerFadeIn( );
	doDisclaimerBGFadeIn( );

	disclaimerLogoLedIndex = 0;

	doDisclaimerLogoFadeIn( );

	u8 i=57;
	while ( i-- )
	{
		waitHz(4);

		doDisclaimerLogoLedUpdate ( );
	}

	doDisclaimerBGFadeOut( );
	doTextDisclaimerFadeOut( );

	doDisclaimerLogoFadeOut ( );

	VDP_clearPlan( PLAN_A, 1 );
}
