#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <string>

#include "SDL_mixer.h"

class Audio
{
private:
	std::string filename;
	Mix_Chunk* sound;
	Mix_Music* music;
	int channel;
public:
	Audio(const std::string& filename);
	~Audio();
	
	void play(int n = 0);
	void stop(int fade = 0);
};

#endif
