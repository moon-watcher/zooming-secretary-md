#include <genesis.h>
#include "../inc/player.h"
#include "../inc/common.h"
#include "../inc/map.h"
#include "../inc/SPRFactory.h"
#include "../inc/helpers.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_NPC_WAITING (waitUntilAppearsAgain != 0)
#define manboxHeight ( npcManboxSprite->frame->h )
#define manboxWidth ( npcManboxSprite->frame->w )
#define manboxPositionYY ( manboxPositionY + manboxHeight )
#define manboxPositionXX ( manboxPositionX + manboxWidth )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *npcManboxSprite;

static s16 manboxPositionX;
static s16 manboxPositionY;
static  u8 manboxDirection;
static  u8 manboxSpeed;
static u16 waitUntilAppearsAgain;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void warpManboxNPC( u8 warpDirection )
{
	if ( warpDirection ==  DIR_RIGHT )
	{
		manboxPositionY = getRandomRightFloorPositionY( ) - manboxHeight;
		manboxDirection = DIR_LEFT;
	}
	else
	{
		manboxPositionY = getRandomLeftFloorPositionY( )  - manboxHeight;
		manboxDirection = DIR_RIGHT;
	}

	waitUntilAppearsAgain = 100;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void moveManboxNPC( void )
{
	if ( manboxDirection == DIR_LEFT )
	{
		manboxPositionX -= manboxSpeed;

		if ( manboxPositionX <= -manboxWidth )
		{
			warpManboxNPC( manboxDirection );
		}

		if ( map_getTile( manboxPositionX - 4, manboxPositionYY ) == NULL )
		{
			manboxDirection = DIR_RIGHT;
		}
	}


	else if( manboxDirection == DIR_RIGHT )
	{
		manboxPositionX += manboxSpeed;

		if ( manboxPositionX >= VDP_getScreenWidth() )
		{
			warpManboxNPC( manboxDirection );
		}

		if ( map_getTile( manboxPositionX + 20, manboxPositionYY ) == NULL )
		{
			manboxDirection = DIR_LEFT;
		}
	}


	const u8 shouldBeFlipped = ( manboxDirection == DIR_LEFT ) ? FALSE : TRUE;

	u8 frameNumber;

	if( shouldBeFlipped )
	{
		frameNumber = ( manboxPositionX >> 2 ) & 3;
	}
	else
	{
		frameNumber = 3 - ( ( manboxPositionX >> 2 ) & 3 );
	}


	SPR_setPosition( npcManboxSprite, manboxPositionX, manboxPositionY );
	SPR_setFrame( npcManboxSprite, frameNumber );
	SPRFactory.setHFlip( npcManboxSprite, shouldBeFlipped );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void checkForPlayerCollision( void )
{
	const s16 playerPositionY = ( playerFixedPositionY >> FP_BITS );
	const s16 playerPositionX = ( playerFixedPositionX >> FP_BITS );

	if ( playerPositionY == manboxPositionY )
	{
		if ( ABS( playerPositionX - manboxPositionX) < 16 )
		{
			if ( !IS_PLAYER_KNOCKED )
			{
				playerKnockedDuration = 48;
				player_knocked_ani = 8;

				playerFixedPositionX += ( playerPositionX < manboxPositionX ) ? (-12<<FP_BITS) : (12<<FP_BITS) ;
				manboxDirection = ( manboxDirection == DIR_LEFT ) ? DIR_RIGHT : DIR_LEFT;
				player_dir = ( playerPositionX < manboxPositionX ) ? DIR_RIGHT : DIR_LEFT;
				playSfx(SFX_KNOCK);
			}
			else
			{
				playerKnockedDuration++;
			}
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcManboxInitialize( s16 positionX, s16 positionY )
{
	npcManboxSprite	= SPRFactory.createManboxNPC( positionX, positionY );
	manboxPositionX = positionX;
	manboxPositionY = positionY;
	manboxSpeed		= 1;
	manboxDirection = DIR_LEFT;

	waitUntilAppearsAgain = 0;

	const u16 *npcPalette	=	npcManboxSprite->definition->palette->data;
	VDP_setPalette(PAL3, npcPalette);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcManboxUpdate( void )
{
	if ( IS_NPC_WAITING )
	{
		waitUntilAppearsAgain--;
	}
	else
	{
		checkForPlayerCollision( );

		if ( vtimer & 1 )
		{
			moveManboxNPC( );
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
