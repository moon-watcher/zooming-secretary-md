#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/player.h"
#include "../inc/map.h"
#include "../inc/display.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define FRAME_SHOVE_FRONT 0
#define FRAME_SHOVE_BACK 1

#define ANIMATION_WALK 0
#define ANIMATION_PUSH 1


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite* npcBouncerSprite;

static s16 npcBouncerPositionX;
static s16 npcBouncerPositionY;
static s8 npcBouncerSpeed;

static u8 npcBouncerDirection;
static u16 npcBouncerAICounter;
static u8 npcBouncerIsPushing;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateBouncerNPCGraphics( void )
{
	const u8 shouldBeHFlipped = ( npcBouncerDirection == DIR_RIGHT );
	const u8 animationNumber  = ( npcBouncerIsPushing ) ? ANIMATION_PUSH : ANIMATION_WALK;
	u8 frameNumber            = ( npcBouncerPositionX >> 2 ) & 3;

	if ( npcBouncerDirection == DIR_LEFT )
    {
        frameNumber = ABS ( frameNumber - 3 );
    }

	SPR_setAnimAndFrame( npcBouncerSprite, animationNumber, frameNumber );
	SPRFactory.setHFlip( npcBouncerSprite, shouldBeHFlipped );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void moveBouncerNPC( void )
{
	const u16 npcBouncerWidth	= npcBouncerSprite->frame->w;
	const u16 npcBouncerHeight	= npcBouncerSprite->frame->h;

	if ( npcBouncerDirection == DIR_LEFT )
	{
		npcBouncerPositionX -= npcBouncerSpeed;

		if ( npcBouncerPositionX + npcBouncerWidth <= 0 )
		{
			npcBouncerPositionY = getRandomLeftFloorPositionY( ) - npcBouncerHeight;
			npcBouncerDirection = DIR_RIGHT;
		}
	}

	else    // DIR_RIGHT
	{
		npcBouncerPositionX += npcBouncerSpeed;

		if ( npcBouncerPositionX >= VDP_getScreenWidth( ) )
		{
			npcBouncerPositionY = getRandomRightFloorPositionY( ) - npcBouncerHeight;
			npcBouncerDirection = DIR_LEFT;
		}
	}

	SPR_setPosition( npcBouncerSprite, npcBouncerPositionX, npcBouncerPositionY );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void checkCollisionWithPlayer( void )
{
	const s16 playerPositionX = ( playerFixedPositionX >> FP_BITS );
	const s16 playerPositionY = ( playerFixedPositionY >> FP_BITS );
//	const u16 npcBouncerWidth = npcBouncerSprite->frame->w;

	npcBouncerIsPushing = FALSE;

//	if ( !( npcBouncerPositionX < 0 || ( playerPositionX + npcBouncerWidth ) > VDP_getScreenWidth( )  ) )
	{
		if ( playerPositionY == npcBouncerPositionY )
		{
			if ( playerPositionX < npcBouncerPositionX )
			{
				if ( playerPositionX + 16 >= npcBouncerPositionX )
				{
				    npcBouncerIsPushing = ( npcBouncerDirection == DIR_LEFT );
					playerFixedPositionX = ( npcBouncerPositionX - 16 ) << FP_BITS;
				}
			}
			else
			{
				if ( playerPositionX <= npcBouncerPositionX + 16 )
				{
				    npcBouncerIsPushing = ( npcBouncerDirection == DIR_RIGHT );
					playerFixedPositionX = ( npcBouncerPositionX + 16 ) << FP_BITS;
				}
			}
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcBouncerInitialize( s16 positionX, s16 positionY )
{
	npcBouncerSprite = SPRFactory.createBouncerNPC( positionX, positionY );

	npcBouncerPositionX = positionX;
	npcBouncerPositionY = positionY;
	npcBouncerSpeed = 1;

	npcBouncerDirection = DIR_LEFT;
	npcBouncerAICounter = 0;
	npcBouncerIsPushing = FALSE;

	preparePal( PAL3, npcBouncerSprite->definition->palette->data );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcBouncerUpdate( void )
{
	checkCollisionWithPlayer( );

	if ( !( npcBouncerAICounter & 3 ) )
	{
		moveBouncerNPC( );
	}

	updateBouncerNPCGraphics( );

	npcBouncerAICounter++;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
