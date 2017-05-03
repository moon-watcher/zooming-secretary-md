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


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 str[40];


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 *funcOnOff ( Option *option )
{
    return option->value ? "ON ": "OFF";
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 *funcMusic ( Option *option )
{
    strcpy ( str, getMusic ( option->value )->title );
    stoupper ( str );

    return str;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 *funcSfx ( Option *option )
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


static void initDebugScreen ( )
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

    VDP_setEnable( TRUE );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showDebugScreen( void )
{
    initDebugScreen ( );

    Option options[] =
    {
        { "INVINCIBILITY",   funcOnOff, MenuOptionInc, GOD_MODE_FLAG,   2         },
        { "LEVEL SELECTION", funcOnOff, MenuOptionInc, LEVEL_MODE_FLAG, 2         },
        { "FAST EXIT",       funcOnOff, MenuOptionInc, EXIT_MODE_FLAG,  2         },
        { "ACTIVATE MUSIC",  funcOnOff, MenuOptionInc, MUSIC_MODE_FLAG, 2         },
        { "ACTIVATE SFX",    funcOnOff, MenuOptionInc, SFX_MODE_FLAG,   2         },
        { "MUSIC",           funcMusic, funcPlayMusic, 0,               MUSIC_MAX },
        { "SFX",             funcSfx,   funcPlaySfx,   0,               SFX_MAX   },
    };

    MUSIC_MODE_FLAG = 1; // enable flag to play music
    SFX_MODE_FLAG   = 1; // enable flag to play sfx

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
    MenuDraw ( &menu );
    MenuLoop ( &menu );

    GOD_MODE_FLAG   = menu.options[0]->value;
	LEVEL_MODE_FLAG = menu.options[1]->value;
	EXIT_MODE_FLAG  = menu.options[2]->value;
	MUSIC_MODE_FLAG = menu.options[3]->value;
	SFX_MODE_FLAG   = menu.options[4]->value;

    musicStop();
    VDP_fadeOutAll ( 15, 0 );
    VIntSetUpdateScroll ( 0 );
    VDP_waitVSync();
	VDP_setHorizontalScroll ( PLAN_B, 0 );
	VDP_setVerticalScroll   ( PLAN_B, 0 );
	VDP_setHorizontalScroll ( PLAN_A, 0 );
	VDP_setVerticalScroll   ( PLAN_A, 0 );
	VDP_clearPlan( PLAN_A, 1 );

    if ( DEV )
    {
        drawInt ( GOD_MODE_FLAG,   0, 10, 1 );
        drawInt ( LEVEL_MODE_FLAG, 0, 11, 1 );
        drawInt ( EXIT_MODE_FLAG,  0, 12, 1 );
        drawInt ( MUSIC_MODE_FLAG, 0, 13, 1 );
        drawInt ( SFX_MODE_FLAG,   0, 14, 1 );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
