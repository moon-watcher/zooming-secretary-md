#include <genesis.h>

#include "../inc/vint.h"
#include "../inc/tempo.h"
#include "../inc/psg.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static s16 scrollOffset;
static s16 updateScroll;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void VintInit ()
{
    scrollOffset = 0;
    updateScroll = 0;

	SYS_setInterruptMaskLevel( 4 );
	SYS_setVIntCallback ( (_voidCallback*) VIntCallback );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


_voidCallback *VIntCallback ( )
{
    psg_callback ( );
	XGM_setMusicTempo ( getMusicTempo() );

	if ( updateScroll )
    {
        VDP_setHorizontalScroll(PLAN_B, scrollOffset>>1 );
        VDP_setVerticalScroll(PLAN_B, scrollOffset>>1 );

        scrollOffset++;
    }

	return 0;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void VIntSetUpdateScroll ( u16 value )
{
    updateScroll = value;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
