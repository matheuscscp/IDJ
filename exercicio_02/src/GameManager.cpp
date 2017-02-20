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
	
	quit = false;
	
	srand ( time ( NULL ) );
	
	tileset = new TileSet (
		"./bin/img/tileset/Tileset.png", TILE_SIZE, TILE_SIZE
	);
	
	tilemap = new TileMap ( "./map/tilemap.txt", tileset );
	
	key_up = false;
	key_down = false;
	key_left = false;
	key_right = false;
}

GameManager::~GameManager ()
{
	while ( planets.size () )
	{
		delete ( *( planets.end () - 1 ) );
		planets.pop_back ();
	}
	
	delete tileset;
	delete tilemap;
	
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
		
		try {
			spr_conf.insertStr ( "redplanet", tmp.getStr ( "redplanet" ) );
		} catch (Configuration::VarNotFound& e) {
			spr_conf.insertStr ( "redplanet", SPR_BG );
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
	spr_conf.insertStr ( "bg", SPR_BG );
	spr_conf.insertStr ( "redplanet", SPR_REDPLANET );
}

void GameManager::loadSprite ()
{
	spr_bg.load ( spr_conf.getStr ( "bg" ) );
	spr_redplanet.load ( spr_conf.getStr ( "redplanet" ) );
}

void GameManager::run ()
{
	while ( !quit )
	{
		input ();
		
		update ();
		
		render ();
		
		SDLBase::updateScreen ();
		
		SDL_Delay ( 10 );
	}
}

void GameManager::input ()
{
	processEvents ();
}

void GameManager::update ()
{
	checkPlanets ();
	
	updateCamera ();
}

void GameManager::render ()
{
	spr_bg.render ( 0, 0 );
	
	for ( int k = 0; k < tilemap->layers (); ++k )
	{
		tilemap->renderLayer (
			k,
			camera.x * ( k + 1 ),
			camera.y * ( k + 1 )
		);
	}
	
	for (
		vector< Planet* >::iterator it = planets.begin ();
		it != planets.end ();
		++it
	)
	{	
		(*it)->render (
			camera.x * ( tilemap->layers () + 1 ),
			camera.y * ( tilemap->layers () + 1 )
		);
	}
}

void GameManager::processEvents ()
{
	// procura um evento
	SDL_Event event;
	while ( SDL_PollEvent( &event ) )
	{
		// um evento foi achado
		if ( event.type == SDL_QUIT )
			quit = true;
		else if ( event.type == SDL_MOUSEBUTTONDOWN )
		{
			// obtém o estado do mouse
			int mouseX, mouseY;
			SDL_GetMouseState( &mouseX, &mouseY );
			
			// percorre o array de monstros de forma invertida
			// até achar o monstro que o usuário clicou.
			// percorremos de forma invertida porque os últimos
			// monstros são desenhados no topo, e estes serão
			// atingidos primeiro
			for ( vector<Planet*>::iterator it = planets.end() - 1; it != planets.begin() - 1; --it )
			{
				Planet *planet = *it;
				
				if (	mouseX - ( camera.x * ( tilemap->layers () + 1 ) ) >= planet->position.x &&
					mouseX - ( camera.x * ( tilemap->layers () + 1 ) ) < planet->position.x + planet->sprite->srcrect().w &&
					mouseY - ( camera.y * ( tilemap->layers () + 1 ) ) >= planet->position.y &&
					mouseY - ( camera.y * ( tilemap->layers () + 1 ) ) < planet->position.y + planet->sprite->srcrect().h	)
				{
					// acerta o planeta, dando dano aleatório entre 10 e 19
					
					planet->hitpoints -= ( rand() % 10 + 10 );
					
					// acertamos apenas um monstro
					// portanto saímos do break
					break;
				}
			}
		}
		else if( event.type == SDL_KEYDOWN )
		{
			// uma tecla foi pressionada
			if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				// se a tecla ESC foi pressionada, sair do programa
				quit = true;
			}
			else if( event.key.keysym.sym == SDLK_UP )
				key_up = true;
			else if( event.key.keysym.sym == SDLK_DOWN )
				key_down = true;
			else if( event.key.keysym.sym == SDLK_LEFT )
				key_left = true;
			else if( event.key.keysym.sym == SDLK_RIGHT )
				key_right = true;
			else if( event.key.keysym.sym == SDLK_SPACE )
			{
				// caso qualquer outra tecla seja pressionada,
				// chamar a rotina de adicionar monstro
				addPlanet();
			}
		}
		else if( event.type == SDL_KEYUP )
		{
			// uma tecla foi solta
			
			// diminui as variaveis de velocidade da cÃ¢mera
			// para que elas voltam a ser zero caso uma tecla
			// tenha sido solta
			if( event.key.keysym.sym == SDLK_UP )
				key_up = false;
			else if( event.key.keysym.sym == SDLK_DOWN )
				key_down = false;
			else if( event.key.keysym.sym == SDLK_LEFT )
				key_left = false;
			else if( event.key.keysym.sym == SDLK_RIGHT )
				key_right = false;
		}
	}
	// fim do loop de eventos
}

void GameManager::addPlanet ()
{
	int x = rand () % ( SDL_WIDTH - spr_redplanet.w () );
	x -= camera.x * ( tilemap->layers () + 1 );
	int y = rand () % ( SDL_HEIGHT - spr_redplanet.h () );
	y -= camera.y * ( tilemap->layers () + 1 );
	
	planets.push_back (
		new Planet ( &spr_redplanet, rand () % 10 + 10, x, y )
	);
}

void GameManager::checkPlanets ()
{
	vector< Planet* >::iterator it = planets.begin ();
	
	while ( it != planets.end () )
	{
		if ( (*it)->hitpoints > 0 )
			++it;
		else
		{
			delete ( *it );
			
			// coloca o ultimo no lugar, se este deletado nao
			// for o ultimo, e da o pop no fim
			if ( it + 1 != planets.end () )
				*it = ( *( planets.end () - 1 ) );
			planets.pop_back ();
		}
	}
}

void GameManager::updateCamera ()
{
	if ( key_up )
		camera.y += SCROLL;
	if ( key_down )
		camera.y -= SCROLL;
	if ( key_left )
		camera.x += SCROLL;
	if ( key_right )
		camera.x -= SCROLL;
}
