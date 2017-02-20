#include "SDL.h"

#include "Timer.hpp"

Timer::Timer() : initialtime(-1), paused(false)
{
}

Timer::~Timer()
{
}

void Timer::start()
{
	initialtime = SDL_GetTicks();
	paused = false;
}

void Timer::pause()
{
	if( !paused )
	{
		pausetime = SDL_GetTicks();
		paused = true;
	}
}

void Timer::resume()
{
	if( paused )
	{
		initialtime += ( SDL_GetTicks() - pausetime );
		paused = false;
	}
}

int Timer::time() const
{
	if( initialtime == -1 )
		return 0;
	else if( paused )
		return( pausetime - initialtime );
	
	return( SDL_GetTicks() - initialtime );
}

bool Timer::unused() const
{
	return( initialtime == -1 );
}
