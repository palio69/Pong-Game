#pragma once

#include <SDL2/SDL_mixer.h>

class sfx {
private:
	Mix_Music* fx;

public:
	sfx(const char* file) :
	fx(nullptr) {
		this->fx = Mix_LoadMUS(file);
	}
	~sfx() { Mix_FreeMusic(this->fx); }

	void play(const int n) { Mix_PlayMusic(this->fx, n); }
	void play_once() { Mix_PlayMusic(this->fx, 1); }
	
};