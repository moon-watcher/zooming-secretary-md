#include <genesis.h>

#include "../res/rescomp.h"
#include "../inc/game.h"
#include "../inc/joyreader.h"
#include "../inc/common.h"
#include "../inc/helpers.h"
#include "../inc/dev.h"
#include "../inc/vint.h"
#include "../inc/menu.h"

/*
 * Jack Nolddor, [14.09.15 20:35]
@Sr_Presley sound test / selecionar nivel / Invencibilidad... any more para la pantalla de debug?

Jack Nolddor, [14.09.15 20:49]
music on / off
sfx On / Off
 */

#define IMG_ATTRIBUTES ( TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ) )




static void incOnOff ( Option *option )
{
    incValue ( option, 1 );
}

static u8 *fOnOff ( Option *option )
{
    return option->value ? "ON ": "OFF";
}

static u8 *fMusic ( Option *option )
{
    u8 str[40];

    strcpy ( str, getMusic ( option->value )->title );
    stoupper ( str );

    return str;
}

static u8 *fSfx ( Option *option )
{
    u8 str[40];

    strcpy ( str, getSfx ( option->value )->title );
    stoupper ( str );

    return str;
}

static void myPlayMusic ( Option *option )
{
    playMusic ( option->value );
}

static void myPlaySfx ( Option *option )
{
    playSfx ( option->value );
}




static void initDebugScreen( Option *options )
{
	VDP_waitVSync( );

	VDP_setEnable( FALSE );

	VDP_drawImageEx( PLAN_B, &debugScreenImg, IMG_ATTRIBUTES, 0, 0, TRUE, FALSE );

	VIntSetUpdateScroll ( 1 );

	VDP_setTextPalette(PAL0);
	VDP_drawTextBG( PLAN_A, "DEBUG OPTIONS", 14, 2 );
	VDP_drawTextBG( PLAN_A, "PRESS START TO EXIT", 11, 25 );

	VDP_setPaletteColor ( 30, 0x0ff );
	VDP_setPaletteColor ( 31, 0x000 );
	VDP_setPaletteColor ( 12, 0xfff );
	VDP_setPaletteColor ( 13, 0x000 );

	drawOption ( &options[0], 1 );
	drawOption ( &options[1], 0 );
	drawOption ( &options[2], 0 );
	drawOption ( &options[3], 0 );
	drawOption ( &options[4], 0 );
	drawOption ( &options[5], 0 );
	drawOption ( &options[6], 0 );

    VDP_setEnable( TRUE );
}







void showDebugScreen( void )
{
    Option options[] =
    {
        { 8, 24,  6, "INVINCIBILITY",   fOnOff, incOnOff,    GOD_MODE_FLAG,   2 },
        { 8, 24,  8, "LEVEL SELECTION", fOnOff, incOnOff,    LEVEL_MODE_FLAG, 2 },
        { 8, 24, 10, "FAST EXIT",       fOnOff, incOnOff,    EXIT_MODE_FLAG,  2 },
        { 8, 24, 12, "ACTIVATE MUSIC",  fOnOff, incOnOff,    MUSIC_MODE_FLAG, 2 },
        { 8, 24, 14, "ACTIVATE SFX",    fOnOff, incOnOff,    SFX_MODE_FLAG,   2 },
        { 8, 24, 16, "MUSIC",           fMusic, myPlayMusic, 0,               MUSIC_MAX },
        { 8, 24, 18, "SFX",             fSfx,   myPlaySfx,   0,               SFX_MAX },
    };


    initDebugScreen( options );



    MUSIC_MODE_FLAG = 1; // enable flag to play it
    SFX_MODE_FLAG   = 1; // enable flag to play it

	MenuInit ( options );
    MenuLoop ( options );


    VDP_setEnable( FALSE ) ;

	VDP_setHorizontalScroll(PLAN_B, 0 );
	VDP_setVerticalScroll(PLAN_B, 0 );
	VDP_setHorizontalScroll(PLAN_A, 0 );
	VDP_setVerticalScroll(PLAN_A, 0 );

	VDP_clearPlan( PLAN_A, 1 );

    musicStop();

    VIntSetUpdateScroll ( 0 );

	GOD_MODE_FLAG   = options[0].value;
	LEVEL_MODE_FLAG = options[1].value;
	EXIT_MODE_FLAG  = options[2].value;
	MUSIC_MODE_FLAG = options[3].value;
	SFX_MODE_FLAG   = options[4].value;

    if ( DEV )
    {
        drawInt ( GOD_MODE_FLAG,   0, 10, 1 );
        drawInt ( LEVEL_MODE_FLAG, 0, 11, 1 );
        drawInt ( EXIT_MODE_FLAG,  0, 12, 1 );
        drawInt ( MUSIC_MODE_FLAG, 0, 13, 1 );
        drawInt ( SFX_MODE_FLAG,   0, 14, 1 );
    }
}
