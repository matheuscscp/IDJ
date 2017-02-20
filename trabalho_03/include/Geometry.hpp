#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "GameObject.hpp"

class Ellipse : public GameObject
{
protected:
	lalge::Scalar a_;
	lalge::Scalar b_;
	lalge::Scalar angle_;
public:
	Ellipse (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		const lalge::Scalar& a = 2,
		const lalge::Scalar& b = 1,
		const lalge::Scalar& angle = 0
	);
	virtual ~Ellipse ();
	
	virtual void update () = 0;
	
	virtual void render () = 0;
	
	virtual GameObject* clone () const = 0;
	
	const lalge::Scalar& a () const;
	const lalge::Scalar& b () const;
	const lalge::Scalar& angle () const;
	
	void set_a (const lalge::Scalar& a);
	void set_b (const lalge::Scalar& b);
	void setAngle (const lalge::Scalar& angle);
	
	lalge::Scalar area () const;
	
	bool colliding (const Ellipse& param) const;
};

class Circle : public GameObject
{
protected:
	lalge::Scalar radius_;
public:
	Circle (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		const lalge::Scalar& radius_ = 1
	);
	virtual ~Circle ();
	
	virtual void update () = 0;
	
	virtual void render () = 0;
	
	virtual GameObject* clone () const = 0;
	
	const lalge::Scalar& radius () const;
	
	void setRadius (const lalge::Scalar& radius);
	
	lalge::Scalar length () const;
	lalge::Scalar area () const;
	
	bool colliding (const Ellipse& param) const;
	bool colliding (const Circle& param) const;
};

class Line : public GameObject
{
protected:
	lalge::R2Vector dir_;
public:
	Line (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		const lalge::R2Vector& dir = lalge::r2vec ( 1, 1 )
	);
	virtual ~Line ();
	
	virtual void update () = 0;
	
	virtual void render () = 0;
	
	virtual GameObject* clone () const = 0;
	
	const lalge::R2Vector& dir () const;
	
	void setDir (const lalge::R2Vector& dir);
	
	bool colliding (const Ellipse& param) const;
	bool colliding (const Circle& param) const;
	bool colliding (const Line& param) const;
};

class LineSegment : public GameObject
{
protected:
	lalge::R2Vector dir_;
	lalge::Scalar interval_size_;
public:
	lalge::Scalar t0;
	
	LineSegment (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		const lalge::R2Vector& dir = lalge::r2vec ( 1, 1 ),
		const lalge::Scalar& interval_size = 1,
		const lalge::Scalar& t0 = 0
	);
	virtual ~LineSegment ();
	
	virtual void update () = 0;
	
	virtual void render () = 0;
	
	virtual GameObject* clone () const = 0;
	
	const lalge::R2Vector& dir () const;
	const lalge::Scalar& interval_size () const;
	
	void setDir (const lalge::R2Vector& dir);
	void setIntervalSize (const lalge::Scalar& interval_size);
	
	lalge::Scalar length () const;
	
	bool colliding (const Ellipse& param) const;
	bool colliding (const Circle& param) const;
	bool colliding (const Line& param) const;
	bool colliding (const LineSegment& param) const;
};

class Rectangle : public GameObject
{
protected:
	lalge::Scalar a_;
	lalge::Scalar b_;
	lalge::Scalar angle_;
public:
	Rectangle (
		const lalge::R2Vector& r = lalge::R2Vector (),
		const lalge::Scalar& depthconst = 1,
		const lalge::Scalar& a = 2,
		const lalge::Scalar& b = 1,
		const lalge::Scalar& angle = 0
	);
	virtual ~Rectangle ();
	
	virtual void update () = 0;
	
	virtual void render () = 0;
	
	virtual GameObject* clone () const = 0;
	
	const lalge::Scalar& a () const;
	const lalge::Scalar& b () const;
	const lalge::Scalar& angle () const;
	
	void set_a (const lalge::Scalar& a);
	void set_b (const lalge::Scalar& b);
	void setAngle (const lalge::Scalar& angle);
	
	lalge::Scalar length () const;
	lalge::Scalar area () const;
	
	bool colliding (const Ellipse& param) const;
	bool colliding (const Circle& param) const;
	bool colliding (const Line& param) const;
	bool colliding (const LineSegment& param) const;
	bool colliding (const Rectangle& param) const;
};

#endif
