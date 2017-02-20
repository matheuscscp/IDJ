/// @ingroup MOD_GAMEMANAGER
/// @file GameManager.cpp
/// @brief Implementations of all methods of the GameManager class
/// @author Matheus Pimenta

#include "mod/linearalgebra.hpp"

#include "GameManager.hpp"
#include "GMDefaultConf.hpp"

#include "InputManager.hpp"
#include "Camera.hpp"

using namespace std;
using namespace lalge;

GameManager::GameManager (const MainParam& args) : args ( args ), quit ( false )
{
	InputManager::instance ()->connect (
		InputManager::QUIT,
		this,
		&GameManager::handleQuit
	);
	InputManager::instance ()->connect (
		InputManager::KEYDOWN,
		this,
		&GameManager::handleKeyDown
	);
	InputManager::instance ()->connect (
		InputManager::MOUSEDOWN_LEFT,
		this,
		&GameManager::addPlanet
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
	
	srand ( time ( NULL ) );
	
	ship_anim = new Animation ( "./bin/img/NaveSheet.png", 0, 2000, 1, 4 );
	ship_turn = new Animation ( "./bin/img/NaveTurnSheet.png", 0, 50, 1, 4 );
	boom = new Animation ( "./bin/img/BoomSheet.png", 0, 100, 1, 8 );
	
	tileset = new TileSet ( "./bin/img/tileset/Tileset.png", 75, 75 );
	tilemap = new TileMap ( tileset, "./map/tilemap.txt" );
	
	earth = new Earth (
		r2vec ( ( rand () % 1401 ) - 300, ( rand () % 1201 ) - 300 ),
		tilemap->layers () + 1,
		&spr_earth,
		10
	);
	moon = new Moon (
		R2Vector (), tilemap->layers () + 1, &spr_moon, earth
	);
	
	R2Vector tmp = r2vec ( rand () % 800, rand () % 600 );
	ufo = new FollowerObject (
		tmp, tilemap->layers () + 1, &spr_ufo
	);
	ship = new AccObject (
		tmp + rotate ( rand () % 360, r2vec ( 800, 0 ) ),
		tilemap->layers () + 1,
		ship_anim,
		ship_turn,
		20
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
	
	delete ship_anim;
	delete ship_turn;
	delete boom;
	
	delete tileset;
	delete tilemap;
	
	delete earth;
	delete moon;
	
	if ( ufo )
		delete ufo;
	
	if ( ship )
		delete ship;
	
	InputManager::close ();
	
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
		
		try {
			spr_conf.insertStr ( "ufo", tmp.getStr ( "ufo" ) );
		} catch (Configuration::VarNotFound& e) {
			spr_conf.insertStr ( "ufo", SPR_UFO );
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
	spr_conf.insertStr ( "ufo", SPR_UFO );
}

void GameManager::loadSprite ()
{
	spr_bg.load		( spr_conf.getStr ( "bg" ) );
	spr_redplanet.load	( spr_conf.getStr ( "redplanet" ) );
	spr_earth.load		( spr_conf.getStr ( "earth" ) );
	spr_moon.load		( spr_conf.getStr ( "moon" ) );
	spr_ufo.load		( spr_conf.getStr ( "ufo" ) );
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
	
	showFPS ();
}

void GameManager::update ()
{
	earth->update ();
	moon->update ();
	
	if ( ufo )
		ufo->update ();
	
	if ( ( ufo ) && ( ship ) )
		ship->update ();
	
	boom->update ();
	
	checkCollision ();
}

void GameManager::render ()
{
	spr_bg.render ( 0, 0 );
	
	for ( int k = 0; k < tilemap->layers (); ++k )
	{
		tilemap->renderLayer (
			k,
			Camera::r.x ( 0 ) * ( k + 1 ),
			Camera::r.x ( 1 ) * ( k + 1 )
		);
	}
	
	earth->render ();
	moon->render ();
	
	if ( ship )
		ship->render ();
	
	for (
		list< Planet* >::iterator it = planets.begin ();
		it != planets.end ();
		++it
	)
	{	
		(*it)->render ();
	}
	
	if ( boom_r.length () )
	{
		boom->render (
			boom_r.x ( 0 ) - boom->rectW () / 2 - Camera::r.x ( 0 ) * ( tilemap->layers () + 1 ),
			boom_r.x ( 1 ) - boom->rectH () / 2 - Camera::r.x ( 1 ) * ( tilemap->layers () + 1 )
     		);
	}
	
	if ( ufo )
	{
		ufo->renderLines ();
		ufo->render ();
	}
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
	if ( ( ufo ) && ( ship ) )
	{
		int x = rand () % ( SDLBase::screen ()->w - spr_redplanet.srcW () ) + spr_redplanet.srcW () / 2;
		x += Camera::r.x ( 0 ) * ( tilemap->layers () + 1 );
		int y = rand () % ( SDLBase::screen ()->h - spr_redplanet.srcH () ) + spr_redplanet.srcH () / 2;
		y += Camera::r.x ( 1 ) * ( tilemap->layers () + 1 );
		
		planets.push_back ( new RedPlanet (
			r2vec ( x, y ),
			tilemap->layers () + 1,
			&spr_redplanet
		) );
	}
}

void GameManager::handleKeyDown ()
{
	switch ( ( (InputManager::Event*) InputManager::instance ()->event () )->event->key.keysym.sym )
	{
	case SDLK_ESCAPE:
		quit = true;
		break;
		
	default:
		break;
	}
}

void GameManager::checkCollision ()
{
	if ( ( ufo ) && ( ship ) )
	{
		list< Planet* >::iterator it = planets.begin ();
		
		while ( it != planets.end () )
		{
			if ( !(*it)->colliding ( *ship ) )
				++it;
			else
			{
				boom_r = (*it)->r;
				ship->hp--;
				
				delete ( *it );
				it = planets.erase ( it );
			}
		}
		
		if ( ship->colliding ( *ufo ) )
		{
			boom_r = ship->r;
			ship->hp = 0;
		}
		
		if ( ship->colliding ( *earth ) )
		{
			boom_r = ufo->r;
			
			delete ufo;
			ufo = NULL;
		}
		
		if ( ship->hp <= 0 )
		{
			boom_r = ship->r;
			
			delete ship;
			ship = NULL;
		}
	}
}
