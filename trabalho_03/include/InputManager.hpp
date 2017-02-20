#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

#include "SDL.h"

#include "mod/observer.hpp"

#define INPUTMANAGER_KEYS	323
#define INPUTMANAGER_BUTTONS	6

class InputManager
{
SUBJECT
public:
	enum event_id
	{
		KEYDOWN = 0,		// 1
		KEYUP,			// 2
		MOUSEDOWN_LEFT,		// 3
		MOUSEDOWN_MIDDLE,	// 4
		MOUSEDOWN_RIGHT,	// 5
		MOUSEUP_LEFT,		// 6
		MOUSEUP_MIDDLE,		// 7
		MOUSEUP_RIGHT,		// 8
		MOUSE_WHEELUP,		// 9
		MOUSE_WHEELDOWN,	// 10
		QUIT			// 11
	};
	
	// TODO herdar Event
	struct Event : public observer::Event
	{
		SDL_Event* event;
		
		Event (event_id event_type, SDL_Event* event);
		
		virtual ~Event ();
	};
private:
	static InputManager* instance_;
	
	SDL_Event event_;
	
	bool key_pressed[INPUTMANAGER_KEYS];
	int mouse_x;
	int mouse_y;
	bool mouse_pressed[INPUTMANAGER_BUTTONS];
	int mousedown_x;
	int mousedown_y;
	
	InputManager ();
	~InputManager ();
public:
	static InputManager* instance ();
	static void close ();
	
	void update ();
	
	bool keyPressed (int i) const;
	int mouseX () const;
	int mouseY () const;
	bool mousePressed (int i) const;
	bool mouseInside (const SDL_Rect& rect) const;
	int mouseDownX () const;
	int mouseDownY () const;
	bool mouseDownInside (const SDL_Rect& rect) const;
};

#endif
