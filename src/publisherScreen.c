#include <genesis.h>

#include "../inc/publisherScreen.h"
#include "../inc/dev.h"
#include "../inc/display.h"
#include "../inc/helpers.h"
#include "../res/rescomp.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showPublisher ( )
{
    if ( DEV )
    {
        return;
    }


    displayOff(0);

    SYS_disableInts();

    resetScreen();

    VDP_drawImageEx( PLAN_A, &publisherImage, TILE_ATTR_FULL(PAL1,0,0,0,16), 9, 8, 0, 0 );
    preparePal( PAL1, publisherImage.palette->data );

    SYS_enableInts();

    displayOn(10);

    JOY_waitPressBtnTime(4000);

    displayOff ( 10 );

    resetScreen();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
