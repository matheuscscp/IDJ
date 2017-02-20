#define SIMPLES_NOCOUT
#include "mod/simplestructures.hpp"

#include "Animation.hpp"

using namespace std;

Animation::Animation (
	const string& filename,
	int frame,
	int framesize,
	int rows,
	int cols
) : rows ( rows ), cols ( cols )
{
	if ( ( rows <= 0 ) || ( cols <= 0 ) )
		throw ( mexception ( "Invalid Animation rows or cols value" ) );
	
	load ( filename );
	
	setFrame ( frame );
	setFrameSize ( framesize );
	
	update_ ();
}

void Animation::update ()
{
	float dt = float ( SDLBase::dt () ) / 1000;
	
	frame = ( ( frame + fps * dt ) - float (
		( int ( frame + fps * dt ) / ( rows * cols ) ) * ( rows * cols )
	) );
	
	update_ ();
}

void Animation::update_ ()
{
	clip (
		( int ( frame ) % cols ) * ( src->w / cols ),
		( int ( frame ) / cols ) * ( src->h / rows ),
		src->w / cols,
		src->h / rows
	);
}

int Animation::getFrame () const
{
	return int ( frame );
}

int Animation::getFrameSize () const
{
	return int ( 1000.0f / fps );
}

void Animation::setFrame (int frame)
{
	this->frame = float ( frame );
	
	update_ ();
}

void Animation::setFrameSize (int framesize)
{
	fps = 1000.0f / float ( framesize );
}
