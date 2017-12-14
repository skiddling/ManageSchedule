#pragma once
//#include "schedule.h"
#include "teacher.h"
#include "InterruptibleThread.h"

class GA
{
public:
	Schedule res_;
	int outtime_;//运行时限

	GA();
	GA(vector<Course> courses, vector<Teacher> teachers, vector<TimeTable>timetables, vector<ClassUnit> units);
	void GenerateTable();
	void OutPutRes();

private:
	int num_of_threads_;
	const int thread_schedule_size_ = 5;
	//实体对象
	vector<Schedule> schedules_;
	/*vector<Teacher> teachers_;
	vector<ClassUnit> units_;
	vector<Course> courses_;
	vector<TimeTable> timetables_;*/
	//unordered_set<int> deletedunits_;

	void InitSchedules();
	void GetSchedule(int thid, InterruptibleThread* t, future<Schedule>* fut);
};
