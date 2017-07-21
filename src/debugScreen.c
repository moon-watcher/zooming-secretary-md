#include <genesis.h>

#include "../res/rescomp.h"
#include "../inc/game.h"
#include "../inc/joyreader.h"
#include "../inc/common.h"
#include "../inc/helpers.h"
#include "../inc/dev.h"
#include "../inc/vint.h"
#include "../inc/menu.h"
#include "../inc/display.h"

/*
 * Jack Nolddor, [14.09.15 20:35]
@Sr_Presley sound test / selecionar nivel / Invencibilidad... any more para la pantalla de debug?

Jack Nolddor, [14.09.15 20:49]
music on / off
sfx On / Off
 */

#define IMG_ATTRIBUTES ( TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ) )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 str[40];


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 *funcGetOnOff ( Option *option )
{
    return option->value ? "ON ": "OFF";
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 *funcGetMusic ( Option *option )
{
    strcpy ( str, getMusic ( option->value )->title );
    stoupper ( str );

    return str;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 *funcGetSfx ( Option *option )
{
    strcpy ( str, getSfx ( option->value )->title );
    stoupper ( str );

    return str;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void funcPlayMusic ( Option *option )
{
    playMusic ( option->value );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void funcPlaySfx ( Option *option )
{
    playSfx ( option->value );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showDebugScreen( void )
{
	displayOff(0);

	VDP_drawImageEx( PLAN_B, &debugScreenImg, IMG_ATTRIBUTES, 0, 0, 0, FALSE );

	VIntSetUpdateScroll ( 1 );

	VDP_setTextPalette(PAL0);
	VDP_drawTextBG( PLAN_A, "DEBUG OPTIONS", 14, 2 );
	VDP_drawTextBG( PLAN_A, "PRESS START TO EXIT", 11, 25 );

	preparePal ( PAL0, debugScreenImg.palette->data );
	prepareColor ( 30, 0x0ff );
	prepareColor ( 31, 0x000 );
	prepareColor ( 12, 0xfff );
	prepareColor ( 13, 0x000 );

    Option options[] =
    {
        { "INVINCIBILITY",   funcGetOnOff, MenuOptionInc, GOD_MODE_FLAG,     2         },
        { "LEVEL SELECTION", funcGetOnOff, MenuOptionInc, LEVEL_MODE_FLAG,   2         },
        { "FAST EXIT",       funcGetOnOff, MenuOptionInc, EXIT_MODE_FLAG,    2         },
        { "CLASSIC MODE",    funcGetOnOff, MenuOptionInc, CLASSIC_MODE_FLAG, 2         },
        { "ACTIVATE MUSIC",  funcGetOnOff, MenuOptionInc, MUSIC_MODE_FLAG,   2         },
        { "ACTIVATE SFX",    funcGetOnOff, MenuOptionInc, SFX_MODE_FLAG,     2         },
        { "MUSIC",           funcGetMusic, funcPlayMusic, 0,                 MUSIC_MAX },
        { "SFX",             funcGetSfx,   funcPlaySfx,   0,                 SFX_MAX   },
    };

    Menu menu;

    MenuInit ( &menu );
    MenuSetPosition ( &menu, 7, 6, 24 );
    MenuAddOption ( &menu, &options[0] );
    MenuAddOption ( &menu, &options[1] );
    MenuAddOption ( &menu, &options[2] );
    MenuAddOption ( &menu, &options[3] );
    MenuAddOption ( &menu, &options[4] );
    MenuAddOption ( &menu, &options[5] );
    MenuAddOption ( &menu, &options[6] );
    MenuAddOption ( &menu, &options[7] );
    MenuDraw ( &menu );

    MUSIC_MODE_FLAG = 1; // enable flag to play music
    SFX_MODE_FLAG   = 1; // enable flag to play sfx

	displayOn(10);

    MenuLoop ( &menu );

    GOD_MODE_FLAG     = menu.options[0]->value;
	LEVEL_MODE_FLAG   = menu.options[1]->value;
	EXIT_MODE_FLAG    = menu.options[2]->value;
	CLASSIC_MODE_FLAG = menu.options[3]->value;
	MUSIC_MODE_FLAG   = menu.options[4]->value;
	SFX_MODE_FLAG     = menu.options[5]->value;

    musicStop();
    displayOff ( 10 );
    VIntSetUpdateScroll ( 0 );
    VDP_waitVSync();
	resetScreen();

    if ( DEV )
    {
        drawInt ( GOD_MODE_FLAG,     0, 10, 1 );
        drawInt ( LEVEL_MODE_FLAG,   0, 11, 1 );
        drawInt ( EXIT_MODE_FLAG,    0, 12, 1 );
        drawInt ( CLASSIC_MODE_FLAG, 0, 13, 1 );
        drawInt ( MUSIC_MODE_FLAG,   0, 14, 1 );
        drawInt ( SFX_MODE_FLAG,     0, 15, 1 );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
