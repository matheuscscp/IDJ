/// @defgroup	MOD_PLANET "Planet"
/// @{
/// 	@brief Module to declare and implement the Planet class
/// @}
/// @ingroup MOD_PLANET
/// @file Planet.hpp
/// @brief Declarations of all methods of the Planet class
/// @author Matheus Pimenta

#ifndef PLANET_HPP
#define PLANET_HPP

#include "Sprite.hpp"

class Point
{
public:
	int x;
	int y;
	
	Point (int x = 0, int y = 0);
	virtual ~Point ();
};

class Planet
{
public:
	Sprite* sprite;
	
	Point position;
	
	int hitpoints;
	
	Planet (Sprite* sprite = NULL, int hitpoints = 20, int x = 0, int y = 0);
	virtual ~Planet ();
	
	void render ();
};

#endif
