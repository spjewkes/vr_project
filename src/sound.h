#include <SDL2/SDL.h>

#ifndef __SOUND_H__
#define __SOUND_H__

bool init_audio();
void term_audio();

void start_blip();
void play_blip();
void stop_blip();

#endif // __SOUND_H__
