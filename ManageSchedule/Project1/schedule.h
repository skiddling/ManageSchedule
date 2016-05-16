#pragma once
#include "timetable.h"

/*
ָ������һ�����壬Ҳ����һ���꼶���ܿα�
*/

class Schedule
{
public:
	//static int team_num_;
	int crash_;
	int reward_;
	double fitness_;
	vector<TimeTable> schedule;//һ���꼶���ܿα�

	Schedule();
	static void set_team_num(int team_num);
	void Init(vector<vector<ClassUnit> > &courses);
	void CalRes();
	void Mutate();
	void Cross();
	bool operator < (Schedule &s) {
		return crash_ == s.crash_ ? fitness_ > s.fitness_ : crash_ < s.crash_;
	}
};