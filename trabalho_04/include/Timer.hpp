#ifndef TIMER_HPP
#define TIMER_HPP

class Timer
{
private:
	int initialtime;
	int pausetime;
	bool paused;
public:
	Timer();
	~Timer();
	
	void start();
	void pause();
	void resume();
	
	int time() const;
	
	bool unused() const;
};

#endif
