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

void GA::GenerateTable() {
	promise<Schedule> pro;
	future<Schedule> fut = pro.get_future();
	vector<InterruptibleThread> threads(num_of_threads_);
	
	auto t1 = chrono::system_clock::now();
	chrono::duration<int, ratio<60, 1>> dur(outtime_);
	for (auto i = 0; i < num_of_threads_; i++) {
		threads[i] = InterruptibleThread(this, &GA::GetSchedule, i, &pro, &fut);
	}
}

void GA::GetSchedule(int thid, InterruptibleThread* t, future<Schedule>* fut) {
	for (auto i = 0; i < thread_schedule_size_; i++) {
		schedules_[i + thid * thread_schedule_size_].GetSchedule(t, fut);
	}
}
