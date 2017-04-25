#include "ga.h"

GA::GA() {
	num_of_threads_ = 1;
	//num_of_threads_ = thread::hardware_concurrency();
	schedules_ = vector<Schedule>(num_of_threads_ * thread_schedule_size_);
}

void GA::InitSchedules() {
	//ÿ��schedule���ܽ��п�����ֻ��ͨ�������ĳ�ʼ������
	for (auto& s : schedules_)
		s.init();
}
