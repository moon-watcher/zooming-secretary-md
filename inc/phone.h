#ifndef _PHONE_H_
#define _PHONE_H_

    void    phoneReset				( void );
    void    phoneAdd				( s16 x, s16 y );
    void 	phoneUpdate				( void );
    u8      phoneCheckCollision		( s16 obj_x, s16 obj_xx, s16 obj_y, s16 obj_yy );

#endif // _PHONE_H_
