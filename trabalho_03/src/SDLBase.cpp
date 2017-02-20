/// @ingroup MOD_SDLBASE
/// @file SDLBase.cpp
/// @brief Implementations of all methods of the SDLBase class
/// @author Matheus Pimenta

#include "SDL_image.h"
#include "SDL_rotozoom.h"

#define SIMPLES_NOCOUT
#include "mod/simplestructures.hpp"

#include "SDLBase.hpp"

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

void SDLBase::drawLine(int x1, int y1, int x2, int y2, int rgb, int spacing)
{
	int deltax = abs(x2 - x1); // The difference between the x's
	int deltay = abs(y2 - y1); // The difference between the y's
	int x = x1;                // Start x off at the first pixel
	int y = y1;                // Start y off at the first pixel
	int xinc1 = 0;
	int yinc1 = 0;
	int xinc2 = 0;
	int yinc2 = 0;
	int den = 0;
	int num = 0;
	int numadd = 0;
	int numpixels = 0;

	if (x2 >= x1)              // The x-values are increasing
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else                       // The x-values are decreasing
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1)              // The y-values are increasing
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else                       // The y-values are decreasing
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)		// There is at least one x-value for every y-value
	{
		xinc1 = 0;				// Don't change the x when numerator >= denominator
		yinc2 = 0;				// Don't change the y for every iteration
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;		// There are more x-values than y-values
	}
	else						// There is at least one y-value for every x-value
	{
		xinc2 = 0;				// Don't change the x for every iteration
		yinc1 = 0;				// Don't change the y when numerator >= denominator
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;		// There are more y-values than x-values
	}

	for (int curpixel = 1; curpixel <= numpixels; curpixel++)
	{
		if (spacing == 0)
		{
			SDL_Rect pixel = {x,y,1,1};
			SDL_FillRect(screen_,&pixel,rgb);
		}
		else if (curpixel % spacing >= spacing/2)
		{
			SDL_Rect pixel = {x,y,1,1};
			SDL_FillRect(screen_,&pixel,rgb);
		}

		num += numadd;         // Increase the numerator by the top of the fraction
		if (num >= den)        // Check if numerator >= denominator
		{
			num -= den;         // Calculate the new numerator value
			x += xinc1;         // Change the x as appropriate
			y += yinc1;         // Change the y as appropriate
		}
		x += xinc2;             // Change the x as appropriate
		y += yinc2;             // Change the y as appropriate
	}
}

SDL_Surface* SDLBase::rotozoom (
	SDL_Surface* src, float angle, float zoomx, float zoomy
)
{
	if ( !( ( src ) && ( ( zoomx > 0 ) && ( zoomy > 0 ) ) ) )
		throw ( mexception ( "Invalid SDLBase::rotozoom call" ) );
	
	return rotozoomSurfaceXY ( src, angle, zoomx, zoomy, 1 );
}

SDL_Surface* SDLBase::clip (SDL_Surface* src, SDL_Rect* rect)
{
	if ( !( ( src ) && ( rect ) ) )
		throw ( mexception ( "Invalid SDLBase::clip call" ) );
	
	SDL_Surface* tmp = SDL_CreateRGBSurface (
		SDL_SWSURFACE, rect->w, rect->h, screen_->format->BitsPerPixel,
		0, 0, 0, 0
	);
	
	SDL_BlitSurface ( src, rect, tmp, NULL );
	
	return tmp;
}
