/*

The Observer Design Pattern

WARNING: To use this pattern, any subject class of yours should:

- include the SUBJECT preprocessor macro in its beginning, like:

class <YOUR_SUBJECT_CLASS_HERE>
{
SUBJECT
<YOUR_STUFF_HERE>
};

- and call:

subject.init ( <YOUR_SUBJECT_CLASS_EVENTS_AMOUNT> );

*/

#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <list>

#define SUBJECT								\
protected:								\
	observer::Subject subject;					\
public:									\
	template <class obs_type>					\
	void connect (							\
		unsigned int event_type,					\
		obs_type* observer,					\
		typename observer::handler< obs_type >::type handlerfp	\
	)								\
	{								\
		subject.connect ( event_type, observer, handlerfp );	\
	}								\
									\
	template <class obs_type>					\
	void disconnect (obs_type* observer)				\
	{								\
		subject.disconnect ( observer );			\
	}								\
									\
	template <class obs_type>					\
	void disconnect (unsigned int event_type, obs_type* observer)		\
	{								\
		subject.disconnect ( event_type, observer );		\
	}								\
									\
	const observer::Event* event () const				\
	{								\
		return subject.event;					\
	}

namespace observer
{

// =============================================================================
// Notifier Method Type
// =============================================================================

template <class obs_type>
class handler
{
public:
	typedef void (obs_type::*type)();
};

// =============================================================================
// Generic Event
// =============================================================================

class Event
{
private:
	unsigned int type_;
public:
	Event (unsigned int type) : type_ ( type )
	{
	}
	
	virtual ~Event ()
	{
	}
	
	unsigned int type () const
	{
		return type_;
	}
};

// =============================================================================
// Subject Class
// =============================================================================

class Subject
{
private:
	struct Observer
	{
		virtual ~Observer ()
		{
		}
		
		virtual void callHandler () = 0;
	};
	
	template <class obs_type>
	struct ObserverDerived : public Observer
	{
		obs_type* observer;
		typename handler< obs_type >::type handlerfp;
		
		ObserverDerived (
			obs_type* observer,
			typename handler< obs_type >::type handlerfp
		) :
		observer ( observer ), handlerfp ( handlerfp )
		{
		}
		
		void callHandler ()
		{
			((*observer).*(handlerfp))();
		}
	};
	
	std::list< Observer* >* observers;
	unsigned int n_events;
public:
	Event* event;
	
	Subject () : observers ( 0 ), event ( 0 )
	{
	}
	
	~Subject ()
	{
		if ( observers )
		{
			for ( unsigned int i = 0; i < n_events; ++i )
			{
				while ( observers[i].size () )
				{
					delete observers[i].back ();
					observers[i].pop_back ();
				}
			}
			
			delete[] observers;
		}
	}
	
	void init (unsigned int n_events)
	{
		this->n_events = n_events;
		
		observers = new std::list< Observer* > [ n_events ];
	}
	
	template <class obs_type>
	void connect (
		unsigned int event_type,
		obs_type* observer,
		typename handler< obs_type >::type handlerfp
	)
	{
		if ( observers )
		{
			std::list< Observer* >::iterator it;
			bool found = false;
			
			for (
				it = observers[ event_type ].begin ();
				( it != observers[ event_type ].end () ) && ( !found );
				++it
			)
			{
				if ( ( (ObserverDerived< obs_type >*) (*it) )->observer == observer )
				{
					found = true;
					( (ObserverDerived< obs_type >*) (*it) )->handlerfp = handlerfp;
				}
			}
			
			if ( !found )
			{
				observers[ event_type ].push_back (
					new ObserverDerived< obs_type > ( observer, handlerfp )
				);
			}
		}
	}
	
	template <class obs_type>
	void disconnect (obs_type* observer)
	{
		if ( observers )
		{
			for ( unsigned int i = 0; i < n_events; ++i )
			{
				std::list< Observer* >::iterator it;
				bool found = false;
				
				for (
					it = observers[i].begin ();
					( it != observers[i].end () ) && ( !found );
					++it
				)
				{
					if ( ( (ObserverDerived< obs_type >*) (*it) )->observer == observer )
					{
						found = true;
						observers[i].erase ( it );
					}
				}
			}
		}
	}
	
	template <class obs_type>
	void disconnect (unsigned int event_type, obs_type* observer)
	{
		if ( observers )
		{
			std::list< Observer* >::iterator it;
			bool found = false;
			
			for (
				it = observers[ event_type ].begin ();
				( it != observers[ event_type ].end () ) && ( !found );
				++it
			)
			{
				if ( ( (ObserverDerived< obs_type >*) (*it) )->observer == observer )
				{
					found = true;
					observers[ event_type ].erase ( it );
				}
			}
		}
	}
	
	void broadcast (const Event& event)
	{
		if ( observers )
		{
			this->event = ( (Event*) &event );
			
			std::list< Observer* >::iterator it;
			unsigned int event_type = event.type ();
			
			for (
				it = observers[ event_type ].begin ();
				it != observers[ event_type ].end ();
				++it
			)
			{
				(*it)->callHandler ();
			}
			
			this->event = 0;
		}
	}
};

}

#endif
