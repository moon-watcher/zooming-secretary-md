#include <genesis.h>

#include "../inc/menu.h"
#include "../inc/joyreader.h"


static void incValue ( Option *option, s16 inc )
{
    option->value += inc;

    if ( option->value <  0           ) option->value = option->max-1;
    if ( option->value >= option->max ) option->value = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




void MenuInit ( Menu *menu )
{
    menu->nbOptions     = 0;
    menu->currentOption = 0;
    menu->lineHeight    = 2;
}


void MenuSetX ( Menu *menu, u8 x )
{
    menu->x = x;
}


void MenuSetY ( Menu *menu, u8 y )
{
    menu->y = y;
}


void MenuSetX2 ( Menu *menu, u8 x2 )
{
    menu->x2 = x2;
}


void MenuSetPosition ( Menu *menu, u8 x, u8 y, u8 x2 )
{
    MenuSetX  ( menu,  x );
    MenuSetY  ( menu,  y );
    MenuSetX2 ( menu, x2 );
}

void MenuSetLineHeight ( Menu *menu, u8 lineHeight )
{
    menu->lineHeight = lineHeight;
}


void MenuSetOption ( Menu *menu, s8 currentOption )
{
    menu->currentOption = currentOption;
}


void MenuAddOption ( Menu *menu, Option *option )
{
    menu->options [ menu->nbOptions ] = option;
    menu->options [ menu->nbOptions ]->x  = menu->x;
    menu->options [ menu->nbOptions ]->y  = menu->y + menu->nbOptions *  menu->lineHeight;
    menu->options [ menu->nbOptions ]->x2 = menu->x2;

    ++menu->nbOptions;
}


void MenuDrawOption ( Menu *menu, u8 index )
{
    Option *o = menu->options [ index ];

    VDP_setTextPalette ( menu->currentOption == index ? PAL1 : PAL0 );
    VDP_drawTextBG( PLAN_A, o->string, o->x, o->y  );
}


void MenuDrawValue ( Menu *menu, u8 index )
{
    Option *o = menu->options [ index ];

    VDP_clearTextAreaBG ( PLAN_A, o->x2, o->y, 20, 1 );
    VDP_setTextPalette ( PAL0 );
    VDP_drawTextBG ( PLAN_A, o->fDirection(o), o->x2, o->y );
}


void MenuDraw ( Menu *menu )
{
    u8 i;

    for ( i = 0; i < menu->nbOptions; i++ )
    {
        MenuDrawOption ( menu, i );
        MenuDrawValue  ( menu, i );
    }

    menu->currentOption = 0;
}


void MenuLoop ( Menu *menu )
{
	do
	{
		VDP_waitVSync( );
		JoyReaderUpdate( );

        s8     *currentOption = &menu->currentOption;
        Option *option        =  menu->options [ (u8) *currentOption ];

        if ( PAD_1_PRESSED_ABC   ) MenuOptionFunction ( option );
        if ( PAD_1_PRESSED_RIGHT ) MenuOptionInc      ( option );
        if ( PAD_1_PRESSED_LEFT  ) MenuOptionDec      ( option );

		if ( PAD_1_PRESSED_UP || PAD_1_PRESSED_DOWN )
        {
            s8 aux = *currentOption;

            *currentOption += ( PAD_1_PRESSED_DOWN ? +1 : -1 );

            if ( *currentOption <  0               ) *currentOption = menu->nbOptions - 1;
            if ( *currentOption >= menu->nbOptions ) *currentOption = 0;

            MenuDrawOption ( menu, aux            ); // restore previous selected option
            MenuDrawOption ( menu, *currentOption );
        }

        MenuDrawValue ( menu, *currentOption );
	}
	while( !PAD_1_PRESSED_START );
}


void MenuOptionInc ( Option *option )
{
    incValue ( option, +1 );
}


void MenuOptionDec ( Option *option )
{
    incValue ( option, -1 );
}


void MenuOptionFunction ( Option *option )
{
    option->fButton ( option );
}
