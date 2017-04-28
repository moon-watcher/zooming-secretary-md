#include <genesis.h>
#include "../inc/npc.h"

/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 currentNPC = NULL;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcReset( void )
{
	currentNPC = NULL;
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcInitialize ( s16 positionX, s16 positionY, u8 npcType )
{
	currentNPC = npcType;

	if ( currentNPC ==  NPC_CHIEF )
	{
		npcChiefInitialize( positionX, positionY );
	}

	else if  ( currentNPC ==  NPC_BOUNCER )
	{
		npcBouncerInitialize( positionX, positionY );
	}

	else if  ( currentNPC ==  NPC_GHOST )
	{
		npcGhostInitialize( positionX, positionY );
	}

	else if  ( currentNPC ==  NPC_CHATTER )
	{
		npcChatterInitialize( positionX, positionY );
	}

	else if  ( currentNPC ==  NPC_GEEK )
	{
		npcGeekInitialize( positionX, positionY );
	}

    else if  ( currentNPC ==  NPC_MANBOX )
    {
        npcManboxInitialize( positionX, positionY );
    }

    else if  ( currentNPC ==  NPC_DIBROV )
    {
        npcDibrovInitialize( positionX, positionY );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void npcUpdate ( void )
{
	if ( currentNPC ==  NPC_CHIEF )
	{
		npcChiefUpdate( );
	}

	else if  ( currentNPC ==  NPC_BOUNCER )
	{
		npcBouncerUpdate( );
	}

	else if  ( currentNPC ==  NPC_GHOST )
	{
		npcGhostUpdate( );
	}

	else if  ( currentNPC ==  NPC_CHATTER )
	{
		npcChatterUpdate( );
	}

	else if  ( currentNPC ==  NPC_GEEK )
	{
		npcGeekUpdate( );
	}

	else if  ( currentNPC ==  NPC_MANBOX )
	{
		npcManboxUpdate( );
	}

	else if  ( currentNPC ==  NPC_DIBROV )
    {
        npcDibrovUpdate( );
    }
}


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */
