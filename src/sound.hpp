#include <SDL2/SDL.h>

#ifndef __SOUND_HPP__
#define __SOUND_HPP__

#include "defs.hpp"

Status init_audio();
void term_audio();

void start_blip();
void play_blip();
void stop_blip();

#endif // __SOUND_HPP__
