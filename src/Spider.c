#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/SPRFactory.h"

#include "../inc/Spider.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite* sprSpider;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void spiderAdd( s16 basePositionX, s16 basePositionY )
{
    sprSpider = SPRFactory.createSpider ( basePositionX, basePositionY );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void spiderUpdate( void )
{
    if ( vtimer % 27 )
    {
        return;
    }

    SPR_nextFrame( sprSpider );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
