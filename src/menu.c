#include <genesis.h>

#include "../inc/menu.h"
#include "../inc/joyreader.h"




static s8 currentOption = 0;


void drawOption ( Option *option, u8 selected )
{
    VDP_setTextPalette ( selected ? PAL1 : PAL0 );
    VDP_drawTextBG( PLAN_A, option->string, option->x, option->y );
}

void drawValue ( Option *option )
{
    VDP_clearTextAreaBG ( PLAN_A, option->x2, option->y, 20, 1 );
    VDP_setTextPalette ( PAL0 );
    VDP_drawTextBG( PLAN_A, option->funct(option), option->x2, option->y );
}

void incValue ( Option *option, s16 inc )
{
    option->value += inc;

    if ( option->value < 0 ) option->value = option->nbElements-1;
    if ( option->value >= option->nbElements ) option->value = 0;
}

void execFunct ( Option *option )
{
    option->exec ( option );
}








void MenuLoop ( Option *options )
{
	#define OPTION &options[currentOption]
	do
	{
		VDP_waitVSync( );

        //drawValue ( OPTION );

		JoyReaderUpdate( );

        if ( PAD_1_PRESSED_ABC   ) execFunct ( OPTION );
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

void MenuInit ( Option *options )
{
    currentOption = 0;
//    menu->currentOption = 0;
    drawValue ( &options[0] );
	drawValue ( &options[1] );
	drawValue ( &options[2] );
	drawValue ( &options[3] );
	drawValue ( &options[4] );
	drawValue ( &options[5] );
	drawValue ( &options[6] );
}


void MenuAddOption ( Menu *menu, Option *option )
{
    menu->options[menu->nbOptions] = option;
    ++menu->nbOptions;
}
