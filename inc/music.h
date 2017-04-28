#ifndef _MUSIC_H_
#define _MUSIC_H_



typedef struct
{
    u8  id;
	u8 *title;
	u8 *track;
	s8  loop;
}
Music;



void musicInit   ( );
void musicPlay   ( Music *track );
void musicStop   ( );
void musicPause  ( );
void musicResume ( );

#endif // _MUSIC_H_
