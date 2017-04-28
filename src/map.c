#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/common.h"
#include "../inc/spritedispatcher.h"
#include "../inc/map.h"

#include "../inc/Coffee.h"     //TODO: PARA init
#include "../inc/tilemap.h"
#include "../inc/priomap.h"
#include "../inc/hud.h"     //TODO: PARA PINTAR EL LEVEL CORRESPONDIENTE
#include "../inc/npc.h"     //TODO: PARA tile_npc
#include "../inc/topic.h"     //TODO: PARA init
#include "../inc/player.h"     //TODO: PARA init
#include "../inc/phone.h"     //TODO: PARA init
#include  "../inc/CeilingFan.h"




static const Image *officesPlanA[ ] =
{
    &OfficeDay01_APLAN,
    &OfficeDay02_APLAN,
    &OfficeDay03_APLAN,
    &OfficeDay04_APLAN,
    &OfficeDream_APLAN,
    &OfficeDay05_APLAN,
    &OfficeDay06_APLAN,
    &OfficeDay07_APLAN
};

static const Image *officesPlanB[ ] =
{
    &OfficeDay01_BPLAN,
    &OfficeDay02_BPLAN,
    &OfficeDay03_BPLAN,
    &OfficeDay04_BPLAN,
    &OfficeDream_BPLAN,
    &OfficeDay05_BPLAN,
    &OfficeDay06_BPLAN,
    &OfficeDay07_BPLAN
};


void map_run( void )
{
	VDP_setEnable( FALSE );

		VDP_setPalette( PAL0, officesPlanB[ lvl ]->palette->data );
		VDP_setPalette( PAL1, officesPlanA[ lvl ]->palette->data );

		u16 tileindexA = 118; /* TILE_INDEX + COFFE_MACHINE */
		VDP_loadTileSet( officesPlanA[ lvl ]->tileset, tileindexA, FALSE );

		u16 tileindexB = tileindexA + officesPlanA[ lvl ]->tileset->numTile;
		VDP_loadTileSet( officesPlanB[ lvl ]->tileset, tileindexB, FALSE );

		u16 tileindexC = tileindexB + officesPlanB[ lvl ]->tileset->numTile;
	    VDP_drawImageEx( PLAN_A, &GamePause_APLAN, TILE_ATTR_FULL( PAL1, TRUE, FALSE, FALSE, tileindexC ), 0, VDP_getScreenHeight()>>TILE_BITS, FALSE, FALSE );

		floor_left_cnt = 0;
		floor_right_cnt = 0;


		u8 x, y, type;

		//
		// Ñapa define las prioridades de los sprites :(
		//
		#define aaa \
            for ( y = 0; y < 28; y++ ) \
                for ( x = 0; x < 40; x++ ) \
                    switch ( type = tilemaps[ lvl ][ x + ( y * 40 ) ] ) {

		aaa case NPC_GHOST:   npcInitialize ( 16, VDP_getScreenHeight(),      type ); break; } // higher sprite priority
        aaa case TILE_PLAYER: player_init   ( x << TILE_BITS, y << TILE_BITS       ); break; } // .
        aaa case NPC_CHIEF:   npcInitialize ( x << TILE_BITS, y << TILE_BITS, type ); break;   // .
            case NPC_BOUNCER: npcInitialize ( x << TILE_BITS, y << TILE_BITS, type ); break;   // .
            case NPC_CHATTER: npcInitialize ( x << TILE_BITS, y << TILE_BITS, type ); break;
            case NPC_MANBOX:  npcInitialize ( x << TILE_BITS, y << TILE_BITS, type ); break;
            case NPC_GEEK:    npcInitialize ( x << TILE_BITS, y << TILE_BITS, type ); break;
            case NPC_DIBROV:  npcInitialize ( x << TILE_BITS, y << TILE_BITS, type ); break; } // lower priority
        //
        //


		for ( y = 0; y < 28; y++ )
		{
			for ( x = 0; x < 40; x++ )
			{
				const u16 idx = x + ( y * 40 );

				const u8 prioB	= FALSE;
				const u8 prioA 	= priorityMaps[ lvl ][ idx ];

				const u16 posA 	= officesPlanA[ lvl ]->map->tilemap[ idx ];
				const u16 posB 	= officesPlanB[ lvl ]->map->tilemap[ idx ];

				const u8 type	= tilemaps[ lvl ][ idx ];

				VDP_setTileMapXY( PLAN_A, TILE_ATTR_FULL( PAL1, prioA, FALSE, FALSE, tileindexA + posA ), x, y );
				VDP_setTileMapXY( PLAN_B, TILE_ATTR_FULL( PAL0, prioB, FALSE, FALSE, tileindexB + posB ), x, y );

				switch ( type )
				{
                    case TILE_COFFEE:      coffeeInitialize ( (x << TILE_BITS) -1, (y << TILE_BITS) -1 ); break;
                    case TILE_CEILING_FAN: ceilingFanAdd    ( x << TILE_BITS, (y << TILE_BITS)-1 ); break;
                    case TILE_TOPIC_1:     topic_init       ( x << TILE_BITS, y << TILE_BITS, type - TILE_TOPIC_1 ); break;
                    case TILE_TOPIC_2:     topic_init       ( x << TILE_BITS, y << TILE_BITS, type - TILE_TOPIC_1 ); break;
                    case TILE_TOPIC_3:     topic_init       ( x << TILE_BITS, y << TILE_BITS, type - TILE_TOPIC_1 ); break;
                    case TILE_TOPIC_4:     topic_init       ( x << TILE_BITS, y << TILE_BITS, type - TILE_TOPIC_1 ); break;
                    case TILE_PHONE:       phoneAdd         ( ( x << TILE_BITS ) - ( ( IS_LEVEL_BONUS ) ? 4 : 0), (y << TILE_BITS)+1 ); break;
				}

				//Floors Y pos stuff
				if ( x == 0 && type == TILE_FLOOR )
				{
					floor_left[ floor_left_cnt++ ] = y << TILE_BITS;
				}
				if ( x == 39 && type == TILE_FLOOR )
				{
					floor_right[ floor_right_cnt++ ] = y << TILE_BITS;
				}
			}
		}

		SPR_update( );

	VDP_setEnable( TRUE );
}


u8 map_getTile( s16 x, s16 y )
{
	if ( x >= ( 40 << TILE_BITS ) || x < ( 0 << TILE_BITS ) )
	{
		return TILE_FLOOR;
	}

	if ( y >= ( 28 << TILE_BITS ) || y < ( 0 << TILE_BITS ) )
	{
		return TILE_LADDER;
	}

	return tilemaps[ lvl ][ ( x >> TILE_BITS ) + ( ( y >> TILE_BITS ) * 40 ) ];
}

s16 getRandomLeftFloorPositionY( )
{
	const u8 floorId = random( ) % floor_left_cnt;

	return floor_left[ floorId ];
}

s16 getRandomRightFloorPositionY( ) // TODO: METER RIGHTFLOOR COMO STATIC
{
	const u8 floorId = random( ) % floor_right_cnt;

	return floor_right[ floorId ];
}
