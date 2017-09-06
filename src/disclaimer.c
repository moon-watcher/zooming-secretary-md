#include <genesis.h>

#include "../res/rescomp.h"
#include "../inc/disclaimer.h"
#include "../inc/display.h"
#include "../inc/dev.h"
#include "../inc/helpers.h"
#include "../inc/joyreader.h"


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

#define WAITBREAK(n)  if ( wait(n) ) break;


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

static u8 disclaimerLogoLedIndex;
static u16 exit;

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


static u16 wait ( u16 hz )
{
    u16 ret = 0;

    if ( exit )
    {
        ret = 1;
    }

    while ( hz-- )
    {
        VDP_waitVSync();
        JoyReaderUpdate();

        if( PAD_1_PRESSED_ABCS )
        {
            exit = 1;
            ret  = 1;

            displayOff(10);

            break;
        }
    }

    return ret;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerLogoLedUpdate ( void )
{
	SYS_disableInts();
	VDP_setPaletteColors( 33, (u16 *) disclaimerLogoLedFadeColors[ disclaimerLogoLedIndex ], DISCLAIMERLOGOLED_NUMCOLORS );
	SYS_enableInts();

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
		WAITBREAK(6);

		SYS_disableInts();
		VDP_setPaletteColors( 13, (u16 *) textDisclaimerFadeColors[ i ], TEXTDISCLAIMER_NUMCOLORS );
		SYS_enableInts();
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerBGFadeIn( void )
{
	u8 i;

	for( i = 1; i < DISCLAIMERBG_FADESTEPS; i++ )
	{
		WAITBREAK(4);

		SYS_disableInts();
		VDP_setPaletteColors( 1, (u16 *) disclaimerBGFadeColors[ i ], DISCLAIMERBG_NUMCOLORS );
		SYS_enableInts();
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerLogoFadeIn( void )
{
	u8 i;

	for( i = 1; i < DISCLAIMERLOGO_FADESTEPS; i++ )
	{
		WAITBREAK(4);

		SYS_disableInts();
		VDP_setPaletteColors( 17, (u16 *) disclaimerLogoFadeColors[ i ], DISCLAIMERLOGO_NUMCOLORS );
		SYS_enableInts();
		doDisclaimerLogoLedUpdate( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerBGFadeOut( void )
{
	s8 i;

	for( i = (DISCLAIMERBG_FADESTEPS - 2); i >= 0; i-- )
	{
		WAITBREAK(4);

		SYS_disableInts();
		VDP_setPaletteColors( 1, (u16 *) disclaimerBGFadeColors[ ( u8 ) i ], DISCLAIMERBG_NUMCOLORS );
		SYS_enableInts();
		doDisclaimerLogoLedUpdate ( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doTextDisclaimerFadeOut( void )
{
	s8 i;

	for( i = (TEXTDISCLAIMER_FADESTEPS - 2 ); i >= 0 ; i-- )
	{
		WAITBREAK(4);

		SYS_disableInts();
		VDP_setPaletteColors( 13, (u16 *) textDisclaimerFadeColors[ ( u8 ) i ], TEXTDISCLAIMER_NUMCOLORS );
		SYS_enableInts();
		doDisclaimerLogoLedUpdate ( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doDisclaimerLogoFadeOut( void )
{
	s8 i;

	for( i = (DISCLAIMERLOGO_FADESTEPS -2) ; i >= 0 ; i-- )
	{
		WAITBREAK(4);

		SYS_disableInts();
		VDP_setPaletteColors( 17, (u16 *) disclaimerLogoFadeColors[ ( u8 ) i ], DISCLAIMERLOGO_NUMCOLORS );
		SYS_enableInts();
		doDisclaimerLogoLedUpdate( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

static void drawDisclaimerGraphics( void )
{
	u16 tileIndex;

	displayOff(0);

		SYS_disableInts();
		tileIndex = TILE_USERINDEX;
		VDP_drawImageEx( PLAN_B, &disclaimerBGImage, IMG_ATTRIBUTES( PAL0 ), 0, 0, 0, FALSE );

		tileIndex += disclaimerBGImage.tileset->numTile;
		VDP_drawImageEx( PLAN_A, &disclaimerLogoImage, IMG_ATTRIBUTES( PAL1 ), 9, 9, 0, FALSE );

		tileIndex += disclaimerLogoImage.tileset->numTile;
		VDP_drawImageEx( PLAN_B, &disclaimerLogoLedImage, IMG_ATTRIBUTES( PAL2 ), 26, 9, 0, FALSE );
		SYS_enableInts();

	preparePal ( PAL0, disclaimerBGImage.palette->data );
	preparePal ( PAL1, disclaimerLogoImage.palette->data );
	preparePal ( PAL2, disclaimerLogoLedImage.palette->data );

	displayOn(0);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showDisclaimer( void )
{
    if ( DEV )
    {
        return;
    }

    exit = 0;

	drawDisclaimerGraphics( );
	doTextDisclaimerFadeIn( );
	doDisclaimerBGFadeIn( );

	disclaimerLogoLedIndex = 0;

	doDisclaimerLogoFadeIn( );

	u8 i=57;
	while ( i-- )
	{
		WAITBREAK(4);

		doDisclaimerLogoLedUpdate ( );
	}

	doDisclaimerBGFadeOut( );
	doTextDisclaimerFadeOut( );

	doDisclaimerLogoFadeOut ( );

	resetScreen();
}
