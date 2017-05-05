#include <genesis.h>

#include "../inc/music.h"


static Music *current;
static u8     paused;





void musicInit ( )
{
    current->track = NULL;
    current = NULL;
    paused = 0;
}


void musicPlay ( Music *track )
{
	if ( track == NULL || track->track == NULL )
	{
		musicStop();
	}
	else if ( track->id != current->id )
	{
		u8 playing = XGM_isPlaying();

		musicStop();

		if ( playing )
		{
			VDP_waitVSync();

			//#ifdef SGDKv122a
			VDP_waitVSync();
			VDP_waitVSync();
			//#endif // SGDKv122a
		}

		XGM_startPlay ( track->track );
		XGM_setForceDelayDMA ( 1 );
		XGM_setLoopNumber ( track->loop );

		current = track;
	}
}


void musicStop ( )
{
	musicInit ( );
	XGM_stopPlay();
}


void musicPause ( )
{
	if ( XGM_isPlaying() )
	{
		XGM_pausePlay();
        paused = 1;
	}

	//XGM_stopPlayPCM ( SOUND_PCM_CH1 );  // prevents long samples
	//XGM_stopPlayPCM ( SOUND_PCM_CH2 );
	//XGM_stopPlayPCM ( SOUND_PCM_CH3 );
	//XGM_stopPlayPCM ( SOUND_PCM_CH4 );
}


void musicResume ( )
{
	if ( paused && !XGM_isPlaying() )
	{
		XGM_resumePlay();
		paused = 0;
	}
}

