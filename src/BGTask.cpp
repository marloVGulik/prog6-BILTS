#include "BGTask.h"

BGTask::BGTask() {
	_active = false;
}
void BGTask::_run() {
	try {
		while(_active) {
			runnable();
		}
	} catch(int e) {
		std::cout << e << std::endl;
		// exit(e);
	}
}

bool BGTask::start() {
	if(!_active) {
		_active = true;
		thread th(&BGTask::_run, this);
		_thread = th;
		return true
	} else return false;
}

void BGTask::stop() {
	_active = false;
}

bool BGTask::join() {
	if(_active == false) {
		_thread.join();
		return true;
	} else return false;
}
