#include "../include/mod/configfile.hpp"

#include "../include/Camera.hpp"

#include "../include/InputManager.hpp"
#include "../include/SDLBase.hpp"

#define CONF	"./conf/camera.conf"
#define SCROLL	200.0f

using namespace lalge;

Camera* Camera::instance_ = 0;

Camera::Camera ()
{
	Configuration tmp;
	
	tmp.readTxt ( CONF );
	
	try {
		scroll = tmp.getReal ( "scroll" );
	}
	catch (Configuration::VarNotFound& e)
	{
		scroll = SCROLL;
	}
	
	InputManager::instance ()->connect (
		InputManager::KEYDOWN,
		this,
		&Camera::handleKeyDown
	);
	InputManager::instance ()->connect (
		InputManager::KEYUP,
		this,
		&Camera::handleKeyUp
	);
}

Camera::~Camera ()
{
	InputManager::instance ()->disconnect ( this );
}

Camera* Camera::instance ()
{
	if ( !instance_ )
		instance_ = new Camera ();
	
	return instance_;
}

void Camera::close ()
{
	if ( instance_ )
	{
		delete instance_;
		instance_ = 0;
	}
}

void Camera::update ()
{
	Scalar tmp = ( (Scalar) SDLBase::dt () ) / 1000;
	
	r.add ( 0, v.x ( 0 ) * tmp );
	r.add ( 1, v.x ( 1 ) * tmp );
}

void Camera::render ()
{
}

GameObject* Camera::clone () const
{
	return 0;
}

void Camera::handleKeyDown ()
{
	switch ( ( (InputManager::Event*) InputManager::instance ()->event () )->event->key.keysym.sym )
	{
	case SDLK_UP:
		v.add ( 1, -scroll );
		break;
		
	case SDLK_DOWN:
		v.add ( 1, scroll );
		break;
		
	case SDLK_LEFT:
		v.add ( 0, -scroll );
		break;
		
	case SDLK_RIGHT:
		v.add ( 0, scroll );
		break;
		
	default:
		break;
	}
}

void Camera::handleKeyUp ()
{
	switch ( ( (InputManager::Event*) InputManager::instance ()->event () )->event->key.keysym.sym )
	{
	case SDLK_UP:
		v.add ( 1, scroll );
		break;
		
	case SDLK_DOWN:
		v.add ( 1, -scroll );
		break;
		
	case SDLK_LEFT:
		v.add ( 0, scroll );
		break;
		
	case SDLK_RIGHT:
		v.add ( 0, -scroll );
		break;
		
	default:
		break;
	}
}
