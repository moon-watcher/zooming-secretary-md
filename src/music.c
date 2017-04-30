#include <genesis.h>

#include "../inc/music.h"


static Music *current = NULL;


void musicInit ( )
{
	current->track = NULL;
	current = NULL;
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
	musicPause ( );
}


void musicPause ( )
{
	if ( XGM_isPlaying() )
	{
		XGM_pausePlay();
	}

	XGM_stopPlayPCM ( SOUND_PCM_CH1 );  // prevents long samples
	//XGM_stopPlayPCM ( SOUND_PCM_CH2 );
	//XGM_stopPlayPCM ( SOUND_PCM_CH3 );
	//XGM_stopPlayPCM ( SOUND_PCM_CH4 );
}


void musicResume ( )
{
	if ( !XGM_isPlaying() )
	{
		XGM_resumePlay();
	}
}

