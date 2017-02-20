/// @ingroup MOD_PLANET
/// @file Planet.cpp
/// @brief Implementations of all methods of the Planet class
/// @author Matheus Pimenta

#include "../include/Planet.hpp"

#include "../include/InputManager.hpp"
#include "../include/Camera.hpp"

using namespace lalge;

Planet::Planet (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite,
	int hitpoints
) :
GameObject ( r, depthconst ), sprite ( sprite ), hitpoints ( hitpoints )
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
			r.x ( 0 ) - sprite->srcrect ().w / 2 - Camera::instance ()->r.x ( 0 ) * depthconst,
			r.x ( 1 ) - sprite->srcrect ().h / 2 - Camera::instance ()->r.x ( 1 ) * depthconst
		);
	}
}

bool Planet::mouseInside () const
{
	SDL_Rect rect;
	
	rect.x = r.x ( 0 ) - sprite->srcrect ().w / 2 - Camera::instance ()->r.x ( 0 ) * depthconst;
	rect.y = r.x ( 1 ) - sprite->srcrect ().h / 2 - Camera::instance ()->r.x ( 1 ) * depthconst;
	rect.w = sprite->srcrect ().w;
	rect.h = sprite->srcrect ().h;
	
	return InputManager::instance ()->mouseInside ( rect );
}

int Planet::hp () const
{
	return hitpoints;
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
	hitpoints -= ( rand () % 10 + 10 );
}

GameObject* RedPlanet::clone () const
{
	return new RedPlanet ( *this );
}

Earth::Earth (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite,
	int hitpoints
) :
Planet ( r, depthconst, sprite, hitpoints )
{
	scroll = 200;
	
	InputManager::instance ()->connect (
		InputManager::KEYDOWN,
		this,
		&Earth::handleKeyDown
	);
	InputManager::instance ()->connect (
		InputManager::KEYUP,
		this,
		&Earth::handleKeyUp
	);
}

Earth::~Earth ()
{
	InputManager::instance ()->disconnect ( this );
}

void Earth::update ()
{
	Scalar tmp = ( (Scalar) SDLBase::dt () ) / 1000;
	
	r.add ( 0, v.x ( 0 ) * tmp );
	r.add ( 1, v.x ( 1 ) * tmp );
}

GameObject* Earth::clone () const
{
	return new Earth ( *this );
}

void Earth::handleKeyDown ()
{
	switch ( ( (InputManager::Event*) InputManager::instance ()->event () )->event->key.keysym.sym )
	{
	case 'w':
		v.add ( 1, -scroll );
		break;
		
	case 's':
		v.add ( 1, scroll );
		break;
		
	case 'a':
		v.add ( 0, -scroll );
		break;
		
	case 'd':
		v.add ( 0, scroll );
		break;
		
	default:
		break;
	}
}

void Earth::handleKeyUp ()
{
	switch ( ( (InputManager::Event*) InputManager::instance ()->event () )->event->key.keysym.sym )
	{
	case 'w':
		v.add ( 1, scroll );
		break;
		
	case 's':
		v.add ( 1, -scroll );
		break;
		
	case 'a':
		v.add ( 0, scroll );
		break;
		
	case 'd':
		v.add ( 0, -scroll );
		break;
		
	default:
		break;
	}
}

Moon::Moon (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite,
	int hitpoints,
	Planet* owner
) :
Planet ( r, depthconst, sprite, hitpoints ), owner ( owner )
{
	radius = r2vec ( owner->sprite->srcrect ().w / 2, 0 );
	omega = 180;
	
	this->r.set ( 0, owner->r.x ( 0 ) + radius.x ( 0 ) );
	this->r.set ( 1, owner->r.x ( 1 ) + radius.x ( 1 ) );
}

Moon::~Moon ()
{
}

void Moon::update ()
{
	Scalar tmp = ( (Scalar) SDLBase::dt () ) / 1000;
	
	rotate ( radius, omega * tmp );
	
	r.set ( 0, owner->r.x ( 0 ) + radius.x ( 0 ) );
	r.set ( 1, owner->r.x ( 1 ) + radius.x ( 1 ) );
}

GameObject* Moon::clone () const
{
	return new Moon ( *this );
}
