/// @ingroup MOD_SDLBASE
/// @file SDLBase.cpp
/// @brief Implementations of all methods of the SDLBase class
/// @author Matheus Pimenta

#include "../include/SDLBase.hpp"

SDL_Surface* SDLBase::screen_ = NULL;

void SDLBase::initSDL (	const int& w, const int& h, const int& bpp,
			const char* title, const char* icon	)
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

void SDLBase::renderSurface (	SDL_Surface* src,
				SDL_Rect* srcrect,
				SDL_Rect* dstrect	)
{
	if ( !screen_ )
		throw ( mexception ( "SDL still off" ) );
	
	if ( SDL_BlitSurface ( src, srcrect, screen_, dstrect ) )
		throw ( mexception ( "SDL_BlitSurface error" ) );
}

void SDLBase::updateScreen ()
{
	if ( !screen_ )
		throw ( mexception ( "SDL still off" ) );
	
	if ( SDL_Flip ( screen_ ) )
		throw ( mexception ( "SDL_Flip error" ) );
}
