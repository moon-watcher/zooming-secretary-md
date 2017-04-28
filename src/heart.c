#include <genesis.h>
#include "../inc/SPRFactory.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define HEARTS_MAX	8

#define IS_HEART_USED( obj ) ( obj->duration != 0 )
#define IS_HEART_INITIALIZED( obj ) ( obj->sprite != NULL )

#define FRAME_HEART_BIG 5
#define FRAME_HEART_SMALL 4


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


typedef struct
{
	Sprite *sprite;
	u8 duration;

} heartStruct;

static heartStruct allHearts[ HEARTS_MAX ];

static u16 heartFrameCounter;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void setHeartRandomDuration( heartStruct *heart )
{
	u8 baseDuration 	= 24;
	u8 randomDuration 	= ( random( ) & 7 );

	heart->duration = baseDuration + randomDuration;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void setHeartRandomPosition( heartStruct *heart, s16 basePositionX, s16 basePositionY )
{
	s16 horizontalOffset 	= - 8 + ( random( ) & 15 );
	s16 positionX 			= basePositionX + horizontalOffset;
	s16 positionY 			= basePositionY-7;

	SPR_setPosition( heart->sprite, positionX, positionY );
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateHeartFrame( heartStruct *heart )
{
	u8 frameNumber = FRAME_HEART_BIG;

	if( heart->duration < 10 )
	{
		frameNumber = ( heart->duration >> 1 );
	}
	else if( heart->duration < 24 )
	{
		frameNumber = FRAME_HEART_SMALL;
	}

	SPR_setFrame( heart->sprite, frameNumber );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void heartsReset( void )
{
	memset( allHearts, NULL, sizeof( allHearts ) );
	heartFrameCounter = 0;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void heartAdd( s16 basePositionX, s16 basePositionY )
{
	u8 heartIdx;

	for ( heartIdx = 0; heartIdx < HEARTS_MAX; heartIdx++ )
	{
		heartStruct *thisHeart = &allHearts[ heartIdx ];

		if ( !IS_HEART_USED( thisHeart ) )
		{
			if ( !IS_HEART_INITIALIZED( thisHeart ) )
			{
				thisHeart->sprite = SPRFactory.createHeart( basePositionX, basePositionX );
			}

			setHeartRandomDuration( thisHeart );
			setHeartRandomPosition( thisHeart, basePositionX, basePositionY );
			SPR_setFrame( thisHeart->sprite, FRAME_HEART_BIG );

			break;
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void heartsUpdate( void )
{
	if ( heartFrameCounter & 1 )
	{
		u8 heartIdx;

		for ( heartIdx = 0; heartIdx < HEARTS_MAX; heartIdx++ )
		{
			heartStruct *thisHeart = &allHearts[ heartIdx ];

			if ( IS_HEART_USED( thisHeart ) )
			{
				if ( !( heartFrameCounter & 2 ) )
				{
					s8 hOffset = ( thisHeart->duration & 8 ) ? -1 : +1;
					thisHeart->sprite->x += hOffset;
				}

				thisHeart->sprite->y--;
				thisHeart->duration--;

				updateHeartFrame( thisHeart );
			}
			else
			{
				SPR_setPosition( thisHeart->sprite, VDP_getScreenWidth( ), VDP_getScreenHeight( ) );    // Hidden
			}
		}
	}

	heartFrameCounter++;
}
