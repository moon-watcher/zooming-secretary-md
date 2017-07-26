#include <genesis.h>

#include "../inc/dev.h"
#include "../inc/game.h"
#include "../inc/display.h"
#include "../inc/helpers.h"
#include "../inc/common.h"
#include "../inc/joyreader.h"
#include "../inc/weekendScreen.h"
#include "../res/rescomp.h"
#include "../inc/spritedispatcher.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define TIME1 getHz() * 21 // 21 secs
#define TIME2 TIME1 + 10
#define TIME3 TIME2 + getHz()
#define TIME4 TIME3 + getHz()


static u8 waveCounter;
static u8 waveCurrentFrame;

static u8  wait;
static u8  spr_frame;
static u8  frame_cnt;
static u8  i;
static u16 j;

static Sprite *spr;

static const struct _wave
{
    u8 delay;
    u8 colors[5];
}
waveFrames[10] =
{
    { 50, { 10,  3, 10,  3, 10 } }, // 0

    { 30, {  8, 10,  3, 10,  2 } }, // 1
    { 16, {  8,  9, 10,  3, 10 } }, // 2
    {  9, {  8,  9,  8, 10,  2 } }, // 3
    {  7, {  8,  9,  8,  9, 10 } }, // 4

    { 15, {  8,  9,  8,  9,  8 } }, // 5

    {  7, {  8,  9,  8,  9, 10 } }, // 6
    {  9, {  8,  9,  8, 10,  2 } }, // 7
    { 13, {  8,  9, 10,  3, 10 } }, // 8
    { 20, {  8, 10,  3, 10,  2 } }, // 9
};


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doInitWaves()
{
    waveCounter = 0;
    waveCurrentFrame = 0;

    struct _wave *wave = (struct _wave *) &waveFrames [ 0 ];

    prepareColor ( 11, officeWeekend.palette->data [ wave->colors[0] ] );
    prepareColor ( 12, officeWeekend.palette->data [ wave->colors[1] ] );
    prepareColor ( 13, officeWeekend.palette->data [ wave->colors[2] ] );
    prepareColor ( 14, officeWeekend.palette->data [ wave->colors[3] ] );
    prepareColor ( 15, officeWeekend.palette->data [ wave->colors[4] ] );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doUpdateWaves()
{
    struct _wave *wave;

    wave = (struct _wave *) &waveFrames [ waveCurrentFrame ];


    if ( DEV )
    {
        drawUInt(waveCurrentFrame, 0,0,3);
        drawUInt(waveCounter, 0,1,3);
        drawUInt(wave->delay, 0,2,3);
    }


    if ( wave->delay == waveCounter )
    {
        ++waveCurrentFrame;
        waveCurrentFrame %= 10;
        waveCounter = 0;
    }

    if ( waveCounter == 0 )
    {
        wave = (struct _wave *) &waveFrames [ waveCurrentFrame ];

        VDP_setPaletteColor ( 11, VDP_getPaletteColor ( wave->colors[0]) );
        VDP_setPaletteColor ( 12, VDP_getPaletteColor ( wave->colors[1]) );
        VDP_setPaletteColor ( 13, VDP_getPaletteColor ( wave->colors[2]) );
        VDP_setPaletteColor ( 14, VDP_getPaletteColor ( wave->colors[3]) );
        VDP_setPaletteColor ( 15, VDP_getPaletteColor ( wave->colors[4]) );
    }


    ++waveCounter;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doUpdateNormalMode ( )
{
    if ( CLASSIC_MODE_FLAG )
    {
        return;
    }

    if ( wait < 50 )
    {
        wait++;
    }

    if ( j == TIME1 )
    {
        SPR_setVisibility(spr, VISIBLE);
        SPR_setAnim ( spr, 0 );
        SPR_update( );

        musicPause();
        waitHz(1);
        sfxUseChannel ( 1 );
        playSfx(SFX_RING);
    }

    if ( j == TIME2 )
    {
        SPR_setAnim ( spr, 1 );
        SPR_update( );
        waitHz(1);
    }

    if ( j == TIME3 )
    {
        musicResume();
    }

    if ( j == TIME4 )
    {
        SPR_setVisibility(spr, HIDDEN);
        SPR_update( );
        waitHz(1);

        j = 0;
    }

    ++j;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void doUpdateClassicMode ( )
{
    if ( !CLASSIC_MODE_FLAG )
    {
        return;
    }

    doUpdateWaves();

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
        musicPause();
        waitHz(1);
        sfxUseChannel ( 1 );
        playSfx(SFX_RING);
    }
    if ( j == 1270 )
    {
        i = 0;
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawScreen()
{
    VDP_drawImageEx( PLAN_A, &officeWeekend, TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ), 0, 0, 0, FALSE );
    preparePal( PAL0, officeWeekend.palette->data );

    if ( !CLASSIC_MODE_FLAG)
    {
        VDP_drawImageEx( PLAN_B, &weekendBgImg, TILE_ATTR_FULL( PAL2, 1, FALSE, FALSE, 100 ),  7, 2, 0, FALSE );
        VDP_drawImageEx( PLAN_A, &weekendFgImg, TILE_ATTR_FULL( PAL3, 1, FALSE, FALSE, 500 ), 19, 3, 0, FALSE );
        preparePal( PAL2, weekendBgImg.palette->data );
        preparePal( PAL3, weekendFgImg.palette->data );
    }

    VDP_waitVSync();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void prepareSprite()
{
	//Sprite Init...
	spr = SPRD_new ( 0, 0 );

	s16 x = 8*21;
	s16 y = 8*4;
	SpriteDefinition *sd = (SpriteDefinition*) &sprsheetRest1Def;
	SpriteVisibility visible = HIDDEN;

	if ( CLASSIC_MODE_FLAG )
    {
        x = 160;
        y = 80;
        sd = (SpriteDefinition *) &secretaryRestSprDef;
        visible = VISIBLE;
    }

	spr = SPR_addSprite ( (SpriteDefinition*) sd, x, y, TILE_ATTR( PAL1, 1, FALSE, FALSE ) );
    SPR_setVisibility ( spr, visible );
	preparePal( PAL1, sd->palette->data );

    SPR_update( );
    VDP_waitVSync();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void showWeekend ()
{
    displayOff(0);

    prepareSprite();
    drawScreen();
    doInitWaves();
	displayOn(60);

	playMusic(MUSIC_WELLDONE);

	wait = 0;
	spr_frame = 0;
	frame_cnt = 0;
	i = 255;
	j = 0;

	JoyReaderReset();

	while( !( PAD_1_PRESSED_ABCS ) || wait < 50 )
	{
		VDP_waitVSync( );

        JoyReaderUpdate();

        doUpdateClassicMode ( );
        doUpdateNormalMode ( );
	}

	sfxMute();
	waitHz(2);
	musicStop();
	waitHz(2);

	displayOff(10);

	resetScreen();
	SPR_reset( );
	SPR_update();
	SPRD_reset( );

    waitSc(1);
}
