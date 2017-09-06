#include <genesis.h>

#include "../inc/dev.h"
#include "../inc/helpers.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void debug ( u8 value )
{
    SYS_disableInts();
    VDP_setPalette ( PAL0, palette_blue );
    SYS_enableInts();
    drawUInt ( value, 0, value, 1 );
    VDP_waitVSync();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
