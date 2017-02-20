#ifndef STATE_HPP
#define STATE_HPP

#include "simplestructures.hpp"

class StateArgs {};

class State
{
protected:
	MainArgs* args;
	int newstate;
public:
	State();
	virtual ~State();
	
	virtual void load(MainArgs* args, StateArgs* st_args = 0) = 0;
	virtual StateArgs* unload() = 0;
	
	virtual int input() = 0;
	virtual int update() = 0;
	virtual void render() = 0;
};

#endif
