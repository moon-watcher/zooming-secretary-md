#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/SPRFactory.h"

#include "../inc/CeilingFan.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define CEILING_FANS_MAX  3
#define IS_CEILING_FAN_INITIALIZED( obj ) ( obj != NULL )



/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

static Sprite* allCeilingFans[ CEILING_FANS_MAX ];

static u16 ceilingFansFrameCounter;
static u16 totalOfCeilingFans;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void ceilingFansReset( void )
{
    memset( allCeilingFans, NULL, sizeof( allCeilingFans ) );
    ceilingFansFrameCounter = 0;
    totalOfCeilingFans  = 0;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void ceilingFanAdd( s16 basePositionX, s16 basePositionY )
{
    u8 ceilingFanIdx;

    for ( ceilingFanIdx = totalOfCeilingFans; ceilingFanIdx < CEILING_FANS_MAX; ceilingFanIdx++ )
    {
        Sprite *thisCeilingFan = allCeilingFans[ ceilingFanIdx ];

        if ( !IS_CEILING_FAN_INITIALIZED( thisCeilingFan ) )
        {
            allCeilingFans[ ceilingFanIdx ] = SPRFactory.createCeilingFan( basePositionX, basePositionY );
            totalOfCeilingFans++;
        }

        break;
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void ceilingFanUpdate( void )
{
    if ( !(ceilingFansFrameCounter & 3 ) )
    {
        u8 ceilingFanIdx;

        for ( ceilingFanIdx = 0; ceilingFanIdx < totalOfCeilingFans; ceilingFanIdx++ )
        {
            Sprite *thisCeilingFan = allCeilingFans[ ceilingFanIdx ];
            SPR_nextFrame( thisCeilingFan );
        }
    }

    ceilingFansFrameCounter++;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
