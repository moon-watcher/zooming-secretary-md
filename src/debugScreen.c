#include <genesis.h>

#include "../res/rescomp.h"
#include "../inc/game.h"
#include "../inc/joyreader.h"
#include "../inc/common.h"
#include "../inc/helpers.h"
#include "../inc/dev.h"
#include "../inc/vint.h"

/*
 * Jack Nolddor, [14.09.15 20:35]
@Sr_Presley sound test / selecionar nivel / Invencibilidad... any more para la pantalla de debug?

Jack Nolddor, [14.09.15 20:49]
music on / off
sfx On / Off
 */

#define IMG_ATTRIBUTES ( TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ) )




typedef struct _Option
{
    u8    x, x2, y;
    u8   *string;
    u8*  (*funct) (struct _Option *);
    void (*exec) (struct _Option *);
    s32   value;
    s32   nbElements;
}
Option;




static void drawOption ( Option *option, u8 selected )
{
    VDP_setTextPalette ( selected ? PAL1 : PAL0 );
    VDP_drawTextBG( PLAN_A, option->string, option->x, option->y );
}

static void drawValue ( Option *option )
{
    VDP_clearTextAreaBG ( PLAN_A, option->x2, option->y, 20, 1 );
    VDP_setTextPalette ( PAL0 );
    VDP_drawTextBG( PLAN_A, option->funct(option), option->x2, option->y );
}

static void incValue ( Option *option, s16 inc )
{
    option->value += inc;

    if ( option->value < 0 ) option->value = option->nbElements-1;
    if ( option->value >= option->nbElements ) option->value = 0;
}





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





s8 currentOption = 0;

void updateMenu ( Option *options )
{
	#define OPTION &options[currentOption]
	do
	{
		VDP_waitVSync( );

        //drawValue ( OPTION );

		JoyReaderUpdate( );

        if ( PAD_1_PRESSED_ABC   ) option->exec ( option );
        if ( PAD_1_PRESSED_RIGHT ) incValue ( OPTION, +1 );
        if ( PAD_1_PRESSED_LEFT  ) incValue ( OPTION, -1 );

		if ( PAD_1_PRESSED_UP || PAD_1_PRESSED_DOWN )
        {
            drawOption ( OPTION, 0 );

            currentOption += ( PAD_1_PRESSED_DOWN ? +1 : -1 );

            if ( currentOption < 0 ) currentOption = 6;
            if ( currentOption > 6 ) currentOption = 0;

            drawOption ( OPTION, 1 );
        }

        drawValue ( OPTION );
	}
	while( !PAD_1_PRESSED_START );
}

void initMenu ( Option *options )
{
    currentOption = 0;
    drawValue ( &options[0] );
	drawValue ( &options[1] );
	drawValue ( &options[2] );
	drawValue ( &options[3] );
	drawValue ( &options[4] );
	drawValue ( &options[5] );
	drawValue ( &options[6] );
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

	initMenu ( options );
    updateMenu ( options );


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
