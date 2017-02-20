#include <string>

#include "configfile.hpp"
#include "simplestructures.hpp"

#include "StateManager.hpp"

#include "SDLBase.hpp"
#include "InputManager.hpp"
#include "GameStates.hpp"

using std::string;

StateManager::StateManager(const MainArgs& args) : args(args), quit(false)
{
	initThirdParty();
	SDLBase::initSDL( args.get( "--path" ) + "conf/SDL.conf" );
	initState();
	InputManager::instance();
}

StateManager::~StateManager()
{
	InputManager::close();
	closeState();
	SDLBase::closeSDL();
	closeThirdParty();
}

void StateManager::initThirdParty()
{
	srand( time( NULL ) );
}

void StateManager::initState()
{
	state = new StateSplash();
	state->load( &args );
}

void StateManager::closeState()
{
	state->unload();
	delete state;
}

void StateManager::closeThirdParty()
{
}

void StateManager::run()
{
	while( !quit )
	{
		SDLBase::delayFrame ();
		
		input();
		
		update();
		
		render();
	}
}

void StateManager::input()
{
	InputManager::instance()->update();
	
	int newstate = state->input();
	if( newstate == STATEQUIT )
		quit = true;
	else if( newstate )
		changeState( newstate );
}

void StateManager::update()
{
	int newstate = state->update();
	if( newstate == STATEQUIT )
		quit = true;
	else if( newstate )
		changeState( newstate );
}

void StateManager::render()
{
	if( !quit )
	{
		state->render();
		SDLBase::updateScreen ();
	}
}

template <typename newstate_t>
void StateManager::changeState_()
{
	StateArgs* st_args = state->unload();
	delete state;
	state = new newstate_t();
	state->load( &args, st_args );
	if( st_args )
		delete st_args;
}

void StateManager::changeState(int newstate)
{
	switch( newstate )
	{
		case STATESPLASH:	changeState_< StateSplash >();		break;
		case STATEGAME:		changeState_< StateGame >();		break;
		case STATEWINLOSE:	changeState_< StateWinLose >();	break;
		
		default:
			break;
	}
}
