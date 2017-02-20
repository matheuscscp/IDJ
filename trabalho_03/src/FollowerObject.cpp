#include "FollowerObject.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"

#define VELOCITY	300

using namespace std;
using namespace lalge;

FollowerObject::FollowerObject (
	const R2Vector& r,
	const Scalar& depthconst,
	Sprite* sprite
) : Circle ( r, depthconst, sprite->srcW () / 2 ), sprite ( sprite )
{
	InputManager::instance ()->connect (
		InputManager::MOUSEDOWN_RIGHT,
		this,
		&FollowerObject::handleMouseDownRight
	);
}

FollowerObject::~FollowerObject ()
{
	InputManager::instance ()->disconnect ( this );
}

void FollowerObject::update ()
{
	if ( v.length () > 0 )
	{
		R2Vector tmp;
		Scalar dt = ( (Scalar) SDLBase::dt () ) / 1000;
		
		tmp.set ( 0, r.x ( 0 ) + v.x ( 0 ) * dt );
		tmp.set ( 1, r.x ( 1 ) + v.x ( 1 ) * dt );
		
		try {
			if ( ( dest - tmp ).scalarProj ( v ) > 0 )
			{
				r.add ( 0, v.x ( 0 ) * dt );
				r.add ( 1, v.x ( 1 ) * dt );
			}
			else
			{
				r = dest;
				v.annul ();
			}
		} catch (lalge::ScalarProjNotDefined& e) {
			r = dest;
			v.annul ();
		}
	}
	else if ( !path.empty () )
	{
		dest = path.front ();
		path.pop ();
		
		try {
			v = range ( dest ).unitvec () * VELOCITY;
		} catch (DirectionNotDefined& e) {
			v.annul ();
		}
		update ();
	}
}

void FollowerObject::render ()
{
	if ( sprite )
	{
		sprite->render (
			r.x ( 0 ) - sprite->srcrect ().w / 2 - Camera::r.x ( 0 ) * depthconst,
			r.x ( 1 ) - sprite->srcrect ().h / 2 - Camera::r.x ( 1 ) * depthconst
		);
	}
}

GameObject* FollowerObject::clone () const
{
	return new FollowerObject ( *this );
}

bool FollowerObject::mouseInside () const
{
	SDL_Rect rect;
	
	rect.x = r.x ( 0 ) - sprite->srcrect ().w / 2 - Camera::r.x ( 0 ) * depthconst;
	rect.y = r.x ( 1 ) - sprite->srcrect ().h / 2 - Camera::r.x ( 1 ) * depthconst;
	rect.w = sprite->srcrect ().w;
	rect.h = sprite->srcrect ().h;
	
	return InputManager::instance ()->mouseInside ( rect );
}

void FollowerObject::handleMouseDownRight ()
{
	path.push ( r2vec (
		InputManager::instance ()->mouseDownX () + Camera::r.x ( 0 ) * depthconst,
		InputManager::instance ()->mouseDownY () + Camera::r.x ( 1 ) * depthconst
	) );
}

void FollowerObject::renderLines ()
{
	if ( ( v.length () ) || ( !path.empty () ) )
	{
		SDLBase::drawLine (
			dest.x ( 0 ) - Camera::r.x ( 0 ) * depthconst,
			dest.x ( 1 ) - Camera::r.x ( 1 ) * depthconst,
			r.x ( 0 ) - Camera::r.x ( 0 ) * depthconst,
			r.x ( 1 ) - Camera::r.x ( 1 ) * depthconst,
			0xFFFFFF,
			30
		);
		
		queue< R2Vector, list< R2Vector > > tmp;
		R2Vector vtmp = dest;
		
		while ( !path.empty () )
		{
			SDLBase::drawLine (
				path.front ().x ( 0 ) - Camera::r.x ( 0 ) * depthconst,
				path.front ().x ( 1 ) - Camera::r.x ( 1 ) * depthconst,
				vtmp.x ( 0 ) - Camera::r.x ( 0 ) * depthconst,
				vtmp.x ( 1 ) - Camera::r.x ( 1 ) * depthconst,
				0xFFFFFF,
				30
			);
			
			vtmp = path.front ();
			tmp.push ( vtmp );
			path.pop ();
		}
		
		path = tmp;
	}
}

void FollowerObject::setSprite (Sprite* sprite)
{
	this->sprite = sprite;
	
	setRadius ( sprite->srcrect ().w / 2 );
}
