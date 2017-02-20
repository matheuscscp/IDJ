#ifndef STATEMANAGER_HPP
#define STATEMANAGER_HPP

#include "simplestructures.hpp"

#include "State.hpp"

class StateManager
{
private:
	MainArgs args;
	bool quit;
	State* state;
public:
	StateManager(const MainArgs& args);
	~StateManager();
private:
	void initThirdParty();
	void initState();
	
	void closeState();
	void closeThirdParty();
public:
	void run();
private:
	void input();
	void update();
	void render();
	
	template <typename newstate_t> void changeState_();
	void changeState(int newstate);
};

#endif
