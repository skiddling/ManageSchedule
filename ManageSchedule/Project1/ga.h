#pragma once
#include "schedule.h"
#include "teacher.h"
#include "InterruptibleThread.h"

class GA
{
public:
	GA();
	Schedule res_;
	int outtime_;//运行时限

private:
	int num_of_threads_;
	const int thread_schedule_size_ = 5;
	vector<Schedule> schedules_;
	vector<ClassUnit> units_;
	vector<Course> courses_;
	vector<Teacher> teachers_;

	void InitSchedules();
	void GenerateTable();
	void GetSchedule(int thid, InterruptibleThread* t, future<Schedule>* fut);
};
