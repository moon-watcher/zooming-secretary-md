#include <genesis.h>

#include "../inc/sfx.h"
#include "../inc/psg.h"



static u8 chnInc = 0;


void sfxInit ( )
{
    chnInc = 0;
}


void sfxPlay ( Sfx *sfx )
{
    switch ( sfx->driver )
    {
        case SFX_DRIVER_PSG:
            psg_play ( sfx->data, sfx->track );
            break;

        case SFX_DRIVER_XMG:
            if ( chnInc == 0 ) chnInc = 1;
            sfxStop ( chnInc );
            XGM_startPlayPCM ( 64 + sfx->id, 1, chnInc++ );
            chnInc %= 4;
            break;
    }
}


void sfxStop ( u8 channel )
{
    if ( channel < 4 )
    {
        XGM_stopPlayPCM ( channel );
    }
    else
    {
        psg_pause();
    }
}


void sfxMute ( )
{
    sfxStop ( SOUND_PCM_CH1 );
    sfxStop ( SOUND_PCM_CH2 );
    sfxStop ( SOUND_PCM_CH3 );
    sfxStop ( SOUND_PCM_CH4 );

    psg_pause();
}


void sfxUseChannel ( u8 channel )
{
    chnInc = channel;
}
