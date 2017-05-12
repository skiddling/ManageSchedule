#pragma once
#include "timetable.h"
#include "interruptflag.h"
#include "InterruptibleThread.h"
//#include "InterruptibleThread.h"

class Schedule
{
public:
	vector<Teacher> teachers_;//所有的教师的实体
	vector<ClassUnit> clsque_;//全部节次的课
	vector<TimeTable> timetables_;//所有的班级课表，相当于是一个指针容器
	int outtime_;
	int crash_;

	Schedule() = default;
	//copy construction
	Schedule(const Schedule& s);
	Schedule& operator=(const Schedule& s);
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	void init();
	
private:
	void CalFitness();
	void Cross();
	void Modify();
	void UpdatePtrs();
};