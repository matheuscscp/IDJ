/// @defgroup	MOD_SDLBASE "SDL Base"
/// @{
/// 	@brief Module to declare and implement the SDLBase class
/// @}
/// @ingroup MOD_SDLBASE
/// @file SDLBase.hpp
/// @brief Declarations of all methods of the SDLBase class
/// @author Matheus Pimenta

#ifndef SDLBASE_HPP
#define SDLBASE_HPP

#include <string>

#include <SDL/SDL_image.h>

/// Made to ease the use of SDL, this class encapsulates some of the features of
/// this library.
/// @brief Class to encapsulate some of SDL features
class SDLBase
{
private:
	/// @brief Pointer to the main SDL surface: the screen
	static SDL_Surface* screen_;
	
	/// @brief Delta-time of the last frame
	static unsigned int dt_;
public:
	/// @brief Frames-per-second rate
	static unsigned int fps;
	
	/// This method initializes everything is needed in SDL.
	/// @param w Screen width.
	/// @param h Screen height.
	/// @param bpp Bits per pixels.
	/// @param title Window title.
	/// @param icon Path to the icon image file.
	/// @throw mexception Thrown if SDL couldn't initialize correctly, or if
	/// it was already initialized.
	/// @brief Initializes SDL
	static void initSDL (
		const int& w, const int& h, const int& bpp,
		const char* title = NULL,
		const char* icon = NULL,
		unsigned int fps = 0
	);
	
	/// This method closes the entire library and frees the memory.
	/// @throw mexception Thrown if the SDL system was already off.
	/// @brief Closes SDL
	static void closeSDL ();
	
	/// @return Pointer to the screen surface.
	/// @throw mexception Thrown if SDL wasn't initialized yet.
	/// @brief Access method to screen
	static SDL_Surface* screen ();
	
	/// This method loads an image file from disk and returns its display
	/// formatted surface.
	/// @param filename Path to the image file.
	/// @return Surface with display formatted image.
	/// @throw mexception Thrown if SDL wasn't initialized yet, or if it was
	/// not possible to load the image file.
	/// @brief Load an image from disk
	static SDL_Surface* loadIMG (const std::string& filename);
	
	/// This method paste the source image in the screen.
	/// @param src Image to be pasted in the screen.
	/// @param srcrect Rectangle in the image that will be pasted in the
	/// screen.
	/// @param dstrect Source position in the screen.
	/// @throw mexception Thrown if SDL wasn't initialized yet, or if it was
	/// not possible to blit the surface.
	/// @brief Blit a surface in the screen
	static void renderSurface (
		SDL_Surface* src,
		SDL_Rect* srcrect = NULL,
		SDL_Rect* dstrect = NULL
	);
	
	/// This method delays a frame to control frames-per-second rate, or
	/// warns of big frame.
	/// @brief Controls the frames-per-second rate
	static void delayFrame ();
	
	/// @return Delta-time in milliseconds of the last frame
	/// @brief Access method to frame delta-time
	static unsigned int dt ();
	
	/// @return The real frames-per-second rate that's being reached
	/// @brief Access method to frame delta-time
	static float realFPS ();
	
	/// This method shows in the screen what is the screen SDL surface.
	/// @throw mexception Thrown if SDL wasn't initialized yet, or if it was
	/// not possible to update the screen.
	/// @brief Shows the screen
	static void updateScreen ();
	
	static void drawLine (int x1, int y1, int x2, int y2, int rgb, int spacing);
};

#endif
