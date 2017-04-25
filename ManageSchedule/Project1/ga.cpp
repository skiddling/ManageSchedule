#include "ga.h"

GA::GA() {
	num_of_threads_ = 1;
	//num_of_threads_ = thread::hardware_concurrency();
	schedules_ = vector<Schedule>(num_of_threads_ * thread_schedule_size_);
}

void GA::InitSchedules() {
	//每个schedule不能进行拷贝，只能通过独立的初始化产生
	for (auto& s : schedules_)
		s.init();
}
