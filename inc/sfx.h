#ifndef _SFX_H_
#define _SFX_H_



typedef struct
{
    u8   id;
	u8  *title;
	u8  *data;
	u32  length;
}
Sfx;


void sfxInit ( );
void sfxPlay ( Sfx *sfx );
void sfxStop ( s8 channel );
void sfxMute ( );


#endif // _SFX_H_
