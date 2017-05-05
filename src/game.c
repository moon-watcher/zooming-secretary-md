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
#include "../inc/display.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_RESTART  ( ( PAD_1_ACTIVE & BUTTON_A ) && ( PAD_1_ACTIVE & BUTTON_B ) && ( PAD_1_ACTIVE & BUTTON_C ) )


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static s8 tempoTimeout;
static u8 isGamePaused;
static u8 delayForPausedOrResumeAgain;
static u8 firstSpriteLink;

static const struct
{
    u8 time;
    u8 frame;
}
levelCompletedFrames [ 5 ] =
{
    {   0, 0 },
    {  60, 1 },
    {  80, 2 },
    { 110, 1 },
    { 125, 0 }
};


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void playerLevelCompletedAnimation ( u8 time, u8 isLevelCompleted )
{
    if ( !isLevelCompleted )
    {
        return;
    }

    u8 j;

    for ( j=0; j<5; j++ )
    {
        if ( levelCompletedFrames[j].time == time )
        {
            SPR_setAnimAndFrame ( playerSprite, 9, levelCompletedFrames[j].frame );
            SPR_update( );
            break;
        }
    }
}


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

        u16  yPos       = 0;
        void (*funct)() = musicResume;
        u8   tempo      = getDefaultMusicTempo();

        if ( isGamePaused )
        {
            firstSpriteLink = vdpSpriteCache[0].link;
            vdpSpriteCache[0].link = 0;

            yPos  = VDP_getScreenHeight();
            funct = musicPause;
            tempo /= 3;
        }
        else
        {
            vdpSpriteCache[0].link = firstSpriteLink;
            VDP_updateSprites(80,0);
            VDP_waitVSync();
        }

        playSfx( SFX_PAUSE );
        VDP_setVerticalScroll( PLAN_A, yPos );
        VDP_setVerticalScroll( PLAN_B, yPos );
        setMusicTempo ( tempo );
        //funct();
    }

    if ( isGamePaused && MUSIC_MODE_FLAG )
    {
        u8 inc        = 0;
        s8 musicTempo = getMusicTempo();
        u8 tempo      = getDefaultMusicTempo();

        if ( PAD_1_ACTIVE_UP   ) inc = -1;
        if ( PAD_1_ACTIVE_DOWN ) inc = +1;

        musicTempo += inc;

        if ( musicTempo <     1 ) musicTempo = 1;
        if ( musicTempo > tempo ) musicTempo = tempo;

        if ( inc )
        {
            VDP_drawTextBG ( PLAN_A, "TEMPO   ", 16, 43 );
            drawUInt ( musicTempo, 22, 43, 2 );
            tempoTimeout = 20; // hz
            waitHz(3);
        }

        if ( tempoTimeout )
        {
            --tempoTimeout;

            if ( !tempoTimeout )
            {
                VDP_drawTextBG ( PLAN_A, "        ", 16, 43 );
            }
        }

        setMusicTempo ( musicTempo );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void leaveProcess()
{
    displayOff(10);

    SPR_reset( );
    SPR_update();
    SPRD_reset( );

    VDP_waitVSync();

    musicStop();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


u8 game_play( void )
{
	displayOff(0);

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
    setRandomSeed(vtimer);

    displayOn(10);

    playMusic(MUSIC_GAME);

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
        playerLevelCompletedAnimation( i, isLevelCompleted );
	}

    leaveProcess();

	return isLevelCompleted;
}


void game_done( void )
{
    displayOff(0);
	//Sprite Init...
	Sprite *spr = SPRD_getFirst( );
	spr = SPR_addSprite ( (SpriteDefinition*) &secretaryRestSprDef, 160, 80, TILE_ATTR( PAL0, FALSE, FALSE, FALSE ) );

    VDP_drawImageEx( PLAN_A, &officeWeekend, TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ), 0, 0, 0, FALSE );
    preparePal( PAL0, officeWeekend.palette->data );

    SPR_update( );

	displayOn(60);

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

	displayOff(60);

    waitHz(100);
	musicStop();

	SPR_reset( );
	SPR_update();
	SPRD_reset( );

	VDP_waitVSync();

    VDP_clearPlan ( PLAN_A, 1 );
    VDP_clearPlan ( PLAN_B, 1 );
}
