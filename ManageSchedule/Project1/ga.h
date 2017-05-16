#pragma once
#include "schedule.h"
#include "teacher.h"
#include "InterruptibleThread.h"

class GA
{
public:
	Schedule res_;
	int outtime_;//����ʱ��

	GA();
	void GenerateTable();
	void OutPutRes();

private:
	int num_of_threads_;
	const int thread_schedule_size_ = 5;
	//ʵ�����
	vector<Schedule> schedules_;
	vector<Teacher> teachers_;
	vector<ClassUnit> units_;
	vector<Course> courses_;

	void InitSchedules();
	void GetSchedule(int thid, InterruptibleThread* t, future<Schedule>* fut);
};
