/// @ingroup MOD_SPRITE
/// @file Sprite.cpp
/// @brief Implementations of all methods of the Sprite class
/// @author Matheus Pimenta

#include "../include/Sprite.hpp"

Sprite::Sprite () : src_ ( NULL )
{
}

Sprite::Sprite (const string& filename)
{
	load_ ( filename );
}

Sprite::~Sprite ()
{
	unload ();
}

void Sprite::load (const string& filename)
{
	unload ();
	
	load_ ( filename );
}

void Sprite::load_ (const string& filename)
{
	src_ = SDLBase::loadIMG ( filename );
	
	srcrect_.x = 0;
	srcrect_.y = 0;
	
	srcrect_.w = src_->w;
	srcrect_.h = src_->h;
}

void Sprite::unload ()
{
	if ( src_ )
	{
		SDL_FreeSurface ( src_ );
		src_ = NULL;
	}
}

void Sprite::clip (const int& x, const int& y, const int& w, const int& h)
{
	srcrect_.x = x;
	srcrect_.y = y;
	
	srcrect_.w = w;
	srcrect_.h = h;
}

const SDL_Rect& Sprite::srcrect () const
{
	return srcrect_;
}

const int& Sprite::w () const
{
	return src_->w;
}

const int& Sprite::h () const
{
	return src_->h;
}

void Sprite::render (const int& x, const int& y)
{
	if (	(	( ( x < 0 ) && ( x + srcrect_.w > 0 ) ) ||
			( ( x >= 0 ) && ( x < SDLBase::screen ()->w ) )	) &&
		(	( ( y < 0 ) && ( y + srcrect_.h > 0 ) ) ||
			( ( y >= 0 ) && ( y < SDLBase::screen ()->h ) )	)	)
	{
		SDL_Rect dstrect;
		
		dstrect.x = x;
		dstrect.y = y;
		
		SDLBase::renderSurface ( src_, &srcrect_, &dstrect );
	}
}
