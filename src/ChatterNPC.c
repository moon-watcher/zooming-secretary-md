#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/player.h"
#include "../inc/topic.h"
#include "../inc/helpers.h"
#include "../inc/display.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define RANDOM_WALKING_DURATION ( 25 + ( random( ) & 31 ) )


#define IS_NPC_WALKING ( walkingDuration != 0 )

#define ANIMATION_WALKING 0
#define ANIMATION_SPEAKING 1
#define ANIMATION_GLASSES 2
#define ANIMATION_STAND 3 //TODO CAMBIAR POR 3


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite *npcChatterSprite;


static s16 npcChatterPositionX;
static s16 npcChatterPositionY;
static u8 npcChatterDirection;
static s8 npcChatterSpeed;

static s8 speakingDuration;
static s16 glassesDuration;
static u16 walkingDuration;
static u8 isSpeakingNPC;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcChatterInitialize( s16 positionX, s16 positionY )
{
	npcChatterSprite	=	SPRFactory.createChatterNPC( positionX, positionY );

	npcChatterPositionX	=	positionX;
	npcChatterPositionY	=	positionY;
	npcChatterSpeed		=	1;
	npcChatterDirection = 	DIR_LEFT;

	walkingDuration		= 20;
	speakingDuration	= 0;
	glassesDuration		= 0;
	isSpeakingNPC		= FALSE;

	preparePal(PAL3, npcChatterSprite->definition->palette->data);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateSpeakingGraphics( void )
{
	const s16 playerPositionX	= ( playerFixedPositionX >> FP_BITS );
	const u8 shouldBeFlipped	= ( playerPositionX > npcChatterPositionX );

	u8 frameNumber;
	u8 animationNumber;

	if( speakingDuration >= 0 )
	{
		frameNumber		= ( speakingDuration >> 3 ) % 3;
		animationNumber	= ANIMATION_SPEAKING;
		speakingDuration--;
	}
	else
	{
		frameNumber		= ( glassesDuration >> 3 ) % 7;
		animationNumber	=  ANIMATION_GLASSES;
		glassesDuration++;
	}


	isSpeakingNPC = ( glassesDuration < 56 );


	SPR_setAnimAndFrame( npcChatterSprite, animationNumber, frameNumber );
	SPRFactory.setHFlip( npcChatterSprite, shouldBeFlipped );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateWalkingGraphics( void )
{
	u8 animationNumber;
	u8 frameNumber;
	u8 shouldBeFlipped;

	if( npcChatterDirection == DIR_NONE )
	{
		animationNumber = ANIMATION_STAND;
		frameNumber 	= 0;
		shouldBeFlipped	= ( vtimer & 128 ) ? TRUE : FALSE;
	}
	else
	{
		animationNumber = ANIMATION_WALKING;
		frameNumber 	= ( npcChatterPositionX >> 3 ) & 7;
		shouldBeFlipped	= ( npcChatterDirection == DIR_RIGHT ) ? TRUE : FALSE;

		if( !shouldBeFlipped ){ frameNumber = 7 - frameNumber; }
	}


	SPR_setAnimAndFrame( npcChatterSprite, animationNumber, frameNumber );
	SPRFactory.setHFlip( npcChatterSprite, shouldBeFlipped );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void moveChatterNPC( )
{
	if( npcChatterDirection == DIR_LEFT )
	{
		npcChatterPositionX -= npcChatterSpeed;

		if ( npcChatterPositionX < 8 )
		{
			npcChatterDirection = DIR_RIGHT;
			walkingDuration = RANDOM_WALKING_DURATION;
		}
	}
	else if( npcChatterDirection == DIR_RIGHT )
	{
		npcChatterPositionX += npcChatterSpeed;

		if ( npcChatterPositionX > ( 320 - 24 - 8 ) ) //TODO: CAMBIOAR
		{
			npcChatterDirection = DIR_LEFT;
			walkingDuration = RANDOM_WALKING_DURATION;
		}
	}

	SPR_setPosition( npcChatterSprite, npcChatterPositionX, npcChatterPositionY );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcChatterUpdate( void )
{
	if ( isSpeakingNPC )
	{
		updateSpeakingGraphics( );
	}

	else
	{
		s16 playerPositionY = ( playerFixedPositionY >> FP_BITS );
		s16 playerPositionX = ( playerFixedPositionX >> FP_BITS );

		// CHECK FOR PLAYER COLLISION
		if ( playerPositionY == npcChatterPositionY )
		{
			npcChatterDirection = ( playerPositionX < npcChatterPositionX ) ? DIR_LEFT : DIR_RIGHT;
			walkingDuration++;

			if ( !( ( npcChatterPositionX - 16 ) >= ( playerPositionX + 16 ) || ( npcChatterPositionX + 32 ) < playerPositionX ) )
			{
					setRandomActiveTopic( );
					npcChatterDirection = DIR_NONE;
					speakingDuration	= 95;
					glassesDuration		= 0;
					isSpeakingNPC		= TRUE;
					walkingDuration		= 0;
					playSfx(SFX_BLA2);
			}
		}


		else if ( vtimer & 1 )
		{
				return;
		}



		if ( IS_NPC_WALKING )
		{
			moveChatterNPC( );
			updateWalkingGraphics( );

			walkingDuration--;
		}
		else
		{
			if ( npcChatterDirection != DIR_NONE )
			{
				npcChatterDirection = DIR_NONE;
			}
			else
			{
				if ( npcChatterPositionX < 64 )
				{
					npcChatterDirection = DIR_RIGHT;
				}

				if ( npcChatterPositionX > ( 320 - 64 ) ) //TODO AÑADIR WIDTH
				{
					npcChatterDirection = DIR_LEFT;
				}

				if ( npcChatterDirection == DIR_NONE )
				{
					npcChatterDirection = ( random( ) & 1 ) ? DIR_RIGHT : DIR_LEFT;
				}
			}

			walkingDuration = RANDOM_WALKING_DURATION;
		}

	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
