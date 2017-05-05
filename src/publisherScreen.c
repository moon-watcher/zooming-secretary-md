#include <genesis.h>

#include "../inc/publisherScreen.h"
#include "../inc/dev.h"
#include "../inc/display.h"
#include "../res/rescomp.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showPublisher ( )
{
    if ( DEV )
    {
        return;
    }


    displayOff(0);

    VDP_clearPlan(PLAN_A, 1);
    VDP_clearPlan(PLAN_B, 1);

    VDP_drawImageEx( PLAN_A, &publisherImage, TILE_ATTR_FULL(PAL1,0,0,0,16), 9, 8, 0, 0 );
    preparePal( PAL1, publisherImage.palette->data );

    displayOn(10);

    JOY_waitPressBtnTime(4000);

    displayOff ( 10 );

    VDP_clearPlan(PLAN_A, 1);
    VDP_clearPlan(PLAN_B, 1);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
