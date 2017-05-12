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
	//实体对象
	vector<Schedule> schedules_;
	vector<Teacher> teachers_;
	vector<ClassUnit> units_;
	vector<Course> courses_;

	void InitSchedules();
	void GenerateTable();
	void GetSchedule(int thid, InterruptibleThread* t, future<Schedule>* fut);
};
