#include <genesis.h>
#include "../inc/joyreader.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void JoyReaderInit ( )
{
	memset( joysticks, NULL, sizeof( joysticks ) );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void JoyReaderUpdate ( void )
{
	u16 active;

	#if PAD_MAX > 0

		active		 = JOY_readJoypad ( JOY_1 );
		joysticks[0] = ( Joyreader ) { active, (active ^ joysticks[0].active) };

	#endif

	#if PAD_MAX > 1

		active		 = JOY_readJoypad ( JOY_2 );
		joysticks[1] = ( Joyreader ) { active, (active ^ joysticks[1].active) };

	#endif

	#if PAD_MAX	> 2

		active		 = JOY_readJoypad ( JOY_3 );
		joysticks[2] = ( Joyreader ) { active, (active ^ joysticks[2].active) };

	#endif

	#if PAD_MAX	> 3

		active		 = JOY_readJoypad ( JOY_4 );
		joysticks[3] = ( Joyreader ) { active, (active ^ joysticks[3].active) };

	#endif

	#if PAD_MAX	> 4

		active		 = JOY_readJoypad ( JOY_5 );
		joysticks[4] = ( Joyreader ) { active, (active ^ joysticks[4].active) };

	#endif

	#if PAD_MAX	> 5

		active		 = JOY_readJoypad ( JOY_6 );
		joysticks[5] = ( Joyreader ) { active, (active ^ joysticks[5].active) };

	#endif

	#if PAD_MAX	> 6

		active		 = JOY_readJoypad ( JOY_7 );
		joysticks[6] = ( Joyreader ) { active, (active ^ joysticks[6].active) };

	#endif

	#if PAD_MAX	> 7

		active		 = JOY_readJoypad ( JOY_8 );
		joysticks[7] = ( Joyreader ) { active, (active ^ joysticks[7].active) };

	#endif
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void JoyReaderReset ( )
{
    JoyReaderInit ( );
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
