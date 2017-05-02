#include <genesis.h>

#include "../inc/common.h"
#include "../res/rescomp.h"
#include "../inc/music.h"
#include "../inc/sfx.h"
#include "../inc/game.h"
#include "../res/psg/sfx_steps.h"



/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


static const Music music_list [ MUSIC_MAX ] =
{
    { MUSIC_NULL,     "Silence",     (u8*) NULL,            0 },
    { MUSIC_GAME,     "Game",        (u8*) music_game,     -1 },
    { MUSIC_DREAM,    "Dream",       (u8*) music_dream,    -1 },
    { MUSIC_CLEAR,    "Stage clear", (u8*) music_clear,     0 },
    { MUSIC_NOBONUS,  "No bonus",    (u8*) music_nobonus,   0 },
    { MUSIC_LEVEL,    "Level",       (u8*) music_level,     0 },
    { MUSIC_GAMEOVER, "Game Over",   (u8*) music_gameover,  0 },
    { MUSIC_WELLDONE, "Welldone",    (u8*) music_welldone, -1 }
};



static const Sfx sfx_list [ SFX_MAX ] =
{
    { SFX_ANSWER,       "Answer",       (u8*) sfx_answer,       sizeof(sfx_answer)       },
    { SFX_BLA1,         "Blah 1",       (u8*) sfx_bla1,         sizeof(sfx_bla1)         },
    { SFX_BLA2,         "Blah 2",       (u8*) sfx_bla2,         sizeof(sfx_bla2)         },
    { SFX_COFFEE,       "Coffee",       (u8*) sfx_coffee,       sizeof(sfx_coffee)       },
    { SFX_COFFEE_READY, "Coffee ready", (u8*) sfx_coffee_ready, sizeof(sfx_coffee_ready) },
    { SFX_EXPLODE,      "Explode",      (u8*) sfx_explode,      sizeof(sfx_explode)      },
    { SFX_FALL,         "Fall",         (u8*) sfx_fall,         sizeof(sfx_fall)         },
    { SFX_KNOCK,        "Knock",        (u8*) sfx_knock,        sizeof(sfx_knock)        },
    { SFX_LOSE,         "Lose",         (u8*) sfx_lose,         sizeof(sfx_lose)         },
    { SFX_PAUSE,        "Pause",        (u8*) sfx_pause,        sizeof(sfx_pause)        },
    { SFX_RING,         "Ring",         (u8*) sfx_ring,         sizeof(sfx_ring)         },
    { SFX_START,        "Start",        (u8*) sfx_start,        sizeof(sfx_start)        },
    { SFX_STEP_1,       "Step 1",       (u8*) sfx_steps_data,   0,                       SFX_DRIVER_PSG, 0 },
    { SFX_STEP_2,       "Step 2",       (u8*) sfx_steps_data,   0,                       SFX_DRIVER_PSG, 1 },
    { SFX_STEP_3,       "Step 3",       (u8*) sfx_steps_data,   0,                       SFX_DRIVER_PSG, 2 },
    { SFX_STEP_4,       "Step 4",       (u8*) sfx_steps_data,   0,                       SFX_DRIVER_PSG, 3 },
    { SFX_TELEPORT,     "Teleport",     (u8*) sfx_teleport,     sizeof(sfx_teleport)     },
    { SFX_TOPIC,        "Topic",        (u8*) sfx_topic,        sizeof(sfx_topic)        },
    { SFX_WISDOM,       "Wisdom",       (u8*) sfx_wisdom,       sizeof(sfx_wisdom)       },
};


/* :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: */


void drawText ( const char *str, u16 x, u16 y )
{
    SYS_disableInts();
    VDP_drawText ( str, x, y );
    SYS_enableInts();
}


void drawInt ( u32 nb, u8 x, u8 y, u8 zeros )
{
    u8 str [ zeros+1 ];
    intToStr ( nb, str, zeros );

    SYS_disableInts();
    VDP_drawText ( str, x, y );
    SYS_enableInts();
}


void drawUInt ( u32 nb, u8 x, u8 y, u8 zeros )
{
    u8 str [ zeros+1 ];
    uintToStr ( nb, str, zeros );

    SYS_disableInts();
    VDP_drawText ( str, x, y );
    SYS_enableInts();
}


// https://www.daniweb.com/programming/software-development/code/217378/toupper-and-tolower-implementation-for-whole-c-strings
void stoupper(char *s)
{
    for(; *s; s++)
        if(('a' <= *s) && (*s <= 'z'))
            *s = 'A' + (*s - 'a');
}



void playMusic ( u8 track )
{
    if ( !MUSIC_MODE_FLAG )
    {
        return;
    }

    musicPlay ( (Music*) &music_list [ track ] );
}


Music *getMusic ( u8 track )
{
    return (Music*) &music_list [ track ];
}



void initSfx ( )
{
    u16 i = SFX_MAX;

    while ( i-- )
    {
        Sfx *sfx = (Sfx*) &sfx_list [ i ];

        if ( sfx->driver == SFX_DRIVER_XMG )
        {
            XGM_setPCM ( 64 + sfx->id, sfx->data, sfx->length );
        }
    }
}


void playSfx( u8 sfx )
{
    if ( !SFX_MODE_FLAG )
    {
        return;
    }

    sfxPlay ( (Sfx*) &sfx_list [ sfx ] );
}


Sfx *getSfx ( u8 sfx )
{
    return (Sfx*) &sfx_list [ sfx ];
}




u8 getHz ( )
{
    return IS_PALSYSTEM ? 50 : 60;
}


void waitHz ( u16 hz )
{
    while ( hz-- )
    {
        VDP_waitVSync();
    }
}


void waitSc ( u16 sc )
{
    waitHz ( sc * getHz() );
}
