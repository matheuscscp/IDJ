/// @ingroup MOD_SDLBASE
/// @file SDLBase.cpp
/// @brief Implementations of all methods of the SDLBase class
/// @author Matheus Pimenta

#include "../include/mod/simplestructures.hpp"

#include "../include/SDLBase.hpp"

using namespace std;

SDL_Surface* SDLBase::screen_ = NULL;
unsigned int SDLBase::dt_ = 0;
unsigned int SDLBase::fps = 0;

void SDLBase::initSDL (
	const int& w, const int& h, const int& bpp,
	const char* title, const char* icon,
	unsigned int fps
)
{
	if ( screen_ )
		throw ( mexception ( "SDL already on" ) );
	
	if ( SDL_Init ( SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER ) )
		throw ( mexception ( "SDL_Init error" ) );
	
	SDL_WM_SetCaption ( title, title );
	
	if ( icon )
	{
		SDL_Surface* tmp = IMG_Load ( icon );
		if ( !tmp )
			throw ( mexception ( "IMG_Load error" ) );
		
		SDL_WM_SetIcon ( tmp, NULL );
		SDL_FreeSurface ( tmp );
	}
	
	screen_ = SDL_SetVideoMode ( w, h, bpp, SDL_SWSURFACE );
	if ( !screen_ )
		throw ( mexception ( "SDL_SetVideoMode error" ) );
	
	SDLBase::fps = fps;
}

void SDLBase::closeSDL ()
{
	if ( !screen_ )
		throw ( mexception ( "SDL already off" ) );
	
	SDL_Quit ();
	screen_ = NULL;
}

SDL_Surface* SDLBase::screen ()
{
	if ( !screen_ )
		throw ( mexception ( "SDL still off" ) );
	
	return screen_;
}

SDL_Surface* SDLBase::loadIMG (const string& filename)
{
	if ( !screen_ )
		throw ( mexception ( "SDL still off" ) );
	
	SDL_Surface* tmp = NULL;
	SDL_Surface* ret = NULL;
	
	tmp = IMG_Load ( filename.c_str () );
	if ( !tmp )
		throw ( mexception ( "IMG_Load error" ) );
	
	if ( tmp->format->Amask )
		ret = SDL_DisplayFormatAlpha ( tmp );
	else
		ret = SDL_DisplayFormat ( tmp );
	if ( !ret )
		throw ( mexception ( "SDL display format conversion error" ) );
	
	SDL_FreeSurface ( tmp );
	
	return ret;
}

void SDLBase::renderSurface (
	SDL_Surface* src,
	SDL_Rect* srcrect,
	SDL_Rect* dstrect
)
{
	SDL_BlitSurface ( src, srcrect, screen_, dstrect );
}

void SDLBase::delayFrame ()
{
	static unsigned int t = 0;
	unsigned int frame_size = 1000 / fps;
	
	dt_ = SDL_GetTicks () - t;
	
	if ( dt_ < frame_size )
		SDL_Delay ( frame_size - dt_ );
	
	t = SDL_GetTicks ();
	
	dt_ = ( ( dt_ < frame_size ) ? frame_size : dt_ );
}

unsigned int SDLBase::dt ()
{
	return dt_;
}

float SDLBase::realFPS ()
{
	return ( (float) 1000 / dt_ );
}

void SDLBase::updateScreen ()
{
	SDL_Flip ( screen_ );
}
