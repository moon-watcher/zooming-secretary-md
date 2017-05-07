#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../res/rescomp.h"
#include "../inc/common.h"

#include "../inc/player.h"

#include "../inc/Coffee.h" //TODO: NECESARIO PARA colision
#include "../inc/map.h"      //TODO: NECESARIO PARA VER TIPO DE TILES
#include "../inc/topic.h"      //TODO: NECESARIO PARA colision
#include "../inc/phone.h"      //TODO: NECESARIO PARA colision
#include "../inc/heart.h" //TODO: NECESARIO PARA corazones anim
#include "../inc/hud.h" //TODO: NECESARIO PARA corazones anim
#include "../inc/game.h"
#include "../inc/helpers.h"
#include "../inc/dev.h"
#include "../inc/display.h"


static u8  playerSpeedBonus;         // Flag for speed bonus (coffee drinked)
static u8  playerSpeedBonusCounter;     // Speed bonus timmer
static u8  player_frame_cnt;           // Used for animation purposes

static s16 player_move_cnt;           // Auto-movement on ladders

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_PLAYER_INITIALIZED ( playerSprite != NULL )
#define IS_TALKING_ON_PHONE ( delayBeforeHangUp != 0 )
#define IS_PLAYER_STANDING_UP ( standingUpDuration != 0)

#define ANIMATION_PLAYER_FALLING 11
#define FRAME_PLAYER_FALLING_1 0
#define FRAME_PLAYER_FALLING_2 1


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u32 playerCurrentSpeed;
static u32 playerSpeedTo;
static u8  delayBeforeHangUp;
static u8  standingUpDuration;
static u16 playerFallingCounter;
static u16 sfxDirHorizontal;
static u16 sfxDirVertical;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u16 playerGetAminationSpeed ()
{
    const u16 baseRefresh = 6;
    const u16 baseSpeed   = 24;

    u16 aux = playerCurrentSpeed ;

    if ( !aux ) aux = 1;

    u16 vel = baseRefresh * baseSpeed / aux;

    if ( !vel ) vel = 1;

    return vel;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void playerSfxStep ( )
{
    if ( player_dir == DIR_LEFT  ||  player_dir == DIR_RIGHT )
    {
        playSfx( ++sfxDirHorizontal % 2 ? SFX_STEP_1 : SFX_STEP_2 );
    }

    if ( player_dir == DIR_UP    ||  player_dir == DIR_DOWN  )
    {
        playSfx ( ++sfxDirVertical % 2 ? SFX_STEP_3 : SFX_STEP_4 );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updateNextFrame ( )
{
    if ( !( ++player_frame_cnt % playerGetAminationSpeed() ) )
    {
        SPR_nextFrame( playerSprite );
        playerSfxStep( );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void checkForPlayerHorizontalWarp( void )
{
	if ( playerFixedPositionX <= ( -24 << FP_BITS ) )
	{
		playerFixedPositionX = ( 319 << FP_BITS );
	}

	if ( playerFixedPositionX >= ( 320 << FP_BITS ) )
	{
		playerFixedPositionX = ( -23 << FP_BITS );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updatePlayerSpeed( void )
{
	//                      1       	+			0.5        	+		[0.625 ~ 0]
	playerSpeedTo = ( 1 << FP_BITS ) 	+ ( 1 << FP_BITS >> 1 ) + ( playerSpeedBonus >> 1 );

	if ( isPlayerSlowed )
	{
		playerSpeedTo = ( (playerSpeedTo << 1) / 3 );	// Speed is now 2/3
	}

	if ( playerCurrentSpeed > playerSpeedTo )
	{
		playerCurrentSpeed--;
	}
	if ( playerCurrentSpeed < playerSpeedTo )
	{
		playerCurrentSpeed++;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 isPlayerFalling( void )
{
	s16 x = ( playerFixedPositionX >> FP_BITS );
	s16 y = ( playerFixedPositionY >> FP_BITS );

	//TODO: delete return ( map_getTile( x + 3, y + 31 ) != TILE_LADDER && map_getTile( x + 12, y + 31 ) != TILE_LADDER && map_getTile( x + 3, y + 32 ) != TILE_LADDER && map_getTile( x + 12, y + 32 ) != TILE_LADDER && map_getTile( x + 3, y + 32 ) != TILE_FLOOR && map_getTile( x + 12, y + 32 ) != TILE_FLOOR );

	u8 isOverFloor 	= ( map_getTile( x + 8, y + 32 ) == TILE_FLOOR ||  map_getTile( x + 15, y + 32 ) == TILE_FLOOR );
	u8 isOverLadder = ( map_getTile( x + 12, y + 32 ) == TILE_LADDER );

	return ( !isOverFloor && !isOverLadder );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void updatePlayerSpeedBonus( void )
{
	playerSpeedBonusCounter++;

	if ( !( playerSpeedBonusCounter & 63 ) )
	{
		if ( playerSpeedBonus ) { playerSpeedBonus--; }
	}

	const s16 x		= ( playerFixedPositionX >> FP_BITS ) + 6;
	const s16 xx	= ( playerFixedPositionX >> FP_BITS ) + 17;
	const s16 y		= ( playerFixedPositionY >> FP_BITS );
	const s16 yy	= ( playerFixedPositionY >> FP_BITS ) + 31;

	if ( coffeeCheckCollision( x, xx, y, yy ) )
	{
		playerSpeedBonus = 20;
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void  setSecretaryPriority( u8 isHighPriority )
{
	if( isHighPriority )
	{
		playerSprite->attribut |= ( 1<<15 );
	}
	else
	{
		playerSprite->attribut &= ~( 1<<15 );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void  setSecretaryHFlip( u8 isFlippedHorizontally )
{
	if( isFlippedHorizontally )
	{
		playerSprite->attribut |= ( 1<<11 );
	}
	else
	{
		playerSprite->attribut &= ~( 1<<11 );
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 isPlayerOnLadder( void )
{
	s16 x = ( playerFixedPositionX >> FP_BITS );
	s16 y = ( playerFixedPositionY >> FP_BITS );

	u8 isOverFloor 	= ( map_getTile( x + 12, y + 32 ) == TILE_FLOOR );
	u8 isOnLadder 	= ( map_getTile( x + 12, y + 31 ) == TILE_LADDER );

	//TODO: delete return ( map_getTile( x + 3, y + 31 ) == TILE_LADDER && map_getTile( x + 12, y + 31 ) == TILE_LADDER && map_getTile( x + 3, y + 32 ) != TILE_FLOOR && map_getTile( x + 12, y + 32 ) != TILE_FLOOR );

	return ( isOnLadder && !isOverFloor);
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doPlayerMovement( u16 state )
{
	if ( IS_TALKING_ON_PHONE ) { delayBeforeHangUp--; }

	u16 prev_player_dir = player_dir;


	if ( isPlayerFalling( ) )
	{
	    if ( playerFallingCounter == 0 )
        {
            playSfx ( SFX_FALL );
        }

		playerFallingCounter++;
		playerFixedPositionY += ( 3 << FP_BITS );

		u8 animationNumber 	= ANIMATION_PLAYER_FALLING;
		u8 frameNumber 		= ( playerFallingCounter & 2 ) 	? FRAME_PLAYER_FALLING_1 : FRAME_PLAYER_FALLING_2;
		u8 needToBeFlipped 	= ( player_dir != DIR_RIGHT ) 	? TRUE : FALSE;

		s16 positionY 	= ( playerFixedPositionY >> FP_BITS ) + playerSprite->frame->h;
		s16 positionX 	= ( playerFixedPositionX >> FP_BITS ) + ( playerSprite->frame->w >> 1 );
		u8 tileType 	= map_getTile( positionX, positionY );

		if( tileType == TILE_FLOOR || tileType == TILE_LADDER )
		{
			standingUpDuration = 5;
			playerFixedPositionY &= ~( 7 << FP_BITS );    //Round to lower multiple of 8
		}

		setSecretaryHFlip( needToBeFlipped );
		SPR_setAnimAndFrame( playerSprite, animationNumber, frameNumber);
	}
	else if ( IS_PLAYER_KNOCKED )
	{
		u8 needToBeFlipped 	= ( player_dir != DIR_RIGHT ) 	? TRUE : FALSE;

		standingUpDuration = 5;
		playerKnockedDuration--;

		if ( player_knocked_ani ) { player_knocked_ani--; }

		if ( player_dir != DIR_RIGHT )  { player_dir = DIR_LEFT; };

		SPR_setAnimAndFrame( playerSprite, 10, ( player_knocked_ani ) ? 0 : ((vtimer & 8) ? 2 : 1) );
		setSecretaryHFlip( needToBeFlipped );

	}

	else if( IS_PLAYER_STANDING_UP )
	{
		standingUpDuration--;
		SPR_setAnimAndFrame( playerSprite, 10, 3 );
	}
	else
	{
        playerFallingCounter = 0;

        s16 x = ( playerFixedPositionX >> FP_BITS );
        s16 y = ( playerFixedPositionY >> FP_BITS );

        //TODO: ARREGLAR FRAME DEBE DE SER DEPENDIENDO DE LA VELOCIDAD --> HECHO

        if ( !player_move_cnt )
        {
            if ( state & BUTTON_UP )
            {
                if ( map_getTile( x + 14, y + 31 ) == TILE_LADDER || map_getTile( x + 9, y + 31 ) == TILE_LADDER )    //ladder doesn't finish
                {
                    //Auto Align to Ladder
                    while( map_getTile( x + 8, y + 31 ) != TILE_LADDER )
                    {
                        x++;
                    }
                    while( map_getTile( x + 16, y + 31 ) != TILE_LADDER )
                    {
                        x--;
                    }

                    playerFixedPositionX = ( x << FP_BITS );
                    player_dir = DIR_UP;
                    player_move_cnt = ( 16 << FP_BITS );
                }
            }

            else if ( state & BUTTON_DOWN )
            {
                if ( map_getTile( x + 14, y + 32 ) == TILE_LADDER || map_getTile( x + 9, y + 32 ) == TILE_LADDER )    //ladder doesn't finish
                {
                    //Auto Align to Ladder
                    while( map_getTile( x + 8, y + 32 ) != TILE_LADDER )
                    {
                        x++;
                    }
                    while( map_getTile( x + 16, y + 32 ) != TILE_LADDER )
                    {
                        x--;
                    }

                    playerFixedPositionX = ( x << FP_BITS );
                    player_dir = DIR_DOWN;
                    player_move_cnt = ( 16 << FP_BITS );
                }
            }
        }

        if ( player_move_cnt )
        {
            if ( player_dir == DIR_UP )
            {
                setSecretaryPriority( FALSE ); // TRUE
                playerFixedPositionY -= playerCurrentSpeed;

                //Check for player wrap
                if ( playerFixedPositionY <= ( 24 ) << FP_BITS )
                {
                    playerFixedPositionY = ( 224 - 8 - 1 ) << FP_BITS;
                }

                //Is player floating?? -OVERFLOW-
                else if ( map_getTile( x + 12, ( playerFixedPositionY >> FP_BITS ) + 32 ) != TILE_LADDER )
                {
                    playerFixedPositionY = ( playerFixedPositionY + ( 7 << FP_BITS ) ) & ~( 7 << FP_BITS );    //Round to next multiple of 8
                    player_dir = DIR_LEFT;
                }
            }
            else if ( player_dir == DIR_DOWN )
            {
                setSecretaryPriority( FALSE ); // TRUE
                playerFixedPositionY += playerCurrentSpeed;

                //Check for player wrap
                if ( playerFixedPositionY > ( 224 - 8 - 1 ) << FP_BITS )
                {
                    playerFixedPositionY = ( 24 + 1 ) << FP_BITS;
                }
                //Is player inside the floor?? -OVERFLOW-
                else if ( map_getTile( x + 12, ( playerFixedPositionY >> FP_BITS ) + 32 ) == TILE_FLOOR )
                {
                    playerFixedPositionY &= ~( 7 << FP_BITS );    //Round to lower multiple of 8
                    player_dir = DIR_RIGHT;
                }
            }

            updateNextFrame();

            player_move_cnt -= playerCurrentSpeed;

            if ( player_move_cnt < 0 )
            {
                player_move_cnt = 0;

                if ( !isPlayerOnLadder() )
                {
                    if ( player_dir == DIR_UP   ) player_dir = DIR_LEFT;
                    if ( player_dir == DIR_DOWN ) player_dir = DIR_RIGHT;
                }
            }
        }

        if ( !isPlayerOnLadder( ) )
        {
            player_dir = DIR_NONE;

            if ( state & BUTTON_RIGHT )
            {
                playerFixedPositionX += playerCurrentSpeed;
                player_dir  = DIR_RIGHT;
                player_flip = FALSE;
            }

            else if ( state & BUTTON_LEFT )
            {
                playerFixedPositionX -= playerCurrentSpeed;
                player_dir  = DIR_LEFT;
                player_flip = TRUE;
            }


            setSecretaryPriority( FALSE );
            checkForPlayerHorizontalWarp( );

            updateNextFrame();
        }

        if ( player_dir == DIR_NONE )
        {
            setSecretaryHFlip( player_flip );
            SPR_setAnim( playerSprite, 8 );
        }
        else
        {
            setSecretaryHFlip( FALSE );
            SPR_setAnim( playerSprite, player_dir + ( IS_TALKING_ON_PHONE ? 4 : 0 ) );
        }

        if ( prev_player_dir != player_dir )
        {
            playerSfxStep( );
        }
    }

    if ( DEV )
    {
        drawUInt ( getFPS(),0,0,2 );
        drawUInt ( player_dir,0,3,2 );
        drawUInt ( playerCurrentSpeed,0,4,3 );
        drawUInt ( playerGetAminationSpeed (),0,5,3 );
        drawInt  ( playerFixedPositionX >> FP_BITS, 0, 6, 3 );
        drawInt  ( playerFixedPositionY >> FP_BITS, 0, 7, 3 );
        drawInt  ( playerFallingCounter, 0, 8, 3 );
    }
}

void playerReset( void )
{
	playerSprite = NULL;
    player_dir = DIR_NONE;
}

void player_init( s16 x, s16 y )
{
	playerSprite = SPRFactory.createSecretary( x, y);
	playerCurrentSpeed = ( 1 << FP_BITS );
	playerSpeedTo = playerCurrentSpeed;
	playerSpeedBonus = 0;
	playerSpeedBonusCounter = 0;
	playerFixedPositionX = x << FP_BITS;
	playerFixedPositionY = y << FP_BITS;
	player_frame_cnt = 0;
	delayBeforeHangUp = 0;
	player_move_cnt = 0;
	player_dir = DIR_NONE;
	player_flip = FALSE;
	playerKnockedDuration = 0;
	playerFallingCounter = 0;
	player_knocked_ani = 0;
	playerWisdomDuration = 0;
	isPlayerCatched = FALSE;
	isPlayerSlowed = FALSE;
    sfxDirHorizontal = 0;
    sfxDirVertical   = 0;

	preparePal(PAL2, secretarySprDef.palette->data);
}

void player_step( u16 state )
{

	if( !IS_PLAYER_INITIALIZED ) { return; }


	updatePlayerSpeed( );
	updatePlayerSpeedBonus( );
	doPlayerMovement( state );

	SPR_setPosition( playerSprite, playerFixedPositionX >> FP_BITS, playerFixedPositionY >> FP_BITS ); //TODO: CREO QUE PUEDE SER ELIMINADO MRIAR SI SE USA EL PLAYER->X despues se hace lo mismo porque se mueve...(NPC)




	if ( IS_PLAYER_WISDOM )
	{
		playerWisdomDuration--;

		if ( !( playerWisdomDuration & 15 ) )
		{
			s16 positionX = ( playerFixedPositionX >> FP_BITS ) + 8;
			s16 positionY = ( playerFixedPositionY >> FP_BITS );

			heartAdd( positionX, positionY );
		}
	}


	//PROCESS TOPICS COLLISION
	topic_checkCollision( playerSprite->x + 5, playerSprite->x + 10, playerSprite->y - ( 2 << TILE_BITS ), playerSprite->y + 23 - ( 2 << TILE_BITS ) );

	//PROCESS PHONES COLLISION
	u8 collision = phoneCheckCollision( playerSprite->x + 5, playerSprite->x + 10, playerSprite->y, playerSprite->y + 23 );

	if ( collision )
	{
		delayBeforeHangUp = 25;
	}
}

