/// @defgroup	MOD_SPRITE "Sprite"
/// @{
/// 	@brief Module to declare and implement the Sprite class
/// @}
/// @ingroup MOD_SPRITE
/// @file Sprite.hpp
/// @brief Declarations of all methods of the Sprite class
/// @author Matheus Pimenta

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "SDLBase.hpp"

/// Class to hold SDL surfaces and rectangles and to render those surfaces in
/// the screen.
/// @brief Class to hold SDL surfaces
class Sprite
{
private:
	/// @brief Pointer to the SDL surface
	SDL_Surface* src_;
	
	/// @brief Clipping SDL rectangle
	SDL_Rect srcrect_;
public:
	/// Assigns NULL to the SDL surface pointer.
	/// @brief Empty constructor
	Sprite ();
	
	/// This method calls load_ to load the image file.
	/// @param filename Path to the image file.
	/// @brief Loading constructor
	Sprite (const std::string& filename);
	
	/// This method calls unload to free the SDL surface.
	/// @brief Unloading constructor
	~Sprite ();
	
	/// This method calls unload and then load_.
	/// @param filename Path to the image file.
	/// @brief Load an image
	void load (const std::string& filename);
private:
	/// This method takes the surface from SDLBase method and sets the
	/// rectangle to take the whole image.
	/// @param filename Path to image file.
	/// @see SDLBase::loadIMG
	/// @brief Load an image
	void load_ (const std::string& filename);
	
	/// This method frees the memory used by the surface and set its pointer
	/// to NULL.
	/// @brief Frees the memory used by the surface
	void unload ();
public:
	/// This method sets the rectangle to take a piece of the surface.
	/// @param x Position in x axis of the surface.
	/// @param y Position in y axis of the surface.
	/// @param w Rectangle width.
	/// @param h Rectangle height.
	/// @brief Clip the surface with the rectangle
	void clip (const int& x, const int& y, const int& w, const int& h);
	
	/// @return A reference to the rectangle.
	/// @brief Access method to the rectangle
	const SDL_Rect& srcrect () const;
	
	/// @return A reference to the surface width.
	/// @brief Access method to the surface width
	const int& w () const;
	
	/// @return A reference to the surface height.
	/// @brief Access method to the surface height
	const int& h () const;
	
	/// This method renders the clipped surface in the screen using the
	/// SDLBase method.
	/// @param x Position in x axis of the screen surface.
	/// @param y Position in y axis of the screen surface.
	/// @see SDLBase::renderSurface
	/// @brief Render the clipped surface
	void render (int x, int y);
};

#endif
