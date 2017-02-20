/// @ingroup MOD_GAMEMANAGER
/// @file GameManager.cpp
/// @brief Implementations of all methods of the GameManager class
/// @author Matheus Pimenta

#include "../include/GameManager.hpp"

GameManager::GameManager ()
{
	loadSDLConf ();
	loadSpriteConf ();
	
	SDLBase::initSDL (	SDL_conf.getInt ( "w" ),
				SDL_conf.getInt ( "h" ),
				SDL_conf.getInt ( "bpp" ),
				SDL_conf.getStr ( "title" ).c_str (),
				SDL_conf.getStr ( "icon" ).c_str ()	);
	loadSprite ();
}

GameManager::~GameManager ()
{
	SDLBase::closeSDL ();
}

void GameManager::loadSDLConf ()
{
	try {
		Configuration tmp;
		tmp.readTxt ( SDL_CONF );
		
		try {
			SDL_conf.insertInt ( "w", tmp.getInt ( "w" ) );
		} catch (Configuration::VarNotFound& e) {
			SDL_conf.insertInt ( "w", SDL_WIDTH );
		}
		
		try {
			SDL_conf.insertInt ( "h", tmp.getInt ( "h" ) );
		} catch (Configuration::VarNotFound& e) {
			SDL_conf.insertInt ( "h", SDL_HEIGHT );
		}
		
		try {
			SDL_conf.insertInt ( "bpp", tmp.getInt ( "bpp" ) );
		} catch (Configuration::VarNotFound& e) {
			SDL_conf.insertInt ( "bpp", SDL_BPP );
		}
		
		try {
			SDL_conf.insertStr ( "title", tmp.getStr ( "title" ) );
		} catch (Configuration::VarNotFound& e) {
			SDL_conf.insertStr ( "title", SDL_TITLE );
		}
		
		try {
			SDL_conf.insertStr ( "icon", tmp.getStr ( "icon" ) );
		} catch (Configuration::VarNotFound& e) {
			SDL_conf.insertStr ( "icon", SDL_ICON );
		}
	}
	catch (Configuration::FileNotFound& e)
	{
		setDefaultSDLConf ();
	}
}

void GameManager::loadSpriteConf ()
{
	try {
		Configuration tmp;
		tmp.readTxt ( SPR_CONF );
		
		try {
			spr_conf.insertStr ( "bg", tmp.getStr ( "bg" ) );
		} catch (Configuration::VarNotFound& e) {
			spr_conf.insertStr ( "bg", SPR_BG );
		}
	}
	catch (Configuration::FileNotFound& e)
	{
		setDefaultSpriteConf ();
	}
}

void GameManager::setDefaultSDLConf ()
{
	SDL_conf.insertInt ( "w",	SDL_WIDTH );
	SDL_conf.insertInt ( "h",	SDL_HEIGHT );
	SDL_conf.insertInt ( "bpp",	SDL_BPP );
	SDL_conf.insertStr ( "title",	SDL_TITLE );
	SDL_conf.insertStr ( "icon",	SDL_ICON );
}

void GameManager::setDefaultSpriteConf ()
{
	spr_conf.insertStr ( "bg",	SPR_BG );
}

void GameManager::loadSprite ()
{
	spr_bg.load ( spr_conf.getStr ( "bg" ) );
}

void GameManager::run ()
{
	while ( !SDL_QuitRequested () )
	{
		input ();
		
		update ();
		
		render ();
		
		SDLBase::updateScreen ();
	}
}

void GameManager::input ()
{
}

void GameManager::update ()
{
}

void GameManager::render ()
{
	spr_bg.render ( 0, 0 );
}
