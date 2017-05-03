#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/player.h"
#include "../inc/game.h"
#include "../inc/display.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_GHOST_WAITING ( delayBeforeGhostMove != 0 )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *npcGhostSprite;
static u8 delayBeforeGhostMove;
static s16 npcGhostFixedPositionX;
static s16 npcGhostFixedPositionY;
static s8 npcGhostFixedSpeedX;
static s8 npcGhostFixedSpeedY;
static u16 npcGhostFrameCounter;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

static u8 checkForCollisionWithPlayer( void )
{
    if ( GOD_MODE_FLAG )
    {
        return 0;
    }

	const s16 npcPositionX = ( npcGhostFixedPositionX >> FP_BITS );
	const s16 npcPositionY = ( npcGhostFixedPositionY >> FP_BITS );

	const u16 playerWith = playerSprite->frame->w;
	const u16 playerHeight = playerSprite->frame->h;

	const s16 playerPositionX = ( playerFixedPositionX >> FP_BITS );
	const s16 playerPositionXX = playerPositionX + playerWith;
	const s16 playerPositionY = ( playerFixedPositionY >> FP_BITS );
	const s16 playerPositionYY = playerPositionY + playerHeight;

	return ( !( npcPositionX + 14 > playerPositionXX || npcPositionX + 18 < playerPositionX || npcPositionY + 28 < playerPositionY || playerPositionYY < npcPositionY + 14 ) );
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

static void updateGraphicsGhostNPC( void )
{
	const s16 playerWith = playerSprite->frame->w;
	const s16 playerHeight = playerSprite->frame->h;
	const s16 playerX = ( playerFixedPositionX >> FP_BITS ) + ( playerWith >> 1 );
	const s16 playerY = ( playerFixedPositionY >> FP_BITS ) + ( playerHeight >> 1 );

	const s16 ghostWith = npcGhostSprite->frame->w;
	const s16 ghostHeight = npcGhostSprite->frame->h;
	const s16 ghostX = ( npcGhostFixedPositionX >> FP_BITS ) + ( ghostWith >> 1 );
	const s16 ghostY = ( npcGhostFixedPositionY >> FP_BITS ) + ( ghostHeight >> 1 );

	const u16 horizontalDistance = ABS( ghostX - playerX );
	const u16 verticalDistance = ABS( ghostY - playerY );
	const u16 totalDistance = ( horizontalDistance + verticalDistance );

	const u8 needToBeFlipped = ( npcGhostFixedPositionX > playerFixedPositionX ) ? TRUE : FALSE;
	const u8 frameNumber = ( npcGhostFrameCounter & 8 ) ? 0 : 1;
	u8 animationNumber;

	if ( totalDistance > 110 )
	{
		animationNumber = 0;
	}

	else if ( totalDistance > 60 )
	{
		animationNumber = 1;
	}

	else
	{
		animationNumber = 2;
	}

	SPRFactory.setHFlip( npcGhostSprite, needToBeFlipped );
	SPR_setAnimAndFrame( npcGhostSprite, animationNumber, frameNumber );
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

static void moveGhostNPC( void )
{
	if ( playerFixedPositionX < npcGhostFixedPositionX )
	{
		if ( npcGhostFixedSpeedX > -16 ) /* = -2px */
		{
			npcGhostFixedSpeedX--;
		}
	}
	else
	{
		if ( npcGhostFixedSpeedX < 16 ) /* = 2px */
		{
			npcGhostFixedSpeedX++;
		}
	}

	if ( playerFixedPositionY < npcGhostFixedPositionY )
	{
		if ( npcGhostFixedSpeedY > -12 ) /* = -1.5px */
		{
			npcGhostFixedSpeedY--;
		}
	}
	else
	{
		if ( npcGhostFixedSpeedY < 12 ) /* = 1.5px */
		{
			npcGhostFixedSpeedY++;
		}
	}

	npcGhostFixedPositionX += npcGhostFixedSpeedX;
	npcGhostFixedPositionY += npcGhostFixedSpeedY;

	const s16 npcGhostPositionX = ( npcGhostFixedPositionX >> FP_BITS );
	const s16 npcGhostPositionY = ( npcGhostFixedPositionY >> FP_BITS );

	SPR_setPosition( npcGhostSprite, npcGhostPositionX, npcGhostPositionY );

}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

void npcGhostInitialize( s16 positionX, s16 positionY )
{
	npcGhostSprite = SPRFactory.createGhostNPC( positionX, positionY );

	delayBeforeGhostMove = 150;
	npcGhostFrameCounter = 0;
	npcGhostFixedPositionX = ( positionX << FP_BITS );
	npcGhostFixedPositionY = ( positionY << FP_BITS );
	npcGhostFixedSpeedX = ( 1 << FP_BITS );
	npcGhostFixedSpeedY = ( 1 << FP_BITS );

	preparePal( PAL3, npcGhostSprite->definition->palette->data );
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

void npcGhostUpdate( void )
{
	if ( IS_GHOST_WAITING )
	{
		delayBeforeGhostMove--;
	}
	else
	{
		isPlayerCatched = checkForCollisionWithPlayer( );

		if ( !isPlayerCatched )
		{
			moveGhostNPC( );
			updateGraphicsGhostNPC( );
		}
	}

	npcGhostFrameCounter++;
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
