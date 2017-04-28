#ifndef _COMMON_H_
#define _COMMON_H_

// Directions available in-game
typedef enum
{
   DIR_RIGHT    = 0,
   DIR_LEFT     = 1,
   DIR_UP       = 2,
   DIR_DOWN     = 3,
   DIR_ALL      = 4,
   DIR_NONE     = 5

} dir;

//TODO: Implement this in SGDK (pull request vía twitter)
#define ABS(n) ( ((n)<0) ? (0-(n)) : (n) )

//Used for simulating float type
#define FP_BITS 4

// Tile to Pixel
#define TILE_BITS 3

//Dev Notes: Sprite plane has a 128px offset, coordinate(x=0,y=0) is in fact (128,128)
#define SPR_OFFSET  128

#endif // _COMMON_H_
