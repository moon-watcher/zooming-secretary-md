#include <genesis.h>

#include "../inc/playonretroScreen.h"
#include "../inc/dev.h"
#include "../inc/display.h"
#include "../res/rescomp.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showPlayonretro ( )
{
    if ( DEV )
    {
        return;
    }


    displayOff(0);

    VDP_clearPlan(PLAN_A, 1);
    VDP_clearPlan(PLAN_B, 1);

    VDP_drawImageEx( PLAN_A, &playonretroImage, TILE_ATTR_FULL(PAL1,0,0,0,16), 9, 8, 0, 0 );
    preparePal( PAL1, playonretroImage.palette->data );

    displayOn(10);

    JOY_waitPressBtnTime(4000);

    displayOff ( 10 );

    VDP_clearPlan(PLAN_A, 1);
    VDP_clearPlan(PLAN_B, 1);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
