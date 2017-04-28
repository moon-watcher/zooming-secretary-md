#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/player.h"
#include "../inc/common.h"
#include "../inc/helpers.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define hideBlaBlaSprite( ) SPR_setPosition( this.blablaSprite, VDP_getScreenWidth( ), VDP_getScreenHeight( ) )
#define setRandomAICounter( ) this.npcChiefAICounter = 64 + ( random( ) & 31 )

#define ANI_NPCCHIEF_WALKING ( 0 )
#define ANI_NPCCHIEF_TALKING ( 1 )
#define ANI_NPCCHIEF_STAND ( 2 )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


typedef struct
{
	Sprite* blablaSprite;
	Sprite* npcChiefSprite;

	s16 npcChiefFixedPositionY;
	s16 npcChiefFixedPositionX;
	 s8 npcChiefFixedSpeed;

	 u8 npcChiefDirection;
	u16 npcChiefAICounter;
	u16 npcChiefFrameCounter;

} npcChiefStruct;

static npcChiefStruct this;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showBlaBlaSprite( s16 basePositionX, s16 basePositionY )
{
	 u8 frameNumber		= ( random( ) & 3 );
	s16 blablaPositionX	= basePositionX - 8 + ( random( ) % 24 );
	s16 blablaPositionY	= basePositionY - 12 + ( random( ) & 3 );

	SPR_setPosition( this.blablaSprite, blablaPositionX, blablaPositionY);
	SPR_setFrame( this.blablaSprite, frameNumber );

	playSfx(SFX_BLA1);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcChiefInitialize( s16 positionX, s16 positionY )
{
	this.blablaSprite			= SPRFactory.createBlaBla( VDP_getScreenWidth( ), VDP_getScreenHeight( ) );
	this.npcChiefSprite			= SPRFactory.createChiefNPC( positionX, positionY );

	this.npcChiefFixedPositionX = ( positionX << FP_BITS );
	this.npcChiefFixedPositionY = ( positionY << FP_BITS );
	this.npcChiefFixedSpeed		= ( 1 << FP_BITS );

	this.npcChiefDirection		= DIR_LEFT;

	this.npcChiefAICounter		= 64;
	this.npcChiefFrameCounter	= 0;

	const u16 *npcPalette		= this.npcChiefSprite->definition->palette->data;
	VDP_setPalette(PAL3, npcPalette);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcChiefUpdate( void )
{
	 u8 npcChiefFrameNumber		= NULL;
	 u8 npcChiefAnimationNumber	= NULL;
	 u8 npcChiefFlip			= FALSE;

	s16 npcPositionX	= ( this.npcChiefFixedPositionX >> FP_BITS );
	s16 npcPositionY	= ( this.npcChiefFixedPositionY >> FP_BITS );
	u16 npcChiefWidth	= ( this.npcChiefSprite->frame->w );

	s16 playerPositionX = ( playerFixedPositionX >> FP_BITS );
	s16 playerPositionY = ( playerFixedPositionY >> FP_BITS );

	 u8 isPlayerVisibleVertically 	= ABS( npcPositionY - playerPositionY ) <= 8;
	 u8 isPlayerVisibleHorizontally	= ABS( npcPositionX - playerPositionX ) <= 56;

	isPlayerSlowed = ( isPlayerVisibleVertically && isPlayerVisibleHorizontally );

	if ( isPlayerSlowed )
	{
		this.npcChiefDirection = DIR_NONE;

		if ( this.npcChiefAICounter > 25 )
		{
			showBlaBlaSprite( npcPositionX, npcPositionY );

			this.npcChiefAICounter = 25;
		}
		else
		{
			if ( this.npcChiefAICounter < 10 )
			{
				hideBlaBlaSprite( );
			}

			this.npcChiefAICounter--;
		}

		npcChiefFrameNumber 	= ( this.npcChiefFrameCounter >> 3 ) & 3;
		npcChiefAnimationNumber = ANI_NPCCHIEF_TALKING;
		npcChiefFlip			= ( playerFixedPositionX < this.npcChiefFixedPositionX );
	}
	else    // PLAYER IS NOT CATCHED
	{
		hideBlaBlaSprite( );

		if ( this.npcChiefAICounter == 0 )
		{
			setRandomAICounter( );

			if ( this.npcChiefDirection != DIR_NONE )
			{
				this.npcChiefDirection = DIR_NONE;
			}

			else if ( npcPositionX < 64 )
			{
				this.npcChiefDirection = DIR_RIGHT;
			}

			else if ( ( npcPositionX + npcChiefWidth ) >  ( VDP_getScreenWidth( ) - 64 ) )
			{
				this.npcChiefDirection = DIR_LEFT;
			}

			else
			{
				this.npcChiefDirection = ( random( ) & 1 ) ? DIR_RIGHT : DIR_LEFT ;
			}
		}

		if( this.npcChiefDirection == DIR_NONE )
		{
			npcChiefFrameNumber		= 0;
			npcChiefAnimationNumber	= ANI_NPCCHIEF_STAND;
			npcChiefFlip			= ( this.npcChiefAICounter & 32 );
		}
		else
		{
			if( this.npcChiefDirection == DIR_LEFT )
			{
				this.npcChiefFixedPositionX -= this.npcChiefFixedSpeed;
				npcPositionX = (  this.npcChiefFixedPositionX >> FP_BITS );

				if ( npcPositionX <= 8 )
				{
					this.npcChiefDirection = DIR_RIGHT;
					setRandomAICounter( );
				}
			}

			else if( this.npcChiefDirection == DIR_RIGHT )
			{
				this.npcChiefFixedPositionX += this.npcChiefFixedSpeed;
				npcPositionX = (  this.npcChiefFixedPositionX >> FP_BITS );

				if ( npcPositionX + npcChiefWidth >= ( VDP_getScreenWidth( ) - 8 ) )
				{
					this.npcChiefDirection = DIR_LEFT;
					setRandomAICounter( );
				}
			}

			npcChiefFrameNumber		= ( npcPositionX >> 3 ) & 7;
			npcChiefAnimationNumber	= ANI_NPCCHIEF_WALKING;
			npcChiefFlip			= ( this.npcChiefDirection == DIR_LEFT );

			if( npcChiefFlip ) { npcChiefFrameNumber = 7 - npcChiefFrameNumber; }

		}

		this.npcChiefAICounter--;
	}

	SPRFactory.setHFlip( this.npcChiefSprite,	npcChiefFlip );
	SPR_setAnimAndFrame( this.npcChiefSprite, npcChiefAnimationNumber, npcChiefFrameNumber );
	SPR_setPosition( this.npcChiefSprite, npcPositionX, npcPositionY );

	this.npcChiefFrameCounter++;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
