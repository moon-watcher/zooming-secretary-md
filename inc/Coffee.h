#ifndef _COFFEE_H_
#define _COFFEE_H_


    void    coffeeDestroy ( void );
    void    coffeeInitialize( s16 basePositionX, s16 basePositionY );
    void    coffeeUpdate ( void );
    u8      coffeeCheckCollision ( s16 spr_x, s16 spr_xx, s16 spr_y, s16 spr_yy );


#endif // _COFFEE_H_

