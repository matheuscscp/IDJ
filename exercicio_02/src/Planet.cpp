/// @ingroup MOD_PLANET
/// @file Planet.cpp
/// @brief Implementations of all methods of the Planet class
/// @author Matheus Pimenta

#include "../include/Planet.hpp"

Planet::Planet (Sprite* sprite, int hitpoints, int x, int y)
{
	this->sprite = sprite;
	this->position.x = x;
	this->position.y = y;
	this->hitpoints = hitpoints;
}

Planet::~Planet ()
{
}

void Planet::render (float cameraX, float cameraY)
{
	if ( !sprite )
		throw ( mexception ( "No sprite to render planet" ) );
	
	sprite->render ( cameraX + position.x, cameraY + position.y );
}
