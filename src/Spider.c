#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/SPRFactory.h"

#include "../inc/Spider.h"


#define UPDATE_FREQUENCY 27


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite* sprSpider = NULL;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void spiderAdd( s16 basePositionX, s16 basePositionY )
{
    sprSpider = SPRFactory.createSpider ( basePositionX, basePositionY );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void spiderUpdate( void )
{
    if ( !sprSpider || vtimer % UPDATE_FREQUENCY )
    {
        return;
    }

    SPR_nextFrame( sprSpider );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void spiderReset ()
{
    sprSpider = NULL;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
