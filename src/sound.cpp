#include <cassert>
#include <iostream>
#include <SDL2/SDL.h>
#include "sound.hpp"

static Uint32 wav_len;
static Uint8 *wav_data;
static SDL_AudioSpec wav_spec;

static Uint32 play_pos;
static Uint8 *data_pos;

void audio_callback(void *, Uint8 *stream, int _len)
{
	if (play_pos == 0)
		return;

	Uint32 len = static_cast<Uint32>(_len);
	
	len = ( len > play_pos ? play_pos : len );
	SDL_MixAudio(stream, data_pos, len, SDL_MIX_MAXVOLUME);// mix from one buffer into another
	
	data_pos += len;
	play_pos -= len;
}

Status init_audio()
{
	Status success = Okay;

	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		success = Error;
	}

	if (success && SDL_LoadWAV("./res/blip.wav", &wav_spec, &wav_data, &wav_len) == NULL)
	{
		std::cerr << "Failed to load ./res/blip.wav\n";
		success = Error;
	}

	if (success)
	{
		// set the callback function
		wav_spec.callback = audio_callback;
		wav_spec.userdata = NULL;
	}
		
	return success;
}

void term_audio()
{
	SDL_FreeWAV(wav_data);
}

void start_blip()
{
	// set our global static variables
	data_pos = wav_data;
	play_pos = wav_len;
	
	if ( SDL_OpenAudio(&wav_spec, NULL) < 0 )
	{
		std::cerr << "Failed to open audio " << SDL_GetError() << std::endl;
	}
	else
	{
		SDL_PauseAudio(0);
	}
}

void play_blip()
{
	while (play_pos > 0)
	{
		SDL_Delay(100);
	}
}

void stop_blip()
{
	SDL_CloseAudio();
}


