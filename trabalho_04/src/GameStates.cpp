#include "GameStates.hpp"

#include "InputManager.hpp"
#include "Text.hpp"
#include "Camera.hpp"

using namespace lalge;

using std::list;

// ==========================================================================
// StateSplash
// ==========================================================================

void StateSplash::load(MainArgs* args, StateArgs* st_args)
{
	if( st_args ){}
	
	this->args = args;
	
	InputManager::instance()->connect(
		InputManager::QUIT,
		this,
		&StateSplash::handleQuit
	);
	InputManager::instance()->connect(
		InputManager::KEYDOWN,
		this,
		&StateSplash::handleKeyDown
	);
	
	bgm = new Audio( "./sfx/stateLose.mp3" );
	bgm->play();
	
	renderMenu();
}

StateArgs* StateSplash::unload()
{
	InputManager::instance()->disconnect( this );
	
	delete bgm;
	
	return 0;
}

int StateSplash::input()
{
	// don't change this
	int tmp = newstate;
	newstate = 0;
	return tmp;
}

int StateSplash::update()
{
	// don't change this
	int tmp = newstate;
	newstate = 0;
	return tmp;
}

void StateSplash::render()
{
}

void StateSplash::renderMenu()
{
	Sprite* bg = new Sprite( "./img/stateMenu.jpg" );
	
	Text* text = new Text(
		"./ttf/DiabloLight.ttf",
		"Hyperion Quest", 45, TTF_STYLE_BOLD,
		SDLBase::getColor( 255, 255, 255 ), Text::blended
	);
	bg->render();
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		SDLBase::screen()->h / 4 - text->h() / 2
	);
	
	text->setSize( 15 );
	
	text->setText( "Para jogar, tecle espaço" );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2
	);
	
	text->setText( "Hyperion: 'W', 'A', 'S', 'D'" );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2 + 30
	);
	
	text->setText(
		"UFO: botão esquerdo cria planetas, botão direito movimenta"
	);
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2 + 50
	);
	
	text->setText(
		"Para voltar ao menu, tecle 'M'"
	);
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2 + 80
	);
	
	text->setText( "Para sair, tecle ESC, 'Q', ou ALT + F4," );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		4 * SDLBase::screen()->h / 5 - text->h() / 2
	);
	
	text->setText( "ou clique no botão 'X' da janela" );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		4 * SDLBase::screen()->h / 5 - text->h() / 2 + 20
	);
	
	delete bg;
	delete text;
}

void StateSplash::handleQuit()
{
	newstate = STATEQUIT;
}

void StateSplash::handleKeyDown()
{
	switch( INPUTMANAGER_EVENT->key.keysym.sym )
	{
	case SDLK_ESCAPE:
		newstate = STATEQUIT;
		break;
		
	case SDLK_q:
		newstate = STATEQUIT;
		break;
		
	case SDLK_SPACE:
		newstate = STATEGAME;
		break;
		
	default:
		break;
	}
}

// ==========================================================================
// StateGame
// ==========================================================================

struct StateGameArgs : public StateArgs
{
	char winner;
	
	StateGameArgs(char winner) : winner(winner) {}
};

void StateGame::load(MainArgs* args, StateArgs* st_args)
{
	if( st_args ){}
	
	this->args = args;
	
	InputManager::instance()->connect(
		InputManager::QUIT,
		this,
		&StateGame::handleQuit
	);
	InputManager::instance()->connect(
		InputManager::KEYDOWN,
		this,
		&StateGame::handleKeyDown
	);
	InputManager::instance ()->connect (
		InputManager::MOUSEDOWN_LEFT,
		this,
		&StateGame::addPlanet
	);
	
	bgm = new Audio( "./sfx/stateGame.mp3" );
	sfx = new Audio( "./sfx/boom.wav" );
	bgm->play();
	
	spr_bg = new Sprite( "./img/bg.png" );
	spr_redplanet = new Sprite( "./img/redplanet.png" );
	spr_earth = new Sprite( "./img/earth.png" );
	spr_moon = new Sprite( "./img/moon.png" );
	spr_ufo = new Sprite( "./img/ufo.png" );
	
	anim_ship = new Animation ( "./img/NaveSheet.png", 0, 2000, 1, 4 );
	anim_shipturn = new Animation ( "./img/NaveTurnSheet.png", 0, 50, 1, 4 );
	anim_boom = new Animation ( "./img/BoomSheet.png", 0, 100, 1, 8 );
	
	tileset = new TileSet ( "./img/Tileset.png", 75, 75 );
	tilemap = new TileMap ( tileset, "./map/tilemap.txt" );
	
	earth = new Earth (
		r2vec ( ( rand () % 2001 ) - 600, ( rand () % 1801 ) - 300 ),
		tilemap->layers () + 1,
		spr_earth,
		10
	);
	moon = new Moon (
		R2Vector (), tilemap->layers () + 1, spr_moon, earth
	);
	
	R2Vector tmp;
	do {
		tmp = r2vec ( rand () % 1601 - 400, rand () % 1401 - 400 );
	} while ( earth->range ( tmp ).length () < 800 );
	ship = new AccObject (
		tmp,
		tilemap->layers () + 1,
		anim_ship,
		anim_shipturn,
		20
	);
	ufo = new FollowerObject (
		tmp + rotate ( rand () % 360, r2vec ( 800, 0 ) ),
		tilemap->layers () + 1,
		spr_ufo
	);
}

StateArgs* StateGame::unload()
{
	StateArgs* ret;
	
	InputManager::instance()->disconnect( this );
	
	delete bgm;
	delete sfx;
	
	delete spr_bg;
	delete spr_redplanet;
	delete spr_earth;
	delete spr_moon;
	delete spr_ufo;
	
	delete anim_ship;
	delete anim_shipturn;
	delete anim_boom;
	
	delete tileset;
	delete tilemap;
	
	while( planets.size () )
	{
		delete planets.back();
		planets.pop_back();
	}
	
	delete earth;
	delete moon;
	
	if( ufo )
	{
		ret = new StateGameArgs( 'u' );
		delete ufo;
	}
	if( ship )
	{
		ret = new StateGameArgs( 's' );
		delete ship;
	}
	
	return ret;
}

int StateGame::input()
{
	showFPS();
	
	// don't change this
	int tmp = newstate;
	newstate = 0;
	return tmp;
}

int StateGame::update()
{
	earth->update();
	moon->update();
	
	if( ufo )
		ufo->update();
	
	if( ( ufo ) && ( ship ) )
		ship->update();
	
	anim_boom->update();
	
	checkCollision();
	checkGameOver();
	
	// don't change this
	int tmp = newstate;
	newstate = 0;
	return tmp;
}

void StateGame::render()
{
	spr_bg->render( 0, 0 );
	
	for( int k = 0; k < tilemap->layers(); ++k )
	{
		tilemap->renderLayer(
			k,
			Camera::r.x( 0 ) * ( k + 1 ),
			Camera::r.x( 1 ) * ( k + 1 )
		);
	}
	
	earth->render();
	moon->render();
	
	if( ship )
		ship->render();
	
	for(
		list< Planet* >::iterator it = planets.begin();
		it != planets.end();
		++it
	)
	{	
		(*it)->render();
	}
	
	if( boom_r.length() )
	{
		anim_boom->render(
			boom_r.x( 0 ) - anim_boom->rectW() / 2 - Camera::r.x( 0 ) * ( tilemap->layers() + 1 ),
			boom_r.x( 1 ) - anim_boom->rectH() / 2 - Camera::r.x( 1 ) * ( tilemap->layers() + 1 )
     	);
	}
	
	if( ufo )
	{
		ufo->renderLines();
		ufo->render();
	}
}

void StateGame::handleQuit()
{
	newstate = STATEQUIT;
}

void StateGame::handleKeyDown()
{
	switch( INPUTMANAGER_EVENT->key.keysym.sym )
	{
	case SDLK_ESCAPE:
		newstate = STATEQUIT;
		break;
		
	case SDLK_q:
		newstate = STATEQUIT;
		break;
		
	case SDLK_m:
		if( gameover.unused() )
			newstate = STATESPLASH;
		break;
		
	default:
		break;
	}
}

void StateGame::addPlanet()
{
	if( ( ufo ) && ( ship ) && ( ( newplanet.time() >= 500 ) || ( newplanet.unused() ) ) )
	{
		int x = rand() % ( SDLBase::screen()->w - spr_redplanet->srcW() ) + spr_redplanet->srcW() / 2;
		x += Camera::r.x( 0 ) * ( tilemap->layers() + 1 );
		int y = rand() % ( SDLBase::screen()->h - spr_redplanet->srcH() ) + spr_redplanet->srcH() / 2;
		y += Camera::r.x( 1 ) * ( tilemap->layers() + 1 );
		
		planets.push_back( new RedPlanet(
			r2vec( x, y ),
			tilemap->layers() + 1,
			spr_redplanet
		) );
		
		newplanet.start();
	}
}

void StateGame::showFPS()
{
	// debugging tool to show FPS
	if(	( args->find( "-fps" ) ) &&
			( InputManager::instance()->keyPressed( SDLK_f ) )	)
		printf ( "FPS: %.1f\n", SDLBase::FPS() );
}

void StateGame::checkCollision()
{
	if( ( ufo ) && ( ship ) )
	{
		list< Planet* >::iterator it = planets.begin();
		
		while( it != planets.end() )
		{
			if( !(*it)->colliding( *ship ) )
				++it;
			else
			{
				sfx->play( 1 );
				
				boom_r = (*it)->r;
				ship->hp--;
				
				delete( *it );
				it = planets.erase( it );
			}
		}
		
		if( ship->colliding( *ufo ) )
		{
			sfx->play( 1 );
			
			boom_r = ship->r;
			ship->hp = 0;
		}
		
		if( ship->colliding( *earth ) )
		{
			sfx->play( 1 );
			
			boom_r = ufo->r;
			
			delete ufo;
			ufo = NULL;
			
			gameover.start();
		}
		
		if( ship->hp <= 0 )
		{
			boom_r = ship->r;
			
			delete ship;
			ship = NULL;
			
			gameover.start();
		}
	}
}

void StateGame::checkGameOver()
{
	if( gameover.time() >= 3000 )
		newstate = STATEWINLOSE;
}

// ==========================================================================
// StateWinLose
// ==========================================================================

void StateWinLose::load(MainArgs* args, StateArgs* st_args)
{
	this->args = args;
	
	InputManager::instance()->connect(
		InputManager::QUIT,
		this,
		&StateWinLose::handleQuit
	);
	InputManager::instance()->connect(
		InputManager::KEYDOWN,
		this,
		&StateWinLose::handleKeyDown
	);
	
	if( ( (StateGameArgs*) st_args )->winner == 'u' )
		bgm = new Audio( "./sfx/stateLose.mp3" );
	else
		bgm = new Audio( "./sfx/stateWin.mp3" );
	bgm->play();
	
	renderMenu( st_args );
}

StateArgs* StateWinLose::unload()
{
	InputManager::instance()->disconnect( this );
	
	delete bgm;
	
	return 0;
}

int StateWinLose::input()
{
	// don't change this
	int tmp = newstate;
	newstate = 0;
	return tmp;
}

int StateWinLose::update()
{
	// don't change this
	int tmp = newstate;
	newstate = 0;
	return tmp;
}

void StateWinLose::render()
{
}

void StateWinLose::renderMenu(StateArgs* st_args)
{
	Sprite* bg;
	Text* text;
	
	if( ( (StateGameArgs*) st_args )->winner == 'u' )
	{
		bg = new Sprite( "./img/stateLose.jpg" );
		text = new Text(
			"./ttf/DiabloLight.ttf",
			"UFO Wins", 45, TTF_STYLE_BOLD,
			SDLBase::getColor( 255, 255, 255 ), Text::blended
		);
	}
	else
	{
		bg = new Sprite( "./img/stateWin.jpg" );
		text = new Text(
			"./ttf/DiabloLight.ttf",
			"Hyperion Wins", 45, TTF_STYLE_BOLD,
			SDLBase::getColor( 255, 255, 255 ), Text::blended
		);
	}
	
	bg->render();
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		SDLBase::screen()->h / 4 - text->h() / 2
	);
	
	text->setSize( 15 );
	
	text->setText( "Para jogar, tecle espaço" );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2
	);
	
	text->setText( "Hyperion: 'W', 'A', 'S', 'D'" );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2 + 30
	);
	
	text->setText(
		"UFO: botão esquerdo cria planetas, botão direito movimenta"
	);
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2 + 50
	);
	
	text->setText(
		"Para voltar ao menu, tecle 'M'"
	);
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		2 * SDLBase::screen()->h / 5 - text->h() / 2 + 80
	);
	
	text->setText( "Para sair, tecle ESC, 'Q', ou ALT + F4," );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		4 * SDLBase::screen()->h / 5 - text->h() / 2
	);
	
	text->setText( "ou clique no botão 'X' da janela" );
	
	text->render(
		SDLBase::screen()->w / 2 - text->w() / 2,
		4 * SDLBase::screen()->h / 5 - text->h() / 2 + 20
	);
	
	delete bg;
	delete text;
}

void StateWinLose::handleQuit()
{
	newstate = STATEQUIT;
}

void StateWinLose::handleKeyDown()
{
	switch( INPUTMANAGER_EVENT->key.keysym.sym )
	{
	case SDLK_ESCAPE:
		newstate = STATEQUIT;
		break;
		
	case SDLK_q:
		newstate = STATEQUIT;
		break;
		
	case SDLK_SPACE:
		newstate = STATEGAME;
		break;
		
	default:
		break;
	}
}
