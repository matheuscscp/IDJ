/// @ingroup MOD_GAMEMANAGER
/// @file GameManager.cpp
/// @brief Implementations of all methods of the GameManager class
/// @author Matheus Pimenta

#include "../include/mod/linearalgebra.hpp"

#include "../include/GameManager.hpp"
#include "../include/GMDefaultConf.hpp"

#include "../include/InputManager.hpp"
#include "../include/Camera.hpp"

using namespace std;
using namespace lalge;

GameManager::GameManager (const MainParam& args) : args ( args ), quit ( false )
{
	InputManager::instance ()->connect (
		InputManager::QUIT,
		this,
		&GameManager::handleQuit
	);
	
	loadSDLConf ();
	loadSpriteConf ();
	
	SDLBase::initSDL (
		SDL_conf.getInt ( "w" ),
		SDL_conf.getInt ( "h" ),
		SDL_conf.getInt ( "bpp" ),
		SDL_conf.getStr ( "title" ).c_str (),
		SDL_conf.getStr ( "icon" ).c_str (),
		SDL_conf.getInt ( "fps" )
	);
	loadSprite ();
	
	InputManager::instance ();
	Camera::instance ();
	
	srand ( time ( NULL ) );
	
	tileset = new TileSet ( "./bin/img/tileset/Tileset.png", 75, 75 );
	tilemap = new TileMap ( tileset, "./map/tilemap.txt" );
	
	earth = new Earth (
		r2vec ( 400, 300 ), tilemap->layers () + 1, &spr_earth, 1
	);
	moon = new Moon (
		R2Vector (), tilemap->layers () + 1, &spr_moon, 1, earth
	);
	
	InputManager::instance ()->connect (
		InputManager::MOUSEDOWN_LEFT,
		this,
		&GameManager::handleMouseDownLeft
	);
}

GameManager::~GameManager ()
{
	InputManager::instance ()->disconnect ( this );
	
	while ( planets.size () )
	{
		delete planets.back ();
		planets.pop_back ();
	}
	
	delete tileset;
	delete tilemap;
	
	delete earth;
	delete moon;
	
	InputManager::close ();
	Camera::close ();
	
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
		
		try {
			SDL_conf.insertInt ( "fps", tmp.getInt ( "fps" ) );
		} catch (Configuration::VarNotFound& e) {
			SDL_conf.insertInt ( "fps", SDL_FPS );
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
		
		try {
			spr_conf.insertStr ( "redplanet", tmp.getStr ( "redplanet" ) );
		} catch (Configuration::VarNotFound& e) {
			spr_conf.insertStr ( "redplanet", SPR_REDPLANET );
		}
		
		try {
			spr_conf.insertStr ( "earth", tmp.getStr ( "earth" ) );
		} catch (Configuration::VarNotFound& e) {
			spr_conf.insertStr ( "earth", SPR_EARTH );
		}
		
		try {
			spr_conf.insertStr ( "moon", tmp.getStr ( "moon" ) );
		} catch (Configuration::VarNotFound& e) {
			spr_conf.insertStr ( "moon", SPR_MOON );
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
	SDL_conf.insertInt ( "fps",	SDL_FPS );
}

void GameManager::setDefaultSpriteConf ()
{
	spr_conf.insertStr ( "bg", SPR_BG );
	spr_conf.insertStr ( "redplanet", SPR_REDPLANET );
	spr_conf.insertStr ( "earth", SPR_EARTH );
	spr_conf.insertStr ( "moon", SPR_MOON );
}

void GameManager::loadSprite ()
{
	spr_bg.load		( spr_conf.getStr ( "bg" ) );
	spr_redplanet.load	( spr_conf.getStr ( "redplanet" ) );
	spr_earth.load		( spr_conf.getStr ( "earth" ) );
	spr_moon.load		( spr_conf.getStr ( "moon" ) );
}

void GameManager::run ()
{
	while ( !quit )
	{
		SDLBase::delayFrame ();
		
		input ();
		
		update ();
		
		render ();
		
		SDLBase::updateScreen ();
	}
}

void GameManager::input ()
{
	InputManager::instance ()->update ();
	
	addPlanet ();
	
	showFPS ();
	updateFPS ();
}

void GameManager::update ()
{
	Camera::instance ()->update ();
	
	earth->update ();
	moon->update ();
	
	checkPlanets ();
}

void GameManager::render ()
{
	spr_bg.render ( 0, 0 );
	
	for ( int k = 0; k < tilemap->layers (); ++k )
	{
		tilemap->renderLayer (
			k,
			Camera::instance ()->r.x ( 0 ) * ( k + 1 ),
			Camera::instance ()->r.x ( 1 ) * ( k + 1 )
		);
	}
	
	for (
		list< Planet* >::iterator it = planets.begin ();
		it != planets.end ();
		++it
	)
	{	
		(*it)->render ();
	}
	
	earth->render ();
	moon->render ();
}

void GameManager::handleQuit ()
{
	quit = true;
}

void GameManager::showFPS ()
{
	// debugging tool to show FPS
	if (	( args.find ( "-fps" ) ) &&
		(	( InputManager::instance ()->keyPressed ( 'f' ) ) ||
			( InputManager::instance ()->keyPressed ( 'F' ) )	)	)
		printf ( "FPS: %.1f\n", SDLBase::realFPS () );
}

void GameManager::addPlanet ()
{
	if ( InputManager::instance ()->keyPressed ( SDLK_SPACE ) )
	{
		int x = rand () % ( SDLBase::screen ()->w - spr_redplanet.w () ) + spr_redplanet.w () / 2;
		x += Camera::instance ()->r.x ( 0 ) * ( tilemap->layers () + 1 );
		int y = rand () % ( SDLBase::screen ()->h - spr_redplanet.h () ) + spr_redplanet.h () / 2;
		y += Camera::instance ()->r.x ( 1 ) * ( tilemap->layers () + 1 );
		
		planets.push_back ( new RedPlanet (
			r2vec ( x, y ),
			tilemap->layers () + 1,
			&spr_redplanet,
			rand () % 10 + 10
		) );
	}
}

void GameManager::handleMouseDownLeft ()
{
	bool found = false;
	
	if ( ( moon->mouseInside () ) || ( earth->mouseInside () ) )
		found = true;
	
	for (
		list< Planet* >::reverse_iterator it = planets.rbegin ();
		( it != planets.rend () ) && ( !found );
		++it
	)
	{
		if ( (*it)->mouseInside () )
		{
			found = true;
			(*it)->update ();
		}
	}
}

void GameManager::checkPlanets ()
{
	list< Planet* >::iterator it = planets.begin ();
	list< Planet* >::iterator tmp;
	
	while ( it != planets.end () )
	{
		if ( (*it)->hp () > 0 )
			++it;
		else
		{
			delete ( *it );
			it = planets.erase ( it );
		}
	}
}

void GameManager::updateFPS ()
{
	if (	( args.find ( "-fps" ) ) &&
		(	( InputManager::instance ()->keyPressed ( 'r' ) ) ||
			( InputManager::instance ()->keyPressed ( 'R' ) )	)	)
		SDLBase::fps--;
	
	if (	( args.find ( "-fps" ) ) &&
		(	( InputManager::instance ()->keyPressed ( 't' ) ) ||
			( InputManager::instance ()->keyPressed ( 'T' ) )	)	)
		SDLBase::fps++;
}
