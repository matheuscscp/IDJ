#include "AccObject.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"

#define ACCELERATION	500
#define OMEGA		180
#define AIR_RESISTANCE	0.3
#define STRONG_FRICTION	2.0

using namespace lalge;

AccObject::AccObject (
	const R2Vector& r,
	const Scalar& depthconst,
	Animation* animation,
	Animation* turn,
	int hp
) :
Circle ( r, depthconst, animation->rectW () / 2 ),
animation ( animation ), turn ( turn ), switch_time ( 0 ), hp ( hp ),
omega ( 0 ), acceleration ( 0 )
{
	InputManager::instance ()->connect (
		InputManager::KEYDOWN,
		this,
		&AccObject::handleKeyDown
	);
	InputManager::instance ()->connect (
		InputManager::KEYUP,
		this,
		&AccObject::handleKeyUp
	);
}

AccObject::~AccObject ()
{
	InputManager::instance ()->disconnect ( this );
}

void AccObject::update ()
{
	Scalar dt = ( (Scalar) SDLBase::dt () ) / 1000;
	
	animation->update ();
	animation->rotozoom ( animation->angle () + omega * dt );
	
	try {
		a = rotate (
			- ( animation->angle () + omega * dt ),
			r2vec ( 0, acceleration )
		);
		a += ( -v * ( omega ? STRONG_FRICTION : AIR_RESISTANCE ) );
	} catch (RotationNotDefined& e) {
		a = ( -v * ( omega ? STRONG_FRICTION : AIR_RESISTANCE ) );
	}
	r += ( ( v * dt ) + ( a * ( dt * dt / 2 ) ) );
	v += ( a * dt );
	
	Camera::r = (
		r -
		( r2vec ( SDLBase::screen ()->w, SDLBase::screen ()->h ) / 2 )
	) / depthconst;
}

void AccObject::render ()
{
	if ( ( omega ) || ( SDL_GetTicks () < switch_time ) )
	{
		if ( SDL_GetTicks () >= switch_time )
			turn->setFrame ( side ? 0 : 3 );
		else
			turn->setFrame ( side ? 1 : 2 );
		turn->rotozoom ( animation->angle () );
		turn->render (
			r.x ( 0 ) - animation->rectW () / 2 - Camera::r.x ( 0 ) * depthconst,
			r.x ( 1 ) - animation->rectH () / 2 - Camera::r.x ( 1 ) * depthconst
		);
	}
	else
	{
		animation->render (
			r.x ( 0 ) - animation->rectW () / 2 - Camera::r.x ( 0 ) * depthconst,
			r.x ( 1 ) - animation->rectH () / 2 - Camera::r.x ( 1 ) * depthconst
		);
	}
}

GameObject* AccObject::clone () const
{
	return new AccObject ( *this );
}

void AccObject::setAnimation (Animation* animation)
{
	this->animation = animation;
	
	setRadius ( animation->rectW () / 2 );
}

void AccObject::handleKeyDown ()
{
	switch ( ( (InputManager::Event*) InputManager::instance ()->event () )->event->key.keysym.sym )
	{
	case 'w':
		acceleration -= ACCELERATION;
		animation->setFrameSize ( 50 );
		break;
		
	case 'a':
		setSide ( OMEGA );
		break;
		
	case 'd':
		setSide ( -OMEGA );
		break;
		
	default:
		break;
	}
}

void AccObject::handleKeyUp ()
{
	switch ( ( (InputManager::Event*) InputManager::instance ()->event () )->event->key.keysym.sym )
	{
	case 'w':
		acceleration += ACCELERATION;
		animation->setFrameSize ( 2000 );
		break;
		
	case 'a':
		setSide ( -OMEGA );
		break;
		
	case 'd':
		setSide ( OMEGA );
		break;
		
	default:
		break;
	}
}

void AccObject::setSide (Scalar omega_value)
{
	if ( omega )
	{
		side = ( omega > 0 );
		omega += omega_value;
	}
	else
	{
		omega += omega_value;
		side = ( omega > 0 );
	}
	switch_time = SDL_GetTicks () + 250;
}
