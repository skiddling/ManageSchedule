#pragma once
#include "course.h"

class InterruptFlag
{
public:
	inline void set() {
		lock_guard<mutex> guard(_mtx);
		_set = true;
	}

	inline bool is_set() {
		lock_guard<mutex> guard(_mtx);
		return _set;
	}

private:
	mutex _mtx;
	bool _set;
};


struct thread_interrupted {};

extern thread_local InterruptFlag this_thread_interrupt_flag;

void interruption_point();
//{
//	if (this_thread_interrupt_flag.is_set()) {
//		throw thread_interrupted();
//	}
//}
