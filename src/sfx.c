#include <genesis.h>

#include "../inc/sfx.h"
#include "../inc/psg.h"



static u8 _channel = 0;


void sfxInit ( )
{
    _channel = 0;
}


void sfxPlay ( Sfx *sfx )
{
    switch ( sfx->driver )
    {
        case SFX_DRIVER_PSG:
            psg_play ( sfx->data, sfx->track );
            break;

        case SFX_DRIVER_XMG:
            sfxStop ( _channel );
            XGM_startPlayPCM ( 64 + sfx->id, 1, _channel++ );
            _channel %= 4;
            break;
    }
}


void sfxStop ( s8 channel )
{
    if ( XGM_isPlayingPCM ( channel ) )
    {
        XGM_stopPlayPCM ( channel );
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
