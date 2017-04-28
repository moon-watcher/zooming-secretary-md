#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/heart.h"
#include "../inc/player.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

#define ANIMATION_WALK 0
#define ANIMATION_LOVE 1


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite* npcGeekSprite;

static s16 npcGeekPositionX;
static s16 npcGeekPositionY;
static  s8 npcGeekSpeed;
static  u8 npcGeekDirection;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateWalkingGraphics( void )
{
	const u8 frameNumber		= ( vtimer >> 4 ) & 7;
	const u8 shouldBeFlipped	= ( npcGeekDirection == DIR_RIGHT );

	SPR_setAnimAndFrame( npcGeekSprite, ANIMATION_WALK, frameNumber );
	SPRFactory.setHFlip( npcGeekSprite, shouldBeFlipped );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void moveNpcGeek( )
{
	if( npcGeekDirection == DIR_LEFT )
	{
		npcGeekPositionX -= npcGeekSpeed;

		if ( npcGeekPositionX < 8 )
		{
			npcGeekDirection = DIR_RIGHT;
		}
	}

	else if ( npcGeekDirection == DIR_RIGHT )
	{
		npcGeekPositionX += npcGeekSpeed;

		const u16 npcGeekWidth		= npcGeekSprite->frame->w;
		const s16 npcGeekPositionXX = npcGeekPositionX + npcGeekWidth;

		if ( npcGeekPositionXX >= VDP_getScreenWidth() - 8 )
		{
			npcGeekDirection = DIR_LEFT;
		}
	}

	SPR_setPosition( npcGeekSprite, npcGeekPositionX, npcGeekPositionY );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcGeekInitialize( s16 positionX, s16 positionY )
{
	npcGeekSprite = SPRFactory.createGeekNPC( positionX, positionY );

	npcGeekPositionX = positionX;
	npcGeekPositionY = positionY;
	npcGeekSpeed = 1;

	npcGeekDirection = DIR_LEFT;

	const u16 *npcPalette = npcGeekSprite->definition->palette->data;
	VDP_setPalette( PAL3, npcPalette );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcGeekUpdate( void )
{
	const u16 npcGeekWidth 		= npcGeekSprite->frame->w;
	const s16 playerPositionX 	= ( playerFixedPositionX >> FP_BITS );
	const s16 playerPositionY	= ( playerFixedPositionY >> FP_BITS );

	const u16 horizontalDistance 	= ABS( playerPositionX - npcGeekPositionX );
	const  u8 isPlayerInFrontOffNPC = ( ( playerPositionX < npcGeekPositionX && npcGeekDirection == DIR_LEFT ) || ( playerPositionX > npcGeekPositionX && npcGeekDirection == DIR_RIGHT ) );

	if ( playerPositionY == npcGeekPositionY && isPlayerInFrontOffNPC && horizontalDistance < 64 )
	{

		if( player_dir == DIR_RIGHT &&  playerPositionX < npcGeekPositionX )
		{
			if ( ( playerPositionX + 8 ) >= npcGeekPositionX )
			{
				playerFixedPositionX = ( npcGeekPositionX - 8) << FP_BITS;

				if ( vtimer & 1 )
				{
					if ( npcGeekPositionX + npcGeekWidth < VDP_getScreenWidth() + 8 )
					{
						npcGeekPositionX += npcGeekSpeed;
					}
				}
			}

		}

		else if ( player_dir == DIR_LEFT && playerPositionX > npcGeekPositionX )
		{
			if ( playerPositionX < npcGeekPositionX + 8 )
			{
				playerFixedPositionX = ( npcGeekPositionX + 8 ) << FP_BITS;

				if ( vtimer & 1 )
				{
					if ( (npcGeekPositionX + 8) > 0 )
					{
						npcGeekPositionX -= npcGeekSpeed;
					}
				}
			}
		}


		if ( !( vtimer & 31 ) )
		{
			heartAdd( npcGeekPositionX + 8, npcGeekPositionY );
		}

		SPR_setAnimAndFrame( npcGeekSprite, ANIMATION_LOVE, ( vtimer >> 3 ) & 1 );
		SPR_setPosition( npcGeekSprite, npcGeekPositionX, npcGeekPositionY );
	}

	else if ( vtimer & 1 )
	{
		moveNpcGeek( );
		updateWalkingGraphics( );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
