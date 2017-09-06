#include <genesis.h>
#include "../inc/hud.h"
#include "../inc/StringUtils.h"
#include "../inc/splashScreen.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define HUD_PALETTE PAL1
#define HUD_ATTRIBUTES ( TILE_ATTR( HUD_PALETTE, FALSE, FALSE, FALSE ) )
#define HUD_PLAN PLAN_A


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 frame;        //Used for HUD blinking effect at the end of each level


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void hudReset( void )
{
    lvl         			= 0;
    call_max    			= 0;
    totalOfAnsweredCalls    = 0;
    miss_max   				= 0;
    totalOfMissedCalls		= 0;
    frame      				= 0;
    BONUS_FLAG  			= FALSE;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void hudInitialize( void )
{
    frame                = 0;
    totalOfMissedCalls   = 0;
    miss_max             = BONUS_FLAG ? 5 : 3;
    totalOfAnsweredCalls = 0;


         if (lvl <  BONUS_LEVEL_INDEX)	{ call_max = 10 + lvl * 5; }
    else if (lvl == BONUS_LEVEL_INDEX)  { call_max =  1; }
    else                              	{ call_max =  5 + lvl * 5; }


    char buffer[41];
    strbuilder( buffer, "[  ]%s  CALLS:%02d/%02d  MISS:%d/%d", levelNames[ lvl ], totalOfAnsweredCalls, call_max, totalOfMissedCalls, miss_max );

    SYS_disableInts();
    VDP_setTextPalette(HUD_PALETTE);
    VDP_drawTextBG( HUD_PLAN, buffer, 3, 1 );
    SYS_enableInts();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void hudUpdate( void )
{
	char buffer[41];

	 if( totalOfMissedCalls < miss_max || (frame & 16) )
	{
		strbuilder( buffer, "%d", totalOfMissedCalls );
	}
	else
	{
		strbuilder( buffer, " " );
	}

	SYS_disableInts();
	VDP_drawTextBG( HUD_PLAN, buffer, 34, 1 );
	SYS_enableInts();

    if( totalOfAnsweredCalls < call_max || (frame & 16) )
    {
        strbuilder( buffer, "%02d", totalOfAnsweredCalls );
    }
    else
    {
    	strbuilder( buffer, "  " );
    }

    SYS_disableInts();
    VDP_drawTextBG( HUD_PLAN, buffer, 22, 1 );
    SYS_enableInts();



    frame++;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
