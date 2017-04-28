#include <genesis.h>
#include "../res/rescomp.h"
#include "../inc/game.h"
#include "../inc/joyreader.h"

/*
 * Jack Nolddor, [14.09.15 20:35]
@Sr_Presley sound test / selecionar nivel / Invencibilidad... any more para la pantalla de debug?

Jack Nolddor, [14.09.15 20:49]
music on / off
sfx On / Off
 */

#define IMG_ATTRIBUTES ( TILE_ATTR_FULL( PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX ) )
#define TXT_ATTRIBUTES ( TILE_ATTR( PAL0, FALSE, FALSE, FALSE ) )

static void drawDebugScreenGraphics( void )
{
	VDP_waitVSync( );

	VDP_setEnable( FALSE );

		VDP_drawImageEx( BPLAN, &debugScreenImg, IMG_ATTRIBUTES, 0, 0, TRUE, FALSE );

	VDP_setEnable( TRUE );
}

static void cleanScreen( void )
{
	VDP_setEnable( FALSE ) ;

	VDP_setHorizontalScroll(PLAN_B, 0 );
	VDP_setVerticalScroll(PLAN_B, 0 );

	VDP_clearPlan( APLAN, FALSE );
}

void showDebugScreen( void )
{
	TEST_MODE_FLAG = TRUE;

	drawDebugScreenGraphics( );

	s16 scrollOffset = 0;

	VDP_drawTextBG( APLAN, "DEBUG OPTIONS", TXT_ATTRIBUTES, 14, 2 );
	VDP_drawTextBG( APLAN, "PRESS START TO EXIT", TXT_ATTRIBUTES, 9, 25 );

	VDP_drawTextBG( APLAN, "INVINCIBILITY OFF", TXT_ATTRIBUTES, 2, 9 );
	VDP_drawTextBG( APLAN, "LEVEL SELECTION ON ", TXT_ATTRIBUTES, 2, 11 );

	do
	{
		VDP_waitVSync( );

		VDP_setHorizontalScroll(PLAN_B, scrollOffset>>1 );
		VDP_setVerticalScroll(PLAN_B, scrollOffset>>1 );

		scrollOffset++;

		JoyReaderUpdate( );

	} while( !PAD_1_PRESSED_START );

	cleanScreen( );
}
