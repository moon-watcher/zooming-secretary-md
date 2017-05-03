#include <genesis.h>

#include "../inc/creditsScreen.h"
#include "../inc/dev.h"
#include "../inc/display.h"
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

    VDP_drawText(str, x, y );

    y += inc_y;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void preparePalettes ()
{
    VDP_waitVSync();
    VDP_setPaletteColors ( 0, NULL, 64 );

    VDP_setPalette ( PAL0, NULL );
    VDP_setPalette ( PAL1, font1.data );
    VDP_setPalette ( PAL2, font2.data );

    VDP_setPaletteColor ( 28, VDP_getPaletteColor(46) );
    VDP_setPaletteColor ( 29, VDP_getPaletteColor(47) );
    VDP_setPaletteColor ( 44, VDP_getPaletteColor(46) );
    VDP_setPaletteColor ( 45, VDP_getPaletteColor(47) );

    VDP_getPaletteColors(0,colors, 64);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void writeTextCredits1 ()
{
    writeText ( "ZOOMING SECRETARY", PAL1, 1 );
    writeText ( "FOR SEGA GENESIS / MEGADRIVE", PAL1, 2 );
    writeText ( "THE AFROMONKEYS", PAL2, 1 );
    writeText ( "@ 2015 - 2017", PAL2, 5 );
    writeText ( "ORIGINAL GAME", PAL1, 2 );
    writeText ( "PINWIZZ & SHIRU", PAL2, 1 );
    writeText ( "@ 2011", PAL2, 1 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void writeTextCredits2 ()
{
    writeText ( "CODE", PAL1, 2 );
    writeText ( "JACK NOLDDOR @NOLDDOR", PAL2, 2 );
    writeText ( "MUN @MOONWATCHERMD", PAL2, 4 );
    writeText ( "GRAPHICS", PAL1, 2);
    writeText ( "DANIEL NEVADO @DANYSNOWYMAN", PAL2, 4 );
    writeText ( "MUSIC & SFX", PAL1, 2 );
    writeText ( "BEYKER", PAL2, 4 );
    writeText ( "TESTING", PAL1, 2 );
    writeText ( "ALFONSO MARTINEZ @_SRPRESLEY_", PAL2, 1 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u16 writeCredits ( u8 part, u8 y_pos )
{
    y = y_pos;

    if ( part == 1 ) writeTextCredits1();
    if ( part == 2 ) writeTextCredits2();

    VDP_waitVSync();
    VDP_setPaletteColors ( 0,colors, 64 );

    u16 ret = JOY_waitPressBtnTime(6000);

    displayOff ( 10 );

    VDP_clearPlan ( PLAN_A, 1 );
    VDP_clearPlan ( PLAN_B, 1 );

    return ret;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showCredits ()
{
    if ( DEV )
    {
        return;
    }

    VDP_clearPlan ( PLAN_A, 1 );
    VDP_clearPlan ( PLAN_B, 1 );

    preparePalettes();

    u16 exit = writeCredits ( 1, 8 );

    if ( !exit )
    {
        writeCredits ( 2, 3 );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
