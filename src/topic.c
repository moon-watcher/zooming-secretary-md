#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/helpers.h"

#include "../inc/topic.h"
#include "../inc/player.h" //TODO: used for playerwidsom topic active info
#include "../inc/hud.h" //TODO: used for playerwidsom topic active info


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_ACTIVE_TOPIC_INITIALIZED ( currentActiveTopic.type != TOPIC_TYPE_NONE )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


typedef struct
{
	Sprite *sprite;
	u8 type;
	u8 flashCounter;

} topicStruct;


static topicStruct currentActiveTopic;
static u16 activeTopicFrameCounter;

static const u8 availableTopicsByLevel[ PLAYABLE_LEVELS ] =
{
	2,	2,	3,	3,	1,	4,	4,	4
};
static u8 totalOfAvailableTopics;

static u8 totalOfFileCabinets;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void activeTopicReset( )
{
	activeTopicFrameCounter = 0;

	currentActiveTopic.type 		= TOPIC_TYPE_NONE;
	currentActiveTopic.flashCounter = 0;
//	currentActiveTopic.sprite 		= SPRFactory.createTopicHidden( 32, 8 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void activeTopicUpdate( )
{
	if ( IS_PLAYER_WISDOM || IS_LEVEL_BONUS )
	{
		u8 frameNumer		= ( activeTopicFrameCounter & 4 ) ? TOPIC_FRAME_HIDDEN : TOPIC_FRAME_VISIBLE;
		u8 animationNumber	= TOPIC_TYPE_BONUS;

		activeTopicFrameCounter++;

		SPR_setAnimAndFrame( currentActiveTopic.sprite, animationNumber, frameNumer );
	}
	else if ( IS_ACTIVE_TOPIC_INITIALIZED )
	{
		if( currentActiveTopic.flashCounter )
		{
			currentActiveTopic.flashCounter--;

			u8 frameNumber		= ( currentActiveTopic.flashCounter & 16 ) ? TOPIC_FRAME_HIDDEN : TOPIC_FRAME_VISIBLE;
			u8 animationNumber	= currentActiveTopic.type;

			SPR_setAnimAndFrame( currentActiveTopic.sprite, animationNumber, frameNumber );
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void changeActiveTopicType( u8 topicType )
{
	currentActiveTopic.type			= topicType;
	currentActiveTopic.flashCounter = 160;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


u8 getRandomTopic( void )
{
	return ( random( ) % totalOfAvailableTopics );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

void setRandomActiveTopic( void )
{
	u8 newTopic;
	u8 previousActiveTopic = getActiveTopicType( );

	do
	{
		newTopic = getRandomTopic( );

	} while( previousActiveTopic == newTopic );

	changeActiveTopicType( newTopic );
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


u8 getActiveTopicType( void )
{
	return currentActiveTopic.type;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */




static topicStruct topic[ TOPIC_MAX ];    // Available topics in the current level





void topic_reset( void )
{
	totalOfAvailableTopics	= availableTopicsByLevel[ lvl ];
	totalOfFileCabinets		= 0;

	activeTopicReset( );
}

void topic_init( s16 positionX, s16 positionY, u8 topicType )
{
	if ( totalOfFileCabinets < TOPIC_MAX )
	{
		topicStruct *thisTopic = &topic[ totalOfFileCabinets ];

		thisTopic->type			= topicType;
		thisTopic->flashCounter	= 0;
		thisTopic->sprite		= SPRFactory.createTopicByType( positionX, positionY, topicType );

		totalOfFileCabinets++;
	}
}

void topic_step( void )
{
	u8 topicId;

	for ( topicId = 0; topicId < totalOfFileCabinets; topicId++ )
	{
		topicStruct *thisTopic = &topic[ topicId ];

		if ( thisTopic->flashCounter != 0 )
		{
			u8 frameNumber = ( thisTopic->flashCounter & 2 ) ? TOPIC_FRAME_BLINK : TOPIC_FRAME_VISIBLE;
			SPR_setFrame( thisTopic->sprite,  frameNumber );

			thisTopic->flashCounter--;
		}
	}

	activeTopicUpdate( );
}


u8 topic_checkCollision( s16 spr_x, s16 spr_xx, s16 spr_y, s16 spr_yy )
{
	/*
	 (x,y)
		 o------------
		 |           |
		 |           |
		 |           |
		 ------------o
					 (xx,yy)
	 */

	u8 collision = FALSE;


	if( !IS_PLAYER_WISDOM )
	{
		u8 topicId;

		for ( topicId = 0; topicId < totalOfFileCabinets; topicId++ )
		{
			if ( topic[ topicId ].type != currentActiveTopic.type )
			{
				s16 x 	= topic[ topicId ].sprite->x;
				s16 xx 	= topic[ topicId ].sprite->x + topic[ topicId ].sprite->frame->w;
				s16 y 	= topic[ topicId ].sprite->y;
				s16 yy 	= topic[ topicId ].sprite->y + topic[ topicId ].sprite->frame->h;

				if ( !( x > spr_xx || xx < spr_x || y > spr_yy || yy < spr_y ) )
				{
					collision = TRUE;
					topic[ topicId ].flashCounter = 16;

					changeActiveTopicType( topic[ topicId ].type );
					playSfx(SFX_TOPIC);
				}
			}
		}
	}

	return collision;
}
