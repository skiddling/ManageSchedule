#pragma once
#include "timetable.h"
#include "interruptflag.h"
#include "InterruptibleThread.h"
//#include "InterruptibleThread.h"

class Schedule
{
public:
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	void UpdateFromRes();
	void init();
	vector<TimeTable> timetables_;
	int outtime_;
	int crash_;
private:
	void CalFitness();
	void Cross();
	void Modify();
};