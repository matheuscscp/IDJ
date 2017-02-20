#ifndef ACCOBJECT_HPP
#define ACCOBJECT_HPP

#include "Geometry.hpp"
#include "Animation.hpp"

class AccObject : public Circle
{
protected:
	Animation* animation;
	Animation* turn;
	
	unsigned int switch_time;
	bool side;
public:
	int hp;
protected:
	lalge::R2Vector v;
	lalge::R2Vector a;
	
	lalge::Scalar omega;
	
	lalge::Scalar acceleration;
public:
	AccObject (
		const lalge::R2Vector& r,
		const lalge::Scalar& depthconst,
		Animation* animation,
		Animation* turn,
		int hp
	);
	virtual ~AccObject ();
	
	virtual void update ();
	
	virtual void render ();
	
	virtual GameObject* clone () const;
	
	void setAnimation (Animation* animation);
protected:
	virtual void handleKeyDown ();
	virtual void handleKeyUp ();
	
	void setSide (lalge::Scalar omega_value);
};

#endif
