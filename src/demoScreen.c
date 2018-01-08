#include <genesis.h>

#include "../inc/creditsScreen.h"
#include "../inc/dev.h"
#include "../inc/game.h"
#include "../inc/display.h"
#include "../inc/helpers.h"
#include "../res/rescomp.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u16 colors[64];
static u16 y;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void writeText ( u8 *str, u16 pal, u8 inc_y )
{
    VDP_setTextPalette ( pal );

    u8 len = strlen(str);
    u8 x = 20 - len/2;

    SYS_disableInts();
    VDP_drawText(str, x, y );
    SYS_enableInts();

    y += inc_y;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void preparePalettes ()
{
    VDP_waitVSync();
    SYS_disableInts();
    VDP_setPaletteColors ( 0, NULL, 64 );

    VDP_setPalette ( PAL0, NULL );
    VDP_setPalette ( PAL1, font1.data );
    VDP_setPalette ( PAL2, font2.data );

    VDP_setPaletteColor ( 28, VDP_getPaletteColor(46) );
    VDP_setPaletteColor ( 29, VDP_getPaletteColor(47) );
    VDP_setPaletteColor ( 44, VDP_getPaletteColor(46) );
    VDP_setPaletteColor ( 45, VDP_getPaletteColor(47) );

    VDP_getPaletteColors(0,colors, 64);
    SYS_enableInts();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void writeTextCredits1 ()
{
    writeText ( "THANKS FOR PLAYING", PAL2, 4 );
    writeText ( "ZOOMING SECRETARY DEMO", PAL1, 1 );
    writeText ( "FOR SEGA GENESIS / MEGADRIVE", PAL1, 4 );
    writeText ( "AVAILABLE REAL SOON!", PAL2, 1 );
    writeText ( "STAY TUNED FOR MORE DETAILS", PAL2, 1 );
    writeText ( "AT WWW.PLAYONRETRO.COM", PAL2, 1 );

    SYS_disableInts();
    VDP_setTextPalette ( PAL2 );
    VDP_drawText ( "DEMO", 27, 12 );
    SYS_enableInts();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void writeTextCredits2 ()
{
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u16 writeCredits ( u8 part, u8 y_pos )
{
    y = y_pos;

    if ( part == 1 ) writeTextCredits1();
    if ( part == 2 ) writeTextCredits2();

    VDP_waitVSync();
    SYS_disableInts();
    VDP_setPaletteColors ( 0,colors, 64 );
    SYS_enableInts();

    u16 ret = JOY_waitPressBtnTime(6000);

    displayOff ( 10 );

    resetScreen();

    return ret;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showTFPDemo()
{
    if ( DEV && !DEMO_MODE_FLAG )
    {
        return;
    }

    resetScreen();

    preparePalettes();


    writeCredits ( 1, 8 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
