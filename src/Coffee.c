#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/SPRFactory.h"
#include "../inc/common.h"
#include "../inc/helpers.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


#define IS_COFFEE_INITIALIZED ( this.coffeeSprite != NULL )
#define IS_COFFEE_READY ( this.delayBeforeShowCoffee == 0 )

#define DELAY_BETWEEN_COFFEES 1000

#define setCoffeeFrame( numFrame ) ( SPR_setFrame( this.coffeeSprite, numFrame ) )
#define FRAME_COFFEE_INVISIBLE 0
#define FRAME_COFFEE_VISIBLE 1
#define FRAME_COFFEE_READY_1 2
#define FRAME_COFFEE_READY_2 3

#define loadCofeeMakerTiles( ) ( VDP_loadTileSet( coffeMakerImage.tileset, COFFEEMAKER_TILE_IDX, FALSE ) )
#define COFFEEMAKER_PALETTE PAL1
#define COFFEEMAKER_PLAN PLAN_A
#define COFFEEMAKER_TILE_IDX TILE_USERINDEX
#define COFFEEMAKER_ATTRIBUTES TILE_ATTR_FULL( COFFEEMAKER_PALETTE, FALSE, FALSE, FALSE, COFFEEMAKER_TILE_IDX )

#define FRAME_COFFEEMAKER_WITH		6
#define FRAME_COFFEEMAKER_HEIGHT	6


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


typedef struct
{
	Sprite *coffeeSprite;
	u16 delayBeforeShowCoffee;
	u16 timeSinceCoffeeReady;

} coffeeStruct;

static coffeeStruct this;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void drawCoffeeMakerFrame( u8 frameNumber )
{
    // These positions are measured in tiles
    s16 screenPositionX = ( this.coffeeSprite->x - 128 - 8 ) >> 3;
    s16 screenPositionY = ( this.coffeeSprite->y - 128 - 24) >> 3;

    s16 mapPositionX = FRAME_COFFEEMAKER_WITH * frameNumber;
    s16 mapPositionY = 0;

    SYS_disableInts();
    VDP_setMapEx( COFFEEMAKER_PLAN, coffeMakerImage.map, COFFEEMAKER_ATTRIBUTES, screenPositionX,  screenPositionY, mapPositionX, mapPositionY, FRAME_COFFEEMAKER_WITH, FRAME_COFFEEMAKER_HEIGHT);
	SYS_enableInts();
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static void setDelayBeforeShowCoffee( u16 delay )
{
	this.delayBeforeShowCoffee 	= delay;
	this.timeSinceCoffeeReady 	= 0;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void coffeeDestroy( void )
{
	this.coffeeSprite = NULL;
	setDelayBeforeShowCoffee( 0 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void coffeeInitialize( s16 positionX, s16 positionY )
{
	this.coffeeSprite = SPRFactory.createCoffee( positionX, positionY );
	setDelayBeforeShowCoffee( 150 );

	loadCofeeMakerTiles( ); //TODO: remove coffe makers from images =)
	drawCoffeeMakerFrame( 0 );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void coffeeUpdate( void )
{
	if ( IS_COFFEE_INITIALIZED )
	{
		if( IS_COFFEE_READY )
		{
			this.timeSinceCoffeeReady++;

			u8 frameNumber = ( this.timeSinceCoffeeReady & 16 ) ? FRAME_COFFEE_READY_1 : FRAME_COFFEE_READY_2;
			setCoffeeFrame( frameNumber );
		}
		else
		{
			this.delayBeforeShowCoffee--;

			if ( this.delayBeforeShowCoffee == 34 )
			{
				playSfx( SFX_COFFEE_READY );
			}
			else if ( this.delayBeforeShowCoffee < 34 )
			{
			    u8 frameNumber = ( this.delayBeforeShowCoffee >> 2 );
			    drawCoffeeMakerFrame( frameNumber );
			}
		}
	}
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


u8 coffeeCheckCollision( s16 obj_x, s16 obj_xx, s16 obj_y, s16 obj_yy )
{
	/*
	 * Note: MegaDrive sprite plane starts at position(x=128, y=128) instead of (x=0, y=0)
	 *
	 * (x,y)
	 * 		o------------
	 * 		|           |
	 * 		|           |
	 *		|           |
	 *		|           |
	 *		------------o
	 *					(xx,yy)
	 */

	u8 isCollided = FALSE;

	if ( IS_COFFEE_INITIALIZED && IS_COFFEE_READY )
	{
		s16 coffe_x  = this.coffeeSprite->x - 128; //TODO: QUITAR 128 DEL OFFSET
		s16 coffe_xx = coffe_x + this.coffeeSprite->frame->w;
		s16 coffe_y  = this.coffeeSprite->y - 128;
		s16 coffe_yy = coffe_y + this.coffeeSprite->frame->h;

		isCollided = ( !( coffe_x > obj_xx || coffe_xx < obj_x || coffe_y > obj_yy || coffe_yy < obj_y ) );

		if ( isCollided )
		{
			setDelayBeforeShowCoffee( DELAY_BETWEEN_COFFEES );
			setCoffeeFrame( FRAME_COFFEE_INVISIBLE );

			playSfx( SFX_COFFEE );
		}
	}

	return isCollided;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */

