#ifndef BG_TASK
#define BG_TASK

#include <thread>

class BGTask {
public:
	BGTask();

	bool start();
	bool stop();
	bool join();
	virtual void runnable();
private:
	void _run();
	bool _active;
	thread _thread;
}

#endif
