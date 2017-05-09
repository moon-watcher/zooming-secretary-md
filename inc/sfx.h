#ifndef _SFX_H_
#define _SFX_H_


enum
{
    SFX_DRIVER_XMG,
    SFX_DRIVER_PSG,

    SFX_DRIVER_MAX,
};


typedef struct
{
    u8   id;
	u8  *title;
	u8  *data;
	u32  length;
	u8   driver;
	u8   track;
}
Sfx;


void sfxInit       ( );
void sfxPlay       ( Sfx *sfx );
void sfxStop       ( u8 channel );
void sfxMute       ( );
void sfxUseChannel ( u8 channel );

#endif // _SFX_H_
