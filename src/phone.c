#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/helpers.h"
#include "../inc/game.h"


#include "../inc/hud.h" //Todo: PARA PLAYABLE LELVEL
#include "../inc/topic.h" //Todo: PARA ANIMACION
#include "../inc/player.h" //Todo: PARA COLISION TOPIC WIDSOM
#include "../inc/message.h" //Todo: PARA COLISION TOPIC WIDSOM


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_PHONE_RINGING( obj ) ( obj->phoneStatus >= PHONE_STATUS_RING1 && obj->phoneStatus <= PHONE_STATUS_RING3 )

#define PHONE_MAX   4
#define PHONE_POSITIONS_MAX 6

#define DELAY_AFTER_PHONETELEPORT_MAX 250

#define MAX_SFX_RING_TIMEOUT_1 70
#define MAX_SFX_RING_TIMEOUT_2 60


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


typedef enum
{
	PHONE_STATUS_IDLE		= 0,
	PHONE_STATUS_RING1		= 1,
	PHONE_STATUS_RING2		= 2,
	PHONE_STATUS_RING3		= 3,
	PHONE_STATUS_EXPLODE	= 4,
	PHONE_STATUS_MISS		= 5,
	PHONE_STATUS_PICKUP 	= 6

} phoneStatus;

typedef struct
{
	Sprite* phoneSprite;
	u16 delayBeforeNextPhoneStatus;
	phoneStatus phoneStatus;

	u8 ringDuration;

	Sprite* topicSprite;
	u8 topicType;

	u16 playingSfx;
	u16 ringSfxTimeout;

} phoneStruct;


static phoneStruct allPhones[ PHONE_MAX ];
static u8 totalOfIngamePhones;

static u8 totalOfPhonePositions;
static Vect2D_s16 allPhonePositions[ PHONE_POSITIONS_MAX ];
static u8 currentPhonePositionId;


static u16 currentBaseDelayBeforePhoneRing;
static const u16 baseDelayBeforePhoneRingByLevel[ PLAYABLE_LEVELS ] =
{
	250,	250,	250,	250,
	0,		275,	400,	450
};

static u8 delayBeforePhoneTeleportInitializer;
static u8 delayBeforePhoneTeleport;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void pickUpPhone( u8 phoneId )
{
	if ( phoneId < totalOfIngamePhones )
	{
		phoneStruct *thisPhone = &allPhones[ phoneId ];

		//playSfx(bonus?SFX_EXPLODE:SFX_ANSWER);
		playSfx(SFX_ANSWER);

		if ( !IS_LEVEL_BONUS )
		{
			displayRandomMessageByTopic( thisPhone->topicType );
		}

		thisPhone->phoneStatus = PHONE_STATUS_PICKUP;
		thisPhone->delayBeforeNextPhoneStatus = 25;

		totalOfAnsweredCalls++;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 getNewRandomPhonePositionId( void )
{
	u8 newPhonePositionId;

	do
	{
		newPhonePositionId = ( random( ) % totalOfPhonePositions );
	}
	while( newPhonePositionId == currentPhonePositionId );

	return newPhonePositionId;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void reloadDelayBeforePhoneTeleport( void )
{
	if ( delayBeforePhoneTeleportInitializer < DELAY_AFTER_PHONETELEPORT_MAX )
	{
		delayBeforePhoneTeleportInitializer += 10;
	}

	delayBeforePhoneTeleport = delayBeforePhoneTeleportInitializer;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void movePhone( u8 phoneId, u8 positionId )
{
	if ( phoneId < totalOfIngamePhones && positionId < totalOfPhonePositions )
	{
		phoneStruct *thisPhone = &allPhones[ phoneId ];
		Vect2D_s16 *thisPosition = &allPhonePositions[ positionId ];

		thisPhone->ringDuration = 0;
		thisPhone->delayBeforeNextPhoneStatus = 0;

		SPR_setPosition( thisPhone->phoneSprite, thisPosition->x, thisPosition->y );
		currentPhonePositionId = positionId;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void checkForPhoneTeleport( void )
{
	if ( IS_LEVEL_BONUS )
	{
		if ( delayBeforePhoneTeleport == 0 )
		{
			u8 phoneId = 0;
			phoneStruct *thisPhone = &allPhones[ phoneId ];

			s16 phonePositionX = thisPhone->phoneSprite->x - 128;
			s16 phonePositionY = thisPhone->phoneSprite->y - 128;

			s16 playerPositionX = ( playerFixedPositionX >> FP_BITS );
			s16 playerPositionY = ( playerFixedPositionY >> FP_BITS );

			if ( playerPositionY == ( phonePositionY - 1 ) )
			{
				if ( !( ( playerPositionX + 48 ) < phonePositionX || playerPositionX > ( phonePositionX + 32 ) ) )
				{
					u8 newPhonePositionId = getNewRandomPhonePositionId( );

					movePhone( phoneId, newPhonePositionId );
					playSfx(SFX_TELEPORT);

					reloadDelayBeforePhoneTeleport( );
				}
			}
		}
		else
		{
			delayBeforePhoneTeleport--;
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void addNewPhonePosition( s16 positionX, s16 positionY )
{
	if ( totalOfPhonePositions < PHONE_POSITIONS_MAX )
	{
		Vect2D_s16 *thisPosition = &allPhonePositions[ totalOfPhonePositions ];

		thisPosition->x = positionX;
		thisPosition->y = positionY;

		totalOfPhonePositions++;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void reloadPhone( phoneStruct *phone )
{
	u8 randomDelayBeforePhoneRing = ( random( ) & 63 );

	phone->topicType	= getRandomTopic( );
	phone->phoneStatus	= IS_LEVEL_BONUS ? PHONE_STATUS_RING3 : PHONE_STATUS_IDLE;
	phone->delayBeforeNextPhoneStatus = currentBaseDelayBeforePhoneRing + randomDelayBeforePhoneRing;
	phone->ringDuration	= 0;
	phone->playingSfx	= 0;
	phone->ringSfxTimeout = 0;

	if ( currentBaseDelayBeforePhoneRing > 125 )
	{
		currentBaseDelayBeforePhoneRing -= 10;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateGraphicsPhone( phoneStruct *phone )
{
	u8 animationNumber, frameNumber;

	u8 isPhonePickedUp = ( phone->phoneStatus == PHONE_STATUS_PICKUP );
	u8 isPhoneBlowingUp = ( phone->phoneStatus == PHONE_STATUS_EXPLODE );


	//Phone
	if( isPhoneBlowingUp )
	{
		animationNumber = phone->phoneStatus;
		frameNumber 	= ( phone->delayBeforeNextPhoneStatus >> 3 );
		frameNumber 	= ( frameNumber > 3 ) ? 3 : frameNumber;
	}
	else if ( isPhonePickedUp )
	{
		animationNumber = 5;
		frameNumber 	= ( phone->ringDuration & 32 ) ? 0 : (( phone->delayBeforeNextPhoneStatus >> 2 ) & 3);
	}
	else
	{
		animationNumber = phone->phoneStatus;
		frameNumber 	= ( phone->ringDuration & 32 ) ? 0 : (( phone->delayBeforeNextPhoneStatus >> 2 ) & 3);
	}

	SPR_setAnimAndFrame( phone->phoneSprite, animationNumber, frameNumber );

	//Topic
	if ( !IS_LEVEL_BONUS )
	{
		frameNumber		= IS_PHONE_RINGING( phone ) ? TOPIC_FRAME_VISIBLE : TOPIC_FRAME_HIDDEN;
		animationNumber	=  phone->topicType;
		SPR_setAnimAndFrame( phone->topicSprite, animationNumber, frameNumber );
	}
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateSfxPhone( phoneStruct *phone )
{
	u8 isPhonePickedUp  = ( ( phone->phoneStatus == PHONE_STATUS_PICKUP  ) && ( phone->playingSfx != PHONE_STATUS_PICKUP  ) );
	u8 isPhoneBlowingUp = ( ( phone->phoneStatus == PHONE_STATUS_EXPLODE ) && ( phone->playingSfx != PHONE_STATUS_EXPLODE ) );
	u8 isPhoneRinging   = ( IS_PHONE_RINGING( phone ) ) ;

	//Phone
	if ( isPhoneBlowingUp )
    {
        playSfx ( SFX_EXPLODE );
        phone->playingSfx = PHONE_STATUS_EXPLODE;
	}
	else if ( isPhonePickedUp )
    {
        playSfx ( SFX_ANSWER );
        phone->playingSfx = PHONE_STATUS_PICKUP;
	}
	else if ( isPhoneRinging )
    {
        if ( phone->ringSfxTimeout == 0 )
        {
            playSfx ( SFX_RING );

            if ( phone->phoneStatus == PHONE_STATUS_RING1 )
            {
                phone->ringSfxTimeout = MAX_SFX_RING_TIMEOUT_1;
            }
            else
            {
                phone->ringSfxTimeout = MAX_SFX_RING_TIMEOUT_2;
            }
        }

        --phone->ringSfxTimeout;
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void addNewPhone( s16 positionX, s16 positionY )
{
	if ( totalOfIngamePhones < PHONE_MAX )
	{
		phoneStruct *thisPhone = &allPhones[ totalOfIngamePhones ];

		reloadPhone( thisPhone );

		thisPhone->delayBeforeNextPhoneStatus += ( totalOfIngamePhones << 8 );
		thisPhone->phoneSprite = SPRFactory.createPhone( positionX, positionY );

		if ( !IS_LEVEL_BONUS )
		{
			s16 phonePositionX = positionX;
			s16 phonePositionY = positionY -12;

			thisPhone->topicSprite = SPRFactory.createTopic( phonePositionX, phonePositionY );
		}

		updateGraphicsPhone( thisPhone );

		totalOfIngamePhones++;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void phoneReset( void )
{
	totalOfIngamePhones = 0;
	currentBaseDelayBeforePhoneRing = baseDelayBeforePhoneRingByLevel[ lvl ];

	delayBeforePhoneTeleportInitializer = 0;
	delayBeforePhoneTeleport = delayBeforePhoneTeleportInitializer;

	totalOfPhonePositions = 0;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void phoneAdd( s16 positionX, s16 positionY )
{
	addNewPhonePosition( positionX, positionY );

	u8 phoneShouldBeCreated = !( IS_LEVEL_BONUS && totalOfIngamePhones != 0 );

	if ( phoneShouldBeCreated )
	{
		addNewPhone( positionX, positionY );
	}
	else
	{
		u8 positionId	= totalOfPhonePositions-1;
		u8 phoneId		= 0;

		movePhone( phoneId, positionId );
	}
}

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


u8 phoneCheckCollision( s16 objectPositionX, s16 objectPositionXX, s16 objectPositionY, s16 objectPositionYY )
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

	u8 isCollided = FALSE;

	u8 phoneId;
	for ( phoneId = 0; phoneId < totalOfIngamePhones; phoneId++ )
	{
		phoneStruct *thisPhone = &allPhones[ phoneId ];

		if ( IS_PHONE_RINGING( thisPhone ) )
		{
			u8 isPhonePickeable = ( thisPhone->topicType == getActiveTopicType( ) );

			if( isPhonePickeable || IS_PLAYER_WISDOM || IS_LEVEL_BONUS )
			{

				s16 phonePositionX 	= thisPhone->phoneSprite->x;
				s16 phonePositionXX = phonePositionX + thisPhone->phoneSprite->frame->w;
				s16 phonePositionY 	= thisPhone->phoneSprite->y;
				s16 phonePositionYY	= phonePositionY + thisPhone->phoneSprite->frame->h;

				isCollided = !( phonePositionX > objectPositionXX || phonePositionXX < objectPositionX || phonePositionY > objectPositionYY || phonePositionYY < objectPositionY );

				if ( isCollided )
				{
					pickUpPhone( phoneId );
				}
			}
		}
	}

	return isCollided;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void phoneUpdate( void )
{
	checkForPhoneTeleport( );

	u8 phoneId;

	for ( phoneId = 0; phoneId < totalOfIngamePhones; phoneId++ )
	{
		phoneStruct *thisPhone = &allPhones[ phoneId ];

		if ( IS_PHONE_RINGING( thisPhone ) )
		{
			thisPhone->ringDuration++;
		}

		if ( thisPhone->delayBeforeNextPhoneStatus != 0 )
		{
			thisPhone->delayBeforeNextPhoneStatus--;
		}
		else
		{
			if( !IS_LEVEL_BONUS )
			{
				thisPhone->phoneStatus++;
			}

			switch ( thisPhone->phoneStatus )
			{
				case PHONE_STATUS_RING1:
				case PHONE_STATUS_RING2:
					thisPhone->delayBeforeNextPhoneStatus = 191;    // 3 ringing cycles
					break;

				case PHONE_STATUS_RING3:
					thisPhone->delayBeforeNextPhoneStatus = 159;    // 2.5 ringing cycles
					break;

				case PHONE_STATUS_EXPLODE:
					thisPhone->delayBeforeNextPhoneStatus = 39;
					break;


				case PHONE_STATUS_MISS:
					if ( !GOD_MODE_FLAG )
                    {
                        totalOfMissedCalls++;
                    }
					reloadPhone( thisPhone );
					break;

				default:    //after pick-up animation
					reloadPhone( thisPhone );
			}
		}

		updateGraphicsPhone( thisPhone );
		updateSfxPhone( thisPhone );
	}
}
