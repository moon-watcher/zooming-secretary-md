#include <genesis.h>

#include "../inc/music.h"



static Music *current;



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
	if ( XGM_isPlaying() )
	{
        XGM_stopPlay();
        musicInit ( );
	}
}


void musicPause ( )
{
	if ( XGM_isPlaying() )
	{
		XGM_pausePlay();
	}
}


void musicResume ( )
{
	if ( !XGM_isPlaying() )
	{
		XGM_resumePlay();
	}
}

