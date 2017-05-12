#include "ga.h"

GA::GA() {
	num_of_threads_ = 1;
	//num_of_threads_ = thread::hardware_concurrency();
	//����һ�������Ŀα�res_��Ȼ��ͨ����������Ӧ�ĸ�������α���ȥ
	InitSchedules();
	//schedule copy construction 
	schedules_ = vector<Schedule>(num_of_threads_ * thread_schedule_size_, res_);
}

void GA::InitSchedules() {
	//step 1. init schedule res
	//ÿ��schedule���ܽ��п�����ֻ��ͨ�������ĳ�ʼ������
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
