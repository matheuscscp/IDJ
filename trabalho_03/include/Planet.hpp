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
#include "Geometry.hpp"

class Planet : public Circle
{
protected:
	Sprite* sprite;
	int hitpoints;
public:
	Planet (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		Sprite* sprite = NULL,
		int hitpoints = 1
	);
	virtual ~Planet ();
	
	virtual void update () = 0;
	
	virtual void render ();
	
	virtual GameObject* clone () const = 0;
	
	bool mouseInside () const;
	
	int hp () const;
	
	void setSprite (Sprite* sprite);
};

class RedPlanet : public Planet
{
public:
	RedPlanet (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		Sprite* sprite = NULL,
		int hitpoints = 20
	);
	virtual ~RedPlanet ();
	
	virtual void update ();
	
	virtual GameObject* clone () const;
};

class Earth : public Planet
{
public:
	lalge::Scalar omega;
	
	Earth (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		Sprite* sprite = NULL,
		lalge::Scalar omega = 0
	);
	virtual ~Earth ();
	
	virtual void update ();
	
	virtual GameObject* clone () const;
};

class Moon : public Planet
{
public:
	lalge::R2Vector ucm_radius;
	lalge::Scalar omega;
	
	Planet* owner;
	
	Moon (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		Sprite* sprite = NULL,
		Planet* owner = NULL
	);
	virtual ~Moon ();
	
	virtual void update ();
	
	virtual GameObject* clone () const;
};

#endif
