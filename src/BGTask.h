#ifndef BG_TASK
#define BG_TASK

#include <thread>
#include <iostream>

class BGTask {
public:
	BGTask();

	bool start();
	void stop();
	bool join();
	virtual void runnable() {};
private:
	void _run();
	bool _active;
	std::thread _thread;
};

#endif
