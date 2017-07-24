#ifndef _MAP_H_
#define _MAP_H_


    // Floors Y position stuffs used by NPCs
    #define FLOORS_MAX	4           // MAX floors allowed in the same level

    //Kind of tiles
    #define TILE_FLOOR          1
    #define TILE_LADDER         2
    #define TILE_PHONE          4
    #define TILE_COFFEE         5
    #define TILE_PLAYER         6
    #define TILE_TOPIC_1        7   // EARTH
    #define TILE_TOPIC_2        8   // HISTORY
    #define TILE_TOPIC_3        9   // BOOKS
    #define TILE_TOPIC_4        10  // SPACE
    #define TILE_CEILING_FAN    18
    #define TILE_SPIDER         19

    u8 floor_left_cnt;              // Counter of current left floors
    u8 floor_right_cnt;             // Counter of current right floors
    s16 floor_left [FLOORS_MAX];    // Y position of current left floors
    s16 floor_right[FLOORS_MAX];    // Y position of current right floors

    // Public functions
    void map_run( void );
    u8 map_getTile( s16 x, s16 y );

    s16 getRandomLeftFloorPositionY( );
    s16 getRandomRightFloorPositionY( );


#endif // _MAP_H_

