#define SIMPLES_NOCOUT
#include "mod/simplestructures.hpp"

#include "Geometry.hpp"

using namespace lalge;

Ellipse::Ellipse (
	const R2Vector& r,
	const Scalar& depthconst,
	const Scalar& a,
	const Scalar& b,
	const Scalar& angle
) : GameObject ( r, depthconst ), a_ ( a ), b_ ( b ), angle_ ( angle )
{
	if ( a <= 0 )
		throw ( mexception ( "Invalid ellipse \"a\" axis" ) );
	
	if ( b <= 0 )
		throw ( mexception ( "Invalid ellipse \"b\" axis" ) );
	
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid ellipse angle" ) );
}

Ellipse::~Ellipse ()
{
}

const Scalar& Ellipse::a () const
{
	return a_;
}

const Scalar& Ellipse::b () const
{
	return b_;
}

const Scalar& Ellipse::angle () const
{
	return angle_;
}

void Ellipse::set_a (const Scalar& a)
{
	if ( a <= 0 )
		throw ( mexception ( "Invalid ellipse \"a\" axis" ) );
	
	a_ = a;
}

void Ellipse::set_b (const Scalar& b)
{
	if ( b <= 0 )
		throw ( mexception ( "Invalid ellipse \"b\" axis" ) );
	
	b_ = b;
}

void Ellipse::setAngle (const Scalar& angle)
{
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid ellipse angle" ) );
	
	angle_ = angle;
}

Scalar Ellipse::area () const
{
	return ( pi * a_ * b_ );
}

bool Ellipse::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

Circle::Circle (
	const R2Vector& r,
	const Scalar& depthconst,
	const Scalar& radius
) : GameObject ( r, depthconst ), radius_ ( radius )
{
	if ( radius <= 0 )
		throw ( mexception ( "Invalid circle radius" ) );
}

Circle::~Circle ()
{
}

const Scalar& Circle::radius () const
{
	return radius_;
}

void Circle::setRadius (const Scalar& radius)
{
	if ( radius <= 0 )
		throw ( mexception ( "Invalid circle radius" ) );
	
	radius_ = radius;
}

Scalar Circle::length () const
{
	return ( 2 * pi * radius_ );
}

Scalar Circle::area () const
{
	return ( pi * radius_ * radius_ );
}

bool Circle::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Circle::colliding (const Circle& param) const
{
	Scalar dif = sqrt (
	( param.r.x ( 0 ) - r.x ( 0 ) ) * ( param.r.x ( 0 ) - r.x ( 0 ) ) +
	( param.r.x ( 1 ) - r.x ( 1 ) ) * ( param.r.x ( 1 ) - r.x ( 1 ) )
	);
	
	return ( radius_ + param.radius_ >= dif );
}

Line::Line (
	const R2Vector& r,
	const Scalar& depthconst,
	const R2Vector& dir
) : GameObject ( r, depthconst ), dir_ ( dir )
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line direction" ) );
}

Line::~Line ()
{
}

const R2Vector& Line::dir () const
{
	return dir_;
}

void Line::setDir (const R2Vector& dir)
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line direction" ) );
	
	dir_ = dir;
}

bool Line::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Line::colliding (const Circle& param) const
{
	R2Vector tmp;
	
	tmp.set ( 0, param.r.x ( 0 ) - r.x ( 0 ) );
	tmp.set ( 1, param.r.x ( 1 ) - r.x ( 1 ) );
	
	Scalar projlen = dir_.proj ( tmp ).length ();
	
	Scalar normal_length = sqrt (
		tmp.length () * tmp.length () -
		projlen * projlen
	);
	
	return ( param.radius () >= normal_length );
}

bool Line::colliding (const Line& param) const
{
	return (	( dir_.x ( 0 ) * param.dir_.x ( 1 ) ) !=
			( dir_.x ( 1 ) * param.dir_.x ( 0 ) )	);
}

LineSegment::LineSegment (
	const R2Vector& r,
	const Scalar& depthconst,
	const R2Vector& dir,
	const Scalar& interval_size,
	const Scalar& t0
) : GameObject ( r, depthconst ), dir_ ( dir ),
interval_size_ ( interval_size ), t0 ( t0 )
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line segment direction" ) );
	
	if ( interval_size <= 0 )
		throw ( mexception ( "Invalid line segment interval size" ) );
}

LineSegment::~LineSegment ()
{
}

const R2Vector& LineSegment::dir () const
{
	return dir_;
}

const Scalar& LineSegment::interval_size () const
{
	return interval_size_;
}

void LineSegment::setDir (const R2Vector& dir)
{
	if ( !dir.length () )
		throw ( mexception ( "Invalid line segment direction" ) );
	
	dir_ = dir;
}

void LineSegment::setIntervalSize (const Scalar& interval_size)
{
	if ( interval_size <= 0 )
		throw ( mexception ( "Invalid line segment interval size" ) );
	
	interval_size_ = interval_size;
}

Scalar LineSegment::length () const
{
	return ( dir_.length () * interval_size_ );
}

bool LineSegment::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool LineSegment::colliding (const Circle& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool LineSegment::colliding (const Line& param) const
{
	Scalar t = (
		dir_.x ( 0 ) * ( -param.dir ().x ( 1 ) ) -
		dir_.x ( 1 ) * ( -param.dir ().x ( 0 ) )
	);
	
	if ( !t )
		return false;
	
	t = (
		( param.r.x ( 0 ) - r.x ( 0 ) ) * ( -param.dir ().x ( 1 ) ) -
		( param.r.x ( 1 ) - r.x ( 1 ) ) * ( -param.dir ().x ( 0 ) )
	) / t;
	
	return ( ( t >= t0 ) && ( t <= t0 + interval_size_ ) );
}

bool LineSegment::colliding (const LineSegment& param) const
{
	Scalar t, t1, t2;
	
	t = (
		dir_.x ( 0 ) * ( -param.dir_.x ( 1 ) ) -
		dir_.x ( 1 ) * ( -param.dir_.x ( 0 ) )
	);
	
	if ( !t )
		return false;
	
	t1 = (
		( param.r.x ( 0 ) - r.x ( 0 ) ) * ( -param.dir_.x ( 1 ) ) -
		( param.r.x ( 1 ) - r.x ( 1 ) ) * ( -param.dir_.x ( 0 ) )
	) / t;
	
	if ( !( ( t1 >= t0 ) && ( t1 <= t0 + interval_size_ ) ) )
		return false;
	
	t2 = (
		dir_.x ( 0 ) * ( param.r.x ( 1 ) - r.x ( 1 ) ) -
		dir_.x ( 1 ) * ( param.r.x ( 0 ) - r.x ( 0 ) )
	) / t;
	
	return (
		( t2 >= param.t0 ) &&
		( t2 <= param.t0 + param.interval_size_ )
	);
}

Rectangle::Rectangle (
	const R2Vector& r,
	const Scalar& depthconst,
	const Scalar& a,
	const Scalar& b,
	const Scalar& angle
) : GameObject ( r, depthconst ), a_ ( a ), b_ ( b ), angle_ ( angle )
{
	if ( a <= 0 )
		throw ( mexception ( "Invalid rectangle \"a\" axis" ) );
	
	if ( b <= 0 )
		throw ( mexception ( "Invalid rectangle \"b\" axis" ) );
	
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid rectangle angle" ) );
}

Rectangle::~Rectangle ()
{
}

const Scalar& Rectangle::a () const
{
	return a_;
}

const Scalar& Rectangle::b () const
{
	return b_;
}

const Scalar& Rectangle::angle () const
{
	return angle_;
}

void Rectangle::set_a (const Scalar& a)
{
	if ( a <= 0 )
		throw ( mexception ( "Invalid rectangle \"a\" axis" ) );
	
	a_ = a;
}

void Rectangle::set_b (const Scalar& b)
{
	if ( b <= 0 )
		throw ( mexception ( "Invalid rectangle \"b\" axis" ) );
	
	b_ = b;
}

void Rectangle::setAngle (const Scalar& angle)
{
	if ( ( angle < 0 ) || ( angle >= 180 ) )
		throw ( mexception ( "Invalid rectangle angle" ) );
	
	angle_ = angle;
}

Scalar Rectangle::length () const
{
	return ( 2 * ( a_ + b_ ) );
}

Scalar Rectangle::area () const
{
	return ( a_ * b_ );
}

bool Rectangle::colliding (const Ellipse& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const Circle& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const Line& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const LineSegment& param) const
{
	// FIXME
	return ( !( &param ) );
}

bool Rectangle::colliding (const Rectangle& param) const
{
	// FIXME
	return ( !( &param ) );
}
