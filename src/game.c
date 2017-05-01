#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/common.h"
#include "../inc/music.h"
#include "../inc/spritedispatcher.h"
#include "../inc/joyreader.h"
#include "../inc/helpers.h"

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
#include "../inc/tempo.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_RESTART  ( ( PAD_1_ACTIVE & BUTTON_A ) && ( PAD_1_ACTIVE & BUTTON_B ) && ( PAD_1_ACTIVE & BUTTON_C ) )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


	static u8 isGamePaused;
	static u8 delayForPausedOrResumeAgain;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void checkForGamePauseOrResume( )
{
    if( delayForPausedOrResumeAgain )
    {
        delayForPausedOrResumeAgain--;

        if ( !delayForPausedOrResumeAgain )
        {
            sfxMute();
        }
    }
    else if ( PAD_1_PRESSED_START )
    {
        delayForPausedOrResumeAgain = getHz() / 2;
        isGamePaused = !isGamePaused;

        u16  xPos       = 0;
        void (*funct)() = musicResume;
        u8   tempo      = getDefaultMusicTempo();

        if ( isGamePaused )
        {
            xPos  = VDP_getScreenHeight();
            funct = musicPause;
            tempo /= 3;
        }

        playSfx( SFX_PAUSE );
        VDP_setVerticalScroll( PLAN_A, xPos );
        setMusicTempo ( tempo );
        //funct();
    }

    if ( isGamePaused )
    {
        u8 musicTempo = getMusicTempo();

        if ( ( PAD_1_ACTIVE_UP   | PAD_1_ACTIVE_LEFT  | ( PAD_1_ACTIVE & BUTTON_A ) ) && ( musicTempo > 1                      ) ) --musicTempo;
        if ( ( PAD_1_ACTIVE_DOWN | PAD_1_ACTIVE_RIGHT | ( PAD_1_ACTIVE & BUTTON_B ) ) && ( musicTempo < getDefaultMusicTempo() ) ) ++musicTempo;

        setMusicTempo ( musicTempo );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void leaveProcess()
{
    VDP_fadeOutAll( 30, 0 );

    SPR_reset( );
    SPR_update();
    SPRD_reset( );

    musicStop();
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
        playMusic(MUSIC_GAME);
        setRandomSeed(vtimer);

	VDP_setEnable( TRUE );


	isGamePaused = FALSE;
	delayForPausedOrResumeAgain = ( IS_PALSYSTEM ) ? 25 : 30;


	JoyReaderReset( );

	while( totalOfAnsweredCalls < call_max && totalOfMissedCalls < miss_max && !isPlayerCatched )
	{
		VDP_waitVSync( );

		JoyReaderUpdate( );

        // force restart
        if ( IS_RESTART )
        {
            leaveProcess();

            return 255;
        }


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

            if ( EXIT_MODE_FLAG && PAD_1_PRESSED_ABC )
            {
                totalOfAnsweredCalls = call_max;
            }

			//Player position should be computed at the end, because NPCs could move it...
			SPR_setPosition( playerSprite, playerFixedPositionX >> FP_BITS, playerFixedPositionY >> FP_BITS );
		}

		SPR_update( );
	}

    musicStop();


	u8 isLevelCompleted = ( totalOfAnsweredCalls == call_max );

	if ( IS_LEVEL_BONUS )
	{
		BONUS_FLAG = isLevelCompleted;
		displayMessageBonus( isLevelCompleted );
	}

	if ( isLevelCompleted )
    {
        playMusic ( MUSIC_CLEAR );
    }
    else
    {
        playSfx ( SFX_LOSE );
    }

	//TODO: 3*50 DEPENDE DEL BONUS
	u8 i;
	for ( i = 0; i < 200; ++i )
	{
		VDP_waitVSync( );
		messageUpdate( );
		hudUpdate( );
	}

    leaveProcess();

	return isLevelCompleted;
}


void game_done( void )
{
	//Sprite Init...
	Sprite *spr = SPRD_getFirst( );
	spr = SPR_addSprite ( (SpriteDefinition*) &secretaryRestSprDef, 160, 80, TILE_ATTR( PAL0, FALSE, FALSE, FALSE ) );

	VDP_waitVSync( );

	VDP_setEnable( FALSE );
		VDP_drawImageEx( PLAN_A, &officeWeekend, TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ), 0, 0, TRUE, FALSE );
	VDP_setEnable( TRUE );

	playMusic(MUSIC_WELLDONE);

	u8 wait = 0;
	u8 spr_frame = 0;
	u8 frame_cnt = 0;
	u8 i = 255;
	u16 j = 0;

	JoyReaderReset();

	while( !( PAD_1_PRESSED_ABC | PAD_1_PRESSED_START ) || wait < 50 )
	{
		VDP_waitVSync( );
		JoyReaderUpdate();

		SPR_setFrame( spr, spr_frame );
		SPR_update( );

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
			musicResume();
		}

		if ( j == 1240 )
		{
		    playSfx(SFX_RING);
		    musicPause();
		}
		if ( j == 1270 )
		{
			i = 0;
		}
	}

	VDP_fadeOutAll(60,0);

	for ( i = 0; i < 100; ++i )
	{
		VDP_waitVSync( );
	}

	musicStop();


	SPR_reset( );
	SPR_update();
	SPRD_reset( );

//    VDP_setEnable ( FALSE );
    SYS_disableInts();
    VDP_clearPlan ( PLAN_A, 1 );
    VDP_clearPlan ( PLAN_B, 1 );
    SYS_enableInts();

//	VDP_setEnable ( TRUE );
}
