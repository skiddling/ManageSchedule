#pragma once
#include "timetable.h"
#include "interruptflag.h"
#include "InterruptibleThread.h"
//#include "InterruptibleThread.h"

class Schedule
{
public:

	vector<TimeTable> timetables_;
	int outtime_;
	int crash_;

	Schedule();
	//copy construction
	Schedule(const Schedule& s);
	Schedule& operator=(const Schedule& s);
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	void UpdateFromRes();
	void init();
	
private:
	void CalFitness();
	void Cross();
	void Modify();
};