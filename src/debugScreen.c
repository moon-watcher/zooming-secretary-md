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


static const u8 *options[] =
{
    "INVINCIBILITY",
    "LEVEL SELECTION",
    "FAST EXIT",
    "ACTIVATE MUSIC",
    "ACTIVATE SFX",
    "MUSIC",
    "SFX",
};



static void cleanScreen( void )
{
	VDP_setEnable( FALSE ) ;

	VDP_setHorizontalScroll(PLAN_B, 0 );
	VDP_setVerticalScroll(PLAN_B, 0 );

	VDP_clearPlan( PLAN_A, 1 );
}


static void drawOption ( s8 option, u8 selected )
{
    VDP_setTextPalette ( selected ? PAL1 : PAL0 );

    u8 x = 8;
    u8 y = 6 + option * 2;

    VDP_drawTextBG( PLAN_A, options[(u8)option], x, y );
}


static void drawOptionStatus ( u8 option, s8 *status )
{
    u8 str[40];

    u8 x = 24;
    u8 y = 6 + option * 2;

    if ( option < 5 )
    {
        strcpy ( str, status[option] ? "ON" : "OFF" );
    }
    else if ( option == 5 )
    {
        strcpy ( str, getMusic(status[option])->title );
    }
    else
    {
        strcpy ( str, getSfx(status[option])->title );
    }

    stoupper (str);
    VDP_clearTextAreaBG ( PLAN_A, x, y, 20, 1 );
    VDP_setTextPalette ( PAL0 );
    VDP_drawTextBG( PLAN_A, str, x, y );
}


static void initDebugScreen()
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

	drawOption ( 0, 1 );
	drawOption ( 1, 0 );
	drawOption ( 2, 0 );
	drawOption ( 3, 0 );
	drawOption ( 4, 0 );
	drawOption ( 5, 0 );
	drawOption ( 6, 0 );

    VDP_setEnable( TRUE );
}


void showDebugScreen( void )
{
    initDebugScreen();

    s8 currentOption = 0;
    s8 optionStatus [ 7 ] =
    {
        GOD_MODE_FLAG,
        LEVEL_MODE_FLAG,
        EXIT_MODE_FLAG,
        MUSIC_MODE_FLAG,
        SFX_MODE_FLAG,
        0,
        0
    };

    MUSIC_MODE_FLAG = 1; // enable flag to play it
    SFX_MODE_FLAG   = 1; // enable flag to play it

	drawOptionStatus ( 0, optionStatus );
	drawOptionStatus ( 1, optionStatus );
	drawOptionStatus ( 2, optionStatus );
	drawOptionStatus ( 3, optionStatus );
	drawOptionStatus ( 4, optionStatus );
	drawOptionStatus ( 5, optionStatus );
	drawOptionStatus ( 6, optionStatus );

	do
	{
		VDP_waitVSync( );

        drawOptionStatus ( currentOption, optionStatus );

		JoyReaderUpdate( );

        if ( PAD_1_PRESSED_ABC  && currentOption < 5 ) optionStatus[(u8)currentOption]++;
        if ( PAD_1_PRESSED_RIGHT ) optionStatus[(u8)currentOption]++;
        if ( PAD_1_PRESSED_LEFT  ) optionStatus[(u8)currentOption]--;

		if ( PAD_1_PRESSED_UP || PAD_1_PRESSED_DOWN )
        {
            drawOption ( currentOption, 0 );

            currentOption += ( PAD_1_PRESSED_DOWN ? +1 : -1 );

            if ( currentOption < 0 ) currentOption = 6;
            if ( currentOption > 6 ) currentOption = 0;

            drawOption ( currentOption, 1 );
        }

        if ( optionStatus[0]  <         0 ) optionStatus[0] = 1;
        if ( optionStatus[0]  >         1 ) optionStatus[0] = 0;

        if ( optionStatus[1]  <         0 ) optionStatus[1] = 1;
        if ( optionStatus[1]  >         1 ) optionStatus[1] = 0;

        if ( optionStatus[2]  <         0 ) optionStatus[2] = 1;
        if ( optionStatus[2]  >         1 ) optionStatus[2] = 0;

        if ( optionStatus[3]  <         0 ) optionStatus[3] = 1;
        if ( optionStatus[3]  >         1 ) optionStatus[3] = 0;

        if ( optionStatus[4]  <         0 ) optionStatus[4] = 1;
        if ( optionStatus[4]  >         1 ) optionStatus[4] = 0;

        if ( optionStatus[5]  <         0 ) optionStatus[5] = MUSIC_MAX - 1;
        if ( optionStatus[5] >= MUSIC_MAX ) optionStatus[5] = 0;

        if ( optionStatus[6]  <         0 ) optionStatus[6] = SFX_MAX   - 1;
        if ( optionStatus[6] >= SFX_MAX   ) optionStatus[6] = 0;

        if ( currentOption == 5 && PAD_1_PRESSED_ABC ) playMusic ( optionStatus[5] );
        if ( currentOption == 6 && PAD_1_PRESSED_ABC ) playSfx   ( optionStatus[6] );

        if ( DEV )
        {
            drawInt(currentOption,0,0,1);
            drawInt(optionStatus[0],0,1,1);
            drawInt(optionStatus[1],0,2,1);
            drawInt(optionStatus[2],0,3,1);
            drawInt(optionStatus[3],0,4,1);
            drawInt(optionStatus[4],0,5,1);
            drawInt(optionStatus[5],0,6,1);
            drawInt(optionStatus[6],0,7,1);
        }

	} while( !PAD_1_PRESSED_START );

	cleanScreen( );
    musicStop();

    VIntSetUpdateScroll ( 0 );

	GOD_MODE_FLAG   = optionStatus[0];
	LEVEL_MODE_FLAG = optionStatus[1];
	EXIT_MODE_FLAG  = optionStatus[2];
	MUSIC_MODE_FLAG = optionStatus[3];
	SFX_MODE_FLAG   = optionStatus[4];

    if ( DEV )
    {
        drawInt ( GOD_MODE_FLAG,   0, 10, 1 );
        drawInt ( LEVEL_MODE_FLAG, 0, 11, 1 );
        drawInt ( EXIT_MODE_FLAG,  0, 12, 1 );
        drawInt ( MUSIC_MODE_FLAG, 0, 13, 1 );
        drawInt ( SFX_MODE_FLAG,   0, 14, 1 );
    }
}
