#include <genesis.h>

#include "../inc/playonretroScreen.h"
#include "../inc/dev.h"
#include "../res/rescomp.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showPlayonretro ( )
{
    if ( DEV )
    {
        return;
    }

    SYS_disableInts();

    VDP_clearPlan(PLAN_A, 1);
    VDP_clearPlan(PLAN_B, 1);


    VDP_drawImageEx( PLAN_A, &playonretroImage, TILE_ATTR_FULL(PAL1,0,0,0,16), 9, 8, 1, 0 );

    SYS_enableInts();

    JOY_waitPressBtnTime(4000);

    VDP_fadeOutAll ( 15, 0 );

    VDP_clearPlan(PLAN_A, 1);
    VDP_clearPlan(PLAN_B, 1);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
