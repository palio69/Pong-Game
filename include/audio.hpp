#pragma once

#include <SDL2/SDL_mixer.h>

class audio {
private:
	Mix_Music* aud;

public:
	audio(const char* file) :
	aud(nullptr) {
		this->aud = Mix_LoadMUS(file);
	}
	~audio() { Mix_FreeMusic(this->aud); }

	void play(const int n) { Mix_PlayMusic(this->aud, n); }
	void play_once() { Mix_PlayMusic(this->aud, 1); }
	
};