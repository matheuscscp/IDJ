#include "simplestructures.hpp"

#include "Audio.hpp"

using std::string;

Audio::Audio(const std::string& filename) :
filename(filename), sound(NULL), music(NULL)
{
	if( filename[ filename.size() - 1 ] == 'v' )
	{
		sound = Mix_LoadWAV( filename.c_str() );
		if( !sound )
			throw( mexception( "Mix_LoadWAV error" ) );
	}
	else
	{
		music = Mix_LoadMUS( filename.c_str() );
		if( !music )
			throw( mexception( "Mix_LoadMUS error" ) );
	}
}

Audio::~Audio()
{
	if( sound )
		Mix_FreeChunk( sound );
	if( music )
		Mix_FreeMusic( music );
}

void Audio::play(int n)
{
	if( sound )
		channel = Mix_PlayChannel( -1, sound, n - 1 );
	else
		Mix_PlayMusic( music, n - 1 );
}

void Audio::stop(int fade)
{
	if( sound )
		Mix_HaltChannel( channel );
	else
		Mix_FadeOutMusic( fade );
}
