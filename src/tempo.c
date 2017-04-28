#include <genesis.h>

#include "../inc/tempo.h"


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static u8 musicTempo;


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void initMusicTempo ()
{
    setMusicTempo ( DEFAULT_MUSIC_TEMPO );
}


void setMusicTempo ( u8 tempo )
{
    musicTempo = tempo;
}


u8 getMusicTempo ( )
{
    return musicTempo;
}


u8 getDefaultMusicTempo ( )
{
    return DEFAULT_MUSIC_TEMPO;
}
