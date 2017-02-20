/// @ingroup MOD_PLANET
/// @file Planet.cpp
/// @brief Implementations of all methods of the Planet class
/// @author Matheus Pimenta

#include "Planet.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"

#define EARTH_SCALESIZE	3

using namespace lalge;

Planet::Planet (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite,
	int hitpoints
) :
Circle ( r, depthconst, sprite->srcrect ().w / 2 ),
sprite ( sprite ), hitpoints ( hitpoints )
{
}

Planet::~Planet ()
{
}

void Planet::render ()
{
	if ( sprite )
	{
		sprite->render (
			r.x ( 0 ) - sprite->srcrect ().w / 2 - Camera::r.x ( 0 ) * depthconst,
			r.x ( 1 ) - sprite->srcrect ().h / 2 - Camera::r.x ( 1 ) * depthconst
		);
	}
}

bool Planet::mouseInside () const
{
	SDL_Rect rect;
	
	rect.x = r.x ( 0 ) - sprite->srcrect ().w / 2 - Camera::r.x ( 0 ) * depthconst;
	rect.y = r.x ( 1 ) - sprite->srcrect ().h / 2 - Camera::r.x ( 1 ) * depthconst;
	rect.w = sprite->srcrect ().w;
	rect.h = sprite->srcrect ().h;
	
	return InputManager::instance ()->mouseInside ( rect );
}

int Planet::hp () const
{
	return hitpoints;
}

void Planet::setSprite (Sprite* sprite)
{
	this->sprite = sprite;
	
	setRadius ( sprite->srcrect ().w / 2 );
}

RedPlanet::RedPlanet (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite,
	int hitpoints
) :
Planet ( r, depthconst, sprite, hitpoints )
{
}

RedPlanet::~RedPlanet ()
{
}

void RedPlanet::update ()
{
}

GameObject* RedPlanet::clone () const
{
	return new RedPlanet ( *this );
}

Earth::Earth (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite,
	Scalar omega
) :
Planet ( r, depthconst, sprite ), omega ( omega )
{
	sprite->rotozoom ( 0, EARTH_SCALESIZE, EARTH_SCALESIZE );
	
	setRadius ( radius () / EARTH_SCALESIZE );
}

Earth::~Earth ()
{
}

void Earth::update ()
{
	Scalar dt = ( (Scalar) SDLBase::dt () ) / 1000;
	
	sprite->rotozoom (
		sprite->angle () + omega * dt, EARTH_SCALESIZE, EARTH_SCALESIZE
	);
}

GameObject* Earth::clone () const
{
	return new Earth ( *this );
}

Moon::Moon (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite,
	Planet* owner
) :
Planet ( r, depthconst, sprite ), owner ( owner )
{
	ucm_radius = r2vec (
		owner->radius () * EARTH_SCALESIZE * EARTH_SCALESIZE + 30, 0
	);
	omega = 90;
	
	this->r.set ( 0, owner->r.x ( 0 ) + ucm_radius.x ( 0 ) );
	this->r.set ( 1, owner->r.x ( 1 ) + ucm_radius.x ( 1 ) );
}

Moon::~Moon ()
{
}

void Moon::update ()
{
	Scalar tmp = ( (Scalar) SDLBase::dt () ) / 1000;
	
	rotate ( ucm_radius, omega * tmp );
	
	r.set ( 0, owner->r.x ( 0 ) + ucm_radius.x ( 0 ) );
	r.set ( 1, owner->r.x ( 1 ) + ucm_radius.x ( 1 ) );
}

GameObject* Moon::clone () const
{
	return new Moon ( *this );
}
