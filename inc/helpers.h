#ifndef _HELPERS_H_
#define _HELPERS_H_

    #include "music.h"
    #include "sfx.h"

	void   drawText      ( const char *str, u16 x, u16 y );
    void   drawInt       ( u32 nb, u8 x, u8 y, u8 zeros );
    void   drawUInt      ( u32 nb, u8 x, u8 y, u8 zeros );

    void   stoupper      (char *s);

    void   playMusic     ( u8 track );
    Music *getMusic      ( u8 track );

    void   initSfx       ( );
    void   playSfx       ( u8 sfx );
    Sfx   *getSfx        ( u8 sfx );

    u8     getHz         ( );
    void   waitHz        ( u16 hz );
    void   waitSc        ( u16 sc );

    void   resetScreen   ( );


#endif // _HELPERS_H_
