#include <SDL.h>

#ifndef __SOUND_HPP__
#define __SOUND_HPP__

#include "defs.hpp"

/**
 * @brief Initialize SDL audio and load the collision sample.
 * @return Okay on success or Error when SDL or `./res/blip.wav` cannot be loaded.
 */
Status init_audio();

/** @brief Release the sample loaded by init_audio(). */
void term_audio();

/** @brief Reset playback state and open the SDL audio device. */
void start_blip();

/**
 * @brief Block until the collision sample has finished playing.
 * @pre start_blip() successfully opened the audio device.
 */
void play_blip();

/** @brief Close the audio device after playback. */
void stop_blip();

#endif // __SOUND_HPP__
