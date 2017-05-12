#include "ga.h"

GA::GA() {
	num_of_threads_ = 1;
	//num_of_threads_ = thread::hardware_concurrency();
	//生成一个初步的课表res_，然后通过拷贝到相应的各个具体课表当中去
	InitSchedules();
	//schedule copy construction 
	schedules_ = vector<Schedule>(num_of_threads_ * thread_schedule_size_, res_);
}

void GA::InitSchedules() {
	//step 1. init schedule res
	//每个schedule不能进行拷贝，只能通过独立的初始化产生
	res_.teachers_ = teachers_;
	res_.init();
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
