#ifndef FOLLOWEROBJECT_HPP
#define FOLLOWEROBJECT_HPP

#include <queue>
#include <list>

#include "Geometry.hpp"
#include "Sprite.hpp"

class FollowerObject : public Circle
{
protected:
	Sprite* sprite;
	lalge::R2Vector v;
	lalge::R2Vector dest;
	
	std::queue< lalge::R2Vector, std::list< lalge::R2Vector > > path;
public:
	FollowerObject (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		Sprite* sprite = NULL
	);
	virtual ~FollowerObject ();
	
	virtual void update ();
	
	virtual void render ();
	
	virtual GameObject* clone () const;
	
	void setSprite (Sprite* sprite);
private:
	void handleMouseDownRight ();
public:
	void renderLines ();
};

#endif
