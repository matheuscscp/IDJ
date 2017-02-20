/// @ingroup MOD_SDLBASE
/// @file SDLBase.cpp
/// @brief Implementations of all methods of the SDLBase class
/// @author Matheus Pimenta

#include "SDL_image.h"
#include "SDL_rotozoom.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "simplestructures.hpp"
#include "configfile.hpp"

#include "SDLBase.hpp"

#define SDL_WIDTH	800
#define SDL_HEIGHT	600
#define SDL_BPP		32
#define SDL_TITLE	"Game"
#define SDL_ICON	""
#define SDL_FPS 	30

using namespace lalge;

using std::string;

SDL_Surface* SDLBase::screen_ = NULL;
unsigned int SDLBase::dt_ = 0;
unsigned int SDLBase::fps = 0;

void readSDLConf(
	const string& confpath,
	int& w, int& h, int& bpp,
	string& title, string& icon,
	unsigned int& fps
)
{
	Configuration tmp;
	
	try {
		tmp.readTxt( confpath );
		
		try {
			w = tmp.getInt( "w" );
		} catch (Configuration::VarNotFound& e) {
			w = SDL_WIDTH;
		}
		
		try {
			h = tmp.getInt( "h" );
		} catch (Configuration::VarNotFound& e) {
			h = SDL_HEIGHT;
		}
		
		try {
			bpp = tmp.getInt( "bpp" );
		} catch (Configuration::VarNotFound& e) {
			bpp = SDL_BPP;
		}
		
		try {
			title = tmp.getStr( "title" );
		} catch (Configuration::VarNotFound& e) {
			title = SDL_TITLE;
		}
		
		try {
			icon = tmp.getStr( "icon" );
		} catch (Configuration::VarNotFound& e) {
			icon = SDL_ICON;
		}
		
		try {
			fps = tmp.getInt( "fps" );
		} catch (Configuration::VarNotFound& e) {
			fps = SDL_FPS;
		}
		
	} catch (Configuration::FileNotFound& e) {
		
		w = SDL_WIDTH;
		h = SDL_HEIGHT;
		bpp = SDL_BPP;
		title = SDL_TITLE;
		icon = SDL_ICON;
		fps = SDL_FPS;
	}
}

void SDLBase::initSDL(const string& confpath)
{
	int w, h, bpp;
	string title, icon;
	unsigned int fps;
	
	readSDLConf( confpath, w, h, bpp, title, icon, fps );
	
	if ( screen_ )
		throw ( mexception ( "SDL already on" ) );
	
	if ( SDL_Init ( SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_TIMER ) )
		throw ( mexception ( "SDL_Init error" ) );
	
	SDL_WM_SetCaption ( title.c_str(), title.c_str() );
	
	if ( icon.c_str() )
	{
		SDL_Surface* tmp = IMG_Load ( icon.c_str() );
		if ( tmp )
		{
			SDL_WM_SetIcon ( tmp, NULL );
			SDL_FreeSurface ( tmp );
		}
	}
	
	screen_ = SDL_SetVideoMode ( w, h, bpp, SDL_SWSURFACE );
	if ( !screen_ )
		throw ( mexception ( "SDL_SetVideoMode error" ) );
	
	SDLBase::fps = fps;
	
	if( TTF_Init() )
		throw( mexception( "TTF_Init error" ) );
	
	if( Mix_OpenAudio(
		MIX_DEFAULT_FREQUENCY,
		MIX_DEFAULT_FORMAT,
		MIX_DEFAULT_CHANNELS,
		1024
	) )
	{
		throw( mexception( "Mix_OpenAudio error" ) );
	}
}

void SDLBase::closeSDL()
{
	Mix_CloseAudio();
	
	TTF_Quit();
	
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

float SDLBase::FPS ()
{
	return ( (float) 1000 / dt_ );
}

void SDLBase::setFPS(unsigned int fps)
{
	if( fps )
		SDLBase::fps = fps;
}

void SDLBase::updateScreen ()
{
	SDL_Flip ( screen_ );
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

void SDLBase::drawLine(
	const R2Vector& beg,
	const R2Vector& end,
	int rgb,
	unsigned int spacing
)
{
	try {
		R2Vector lineseg( end - beg );
		R2Vector delta( lineseg.unitvec() );
		R2Vector tmp( beg );
		
		SDL_Rect pixel;
		pixel.w = 1;
		pixel.h = 1;
		
		int i = 0;
		while( i < lineseg.length() )
		{
			unsigned int j = 0;
			while( ( i < lineseg.length() ) && ( ( j < spacing ) || ( !spacing ) ) )
			{
				pixel.x = tmp.x( 0 );
				pixel.y = tmp.x( 1 );
				
				SDL_FillRect( screen_, &pixel, rgb );
				
				tmp += delta;
				i++;
				
				j++;
			}
			
			tmp += ( delta * spacing );
			i += spacing;
		}
		
	} catch(DirectionNotDefined& e) {
	}
}

SDL_Color SDLBase::getColor(int r, int g, int b)
{
	SDL_Color color;
	
	color.r = r;
	color.g = g;
	color.b = b;
	
	return color;
}
