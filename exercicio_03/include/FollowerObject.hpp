#ifndef FOLLOWEROBJECT_HPP
#define FOLLOWEROBJECT_HPP

#include <queue>
#include <list>

#include "GameObject.hpp"
#include "Sprite.hpp"

class FollowerObject : public GameObject
{
public:
	Sprite* sprite;
protected:
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
	
	bool mouseInside () const;
private:
	void handleMouseDownRight ();
public:
	void renderLines ();
};

#endif
