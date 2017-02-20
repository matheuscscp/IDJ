#include <climits>

#include "../include/InputManager.hpp"

#define N_EVENTS	9
#define broadcast(X)	subject.broadcast ( Event ( (X), &event_ ) )

InputManager* InputManager::instance_ = 0;

InputManager::Event::Event (event_id event_type, SDL_Event* event) :
observer::Event ( event_type ), event ( event )
{
}

InputManager::Event::~Event ()
{
}

InputManager::InputManager () :
mouse_x ( INT_MAX ), mouse_y ( INT_MAX )
{
	subject.init ( N_EVENTS );
	
	for ( int i = 0; i < INPUTMANAGER_KEYS; i++ )
		key_pressed[i] = false;
	
	for ( int i = 0; i < INPUTMANAGER_BUTTONS; i++ )
		mouse_pressed[i] = false;
}

InputManager::~InputManager ()
{
}

InputManager* InputManager::instance ()
{
	if ( !instance_ )
		instance_ = new InputManager ();
	
	return instance_;
}

void InputManager::close ()
{
	if ( instance_ )
	{
		delete instance_;
		instance_ = 0;
	}
}

void InputManager::update ()
{
	while ( SDL_PollEvent ( &event_ ) )
	{
		switch ( event_.type )
		{
		case SDL_KEYDOWN:
			key_pressed[ event_.key.keysym.sym ] = true;
			broadcast ( KEYDOWN );
			break;
			
		case SDL_KEYUP:
			key_pressed[ event_.key.keysym.sym ] = false;
			broadcast ( KEYUP );
			break;
			
		case SDL_MOUSEMOTION:
			mouse_x = event_.motion.x;
			mouse_y = event_.motion.y;
			break;
			
		case SDL_MOUSEBUTTONDOWN:
			mouse_pressed[ event_.button.button ] = true;
			
			mousedown_x = event_.button.x;
			mousedown_y = event_.button.y;
			
			switch ( event_.button.button )
			{
			case SDL_BUTTON_LEFT:
				broadcast ( MOUSEDOWN_LEFT );
				break;
				
			case SDL_BUTTON_RIGHT:
				broadcast ( MOUSEDOWN_RIGHT );
				break;
				
			case SDL_BUTTON_WHEELUP:
				broadcast ( MOUSE_WHEELUP );
				break;
				
			case SDL_BUTTON_WHEELDOWN:
				broadcast ( MOUSE_WHEELDOWN );
				break;
			}
			
			break;
			
		case SDL_MOUSEBUTTONUP:
			mouse_pressed[ event_.button.button ] = false;
			
			switch ( event_.button.button )
			{
			case SDL_BUTTON_LEFT:
				broadcast ( MOUSEUP_LEFT );
				break;
				
			case SDL_BUTTON_RIGHT:
				broadcast ( MOUSEUP_RIGHT );
				break;
			}
			
			break;
			
		case SDL_QUIT:
			broadcast ( QUIT );
			break;
		}
	}
}

bool InputManager::keyPressed (int i) const
{
	return key_pressed[i];
}

int InputManager::mouseX () const
{
	return mouse_x;
}

int InputManager::mouseY () const
{
	return mouse_y;
}

bool InputManager::mousePressed (int i) const
{
	return mouse_pressed[i];
}

bool InputManager::mouseInside (const SDL_Rect& rect) const
{
	return ( ( ( mouse_x >= rect.x ) && ( mouse_x < rect.x + rect.w ) ) &&
		 ( ( mouse_y >= rect.y ) && ( mouse_y < rect.y + rect.h ) ) );
}

int InputManager::mouseDownX () const
{
	return mousedown_x;
}

int InputManager::mouseDownY () const
{
	return mousedown_y;
}

bool InputManager::mouseDownInside (const SDL_Rect& rect) const
{
	return ( ( ( mousedown_x >= rect.x ) && ( mousedown_x < rect.x + rect.w ) ) &&
		 ( ( mousedown_y >= rect.y ) && ( mousedown_y < rect.y + rect.h ) ) );
}
