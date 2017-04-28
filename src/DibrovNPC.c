#include <genesis.h>
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/player.h"
#include "../inc/map.h"
#include "../inc/helpers.h"

#include <Kdebug.h>


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_NPC_WAITING (waitUntilAppearsAgain != 0)
#define ANIMATION_WALK  0
#define ANIMATION_RUN   1


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static Sprite* npcDibrovSprite;
static  s8 npcDibrovSpeed;
static s16 npcDibrovPositionX;
static s16 npcDibrovPositionY;
static  s8 npcDibrovWidth;
static  s8 npcDibrovHeight;
static  u8 npcDibrovDir;

static u16 waitUntilAppearsAgain;
static u16 frameCounter;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void warpDibrovNPC( u8 warpDirection )
{
    if ( warpDirection ==  DIR_RIGHT )
    {
        npcDibrovPositionY  = getRandomRightFloorPositionY( ) - npcDibrovHeight;
        npcDibrovDir        = DIR_LEFT;
        npcDibrovPositionX  = VDP_getScreenWidth();
    }
    else
    {
        npcDibrovPositionY  = getRandomLeftFloorPositionY( )  - npcDibrovHeight;
        npcDibrovDir        = DIR_RIGHT;
        npcDibrovPositionX  = -npcDibrovWidth;
    }

    waitUntilAppearsAgain = playerWisdomDuration + 25;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcDibrovInitialize( s16 positionX, s16 positionY )
{
    npcDibrovSprite      =  SPRFactory.createDibrovNPC( positionX, positionY );
    npcDibrovPositionX   =  positionX;
    npcDibrovPositionY   =  positionY;
    npcDibrovDir         =  DIR_LEFT;
    npcDibrovWidth       =  ( npcDibrovSprite->frame->w );
    npcDibrovHeight      =  ( npcDibrovSprite->frame->h );

    waitUntilAppearsAgain = 0;
    frameCounter = 0;

    const u16 *npcPalette = npcDibrovSprite->definition->palette->data;
    VDP_setPalette( PAL3, npcPalette );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcDibrovUpdate( void )
{
    if ( IS_NPC_WAITING )
    {
        waitUntilAppearsAgain--;
        KDebug_Alert("1.1- NPC is waiting");
    }
    else
    {
        KDebug_Alert("1.2- NPC NOT waiting");
        const s16 playerPositionY   = ( playerFixedPositionY >> FP_BITS );
        const s16 playerPositionX   = ( playerFixedPositionX >> FP_BITS );
        const s16 playerWidth       = ( playerSprite->frame->w );

        if( playerPositionY == npcDibrovPositionY )
        {
            KDebug_Alert("1.2.1- NPC y == PLayer Y");
            if ( !( ( playerPositionX + playerWidth - 4 ) < npcDibrovPositionX || playerPositionX >= ( npcDibrovPositionX + npcDibrovWidth - 4 ) ) )
            {
                KDebug_Alert("1.2.1.1- COLLISION");
                if ( !IS_PLAYER_WISDOM )
                {
                    playerWisdomDuration = ( IS_PALSYSTEM ) ? 500 : 600;
                    playSfx(SFX_WISDOM);
                }

            }

            npcDibrovSpeed = 2;
            npcDibrovDir = ( playerPositionX < npcDibrovPositionX ) ? DIR_RIGHT : DIR_LEFT;
        }
        else
        {
            KDebug_Alert("1.2.2- NPC y != PLayer Y");
            npcDibrovSpeed = 1;

            if ( ABS( playerPositionX - npcDibrovPositionX ) < 64 )
            {
                KDebug_Alert("1.2.2.1- NPC y Player muy cerca");
                npcDibrovDir = ( playerPositionX < npcDibrovPositionX ) ? DIR_RIGHT : DIR_LEFT;
            }

        }


        if ( !( ( npcDibrovSpeed < 2 ) && ( frameCounter & 1 ) ) )
        {
            switch ( npcDibrovDir )
            {
                case DIR_LEFT:
                    npcDibrovPositionX -= ( ( npcDibrovSpeed == 2 ) && ( frameCounter & 1 ) ) ? 1 : npcDibrovSpeed;
                    break;

                case DIR_RIGHT:
                    npcDibrovPositionX += ( ( npcDibrovSpeed == 2 ) && ( frameCounter & 1 ) ) ? 1 : npcDibrovSpeed;
                    break;
            }

            if ( npcDibrovPositionX > VDP_getScreenWidth() || npcDibrovPositionX < -npcDibrovWidth )
            {
                u8 warpDir = ( playerPositionX < ( VDP_getScreenWidth() >> 1 ) ) ? DIR_RIGHT : DIR_LEFT;

                warpDibrovNPC( warpDir );
            }


            const u8 shouldBeFlipped = ( npcDibrovDir == DIR_LEFT ) ? FALSE : TRUE;
            const u8 animationNumber = ( npcDibrovSpeed == 2 ) ? ANIMATION_RUN : ANIMATION_WALK;

            u8 frameNumber;

            if( animationNumber == ANIMATION_WALK )
            {
                if( shouldBeFlipped )
                {
                    frameNumber = ( npcDibrovPositionX >> 2 ) & 7;
                }
                else
                {
                    frameNumber = 7 - ( ( npcDibrovPositionX >> 2 ) & 7 );
                }
            }
            else
            {
                if( shouldBeFlipped )
                {
                    frameNumber = ( npcDibrovPositionX >> 3 ) & 3;
                }
                else
                {
                    frameNumber = 3 - ( ( npcDibrovPositionX >> 3 ) & 3 );
                }
            }




            SPR_setPosition( npcDibrovSprite, npcDibrovPositionX, npcDibrovPositionY );
            SPR_setAnimAndFrame( npcDibrovSprite, animationNumber,  frameNumber );

            SPRFactory.setHFlip( npcDibrovSprite, shouldBeFlipped );
        }
    }

    frameCounter++;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
