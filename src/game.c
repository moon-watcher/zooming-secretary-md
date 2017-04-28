#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/common.h"
#include "../inc/spritedispatcher.h"
#include "../inc/joyreader.h"

#include "../inc/game.h"

#include "../inc/Coffee.h"  //TODO: usuado por coffe
#include "../inc/hud.h"     //TODO: usuado por level
#include "../inc/npc.h"     //TODO: usuado por npc
#include "../inc/message.h" //TODO: usado por msgreset
#include "../inc/heart.h" //TODO: USADO POR HEART RESET
#include "../inc/player.h" //TODO: usado por reset
#include "../inc/topic.h" //TODO: usado por reset
#include "../inc/map.h" //TODO: usado por reset
#include "../inc/phone.h" //TODO: usado por reset
#include "../inc/CeilingFan.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


	static u8 isGamePaused;
	static u8 delayForPausedOrResumeAgain;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


	static void checkForGamePauseOrResume( )
	{
		if( delayForPausedOrResumeAgain )
		{
			delayForPausedOrResumeAgain--;
		}
		else if ( PAD_1_PRESSED_START )
		{
			delayForPausedOrResumeAgain = ( IS_PALSYSTEM ) ? 25 : 30;
			isGamePaused = !isGamePaused;
			//TODO: PAUSE SFX

			VDP_setVerticalScroll( PLAN_A, isGamePaused ? VDP_getScreenHeight() : 0 );
		}
	}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


u8 game_play( void )
{
	VDP_setEnable( FALSE );

		coffeeDestroy( );
		messageReset( );
		heartsReset( );
		npcReset( );
		playerReset ( );
		topic_reset( );
		phoneReset( );
		ceilingFansReset( );
		map_run( );
		hudInitialize( );

	VDP_setEnable( TRUE );


	isGamePaused = FALSE;
	delayForPausedOrResumeAgain = ( IS_PALSYSTEM ) ? 25 : 30;


	JoyReaderReset( );

	while( totalOfAnsweredCalls < call_max && totalOfMissedCalls < miss_max && !isPlayerCatched )
	{
		VDP_waitVSync( );

		JoyReaderUpdate( );
		checkForGamePauseOrResume( );

		if ( !isGamePaused )
		{
			coffeeUpdate( );
			topic_step( );
			phoneUpdate( );
			messageUpdate( );
			player_step( PAD_1_ACTIVE );
			hudUpdate( );
			npcUpdate( );
			heartsUpdate( );
			ceilingFanUpdate( );

			//Player position should be computed at the end, because NPCs could move it...
			SPR_setPosition( playerSprite, playerFixedPositionX >> FP_BITS, playerFixedPositionY >> FP_BITS );
		}

		SPR_update( sprlist, isGamePaused ? 0 : SPRD_getLength( ) );
	}

	u8 isLevelCompleted = ( totalOfAnsweredCalls == call_max );

	if ( IS_LEVEL_BONUS )
	{
		BONUS_FLAG = isLevelCompleted;
		displayMessageBonus( isLevelCompleted );
	}

	//TODO: 3*50 DEPENDE DEL BONUS
	u8 i;
	for ( i = 0; i < 200; ++i )
	{
		VDP_waitVSync( );
		messageUpdate( );
		hudUpdate( );
	}

	SPR_clear( );
	SPRD_reset( );

	return isLevelCompleted;
}

void game_done( void )
{
	//Sprite Init...
	Sprite *spr = SPRD_getFirst( );
	SPR_initSprite( spr, (SpriteDefinition*) &secretaryRestSprDef, 160, 80, TILE_ATTR( PAL0, FALSE, FALSE, FALSE ) );

	VDP_waitVSync( );

	VDP_setEnable( FALSE );
		VDP_drawImageEx( APLAN, &officeWeekend, TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ), 0, 0, TRUE, FALSE );
	VDP_setEnable( TRUE );

	//TODO: music welldone

	u8 wait = 0;
	u8 spr_frame = 0;
	u8 frame_cnt = 0;
	u8 i = 255;
	u16 j = 0;

	while( !( JOY_readJoypad( JOY_1 ) & BUTTON_START ) || wait < 50 )
	{
		VDP_waitVSync( );
		SPR_setFrame( spr, spr_frame );
		SPR_update( sprlist, SPRD_getLength( ) );

		if ( wait < 50 )
		{
			wait++;
		}

		if ( i >= 180 )
		{
			spr_frame = ( frame_cnt++ & 64 ) ? 1 : 0;
		}
		else
		{
			if ( i < 80 || i >= 120 )
			{
				spr_frame = 2;
			}
			else
			{
				spr_frame = 3;
			}
			i++;
		}

		if ( ++j >= 1280 )
		{
			j = 0;
		}

		if ( j == 1240 )
		{
		}    //TODO: sfx_play(SFX_RINGTONE,0); }
		if ( j == 1270 )
		{
			i = 0;
		}
	}

	//TODO: music top
	for ( i = 0; i < 25; ++i )
	{
		VDP_waitVSync( );
	}

	SPR_clear( );
	SPRD_reset( );
}
