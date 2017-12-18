#pragma once
//#include "schedule.h"
#include "teacher.h"
#include "InterruptibleThread.h"

class GA
{
public:
	Schedule res_;
	int outtime_ = 5;//运行时限

	GA();
	GA(vector<Course> courses, vector<Teacher> teachers, vector<TimeTable>timetables, vector<ClassUnit> units);
	void GenerateTable();
	void OutPutRes();

private:
	int num_of_threads_;
	const int thread_schedule_size_ = 5;
	//实体对象
	vector<Schedule> schedules_;
	void GetSchedule(int thid, InterruptibleThread* t, future<Schedule>* fut);
};
