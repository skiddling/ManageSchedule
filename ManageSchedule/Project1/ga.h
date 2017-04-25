#pragma once
#include "schedule.h"

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

	void InitSchedules();
	void GenerateTable();
};
