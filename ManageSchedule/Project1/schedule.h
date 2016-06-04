#pragma once
#include "timetable.h"

/*
指代的是一个个体，也就是一个年级的总课表
*/

class Schedule
{
public:
	//static int team_num_;
	static map<string, int> courses_map_;
	static map<string, int> teachers_map_;
	int crash_;
	int reward_;
	double fitness_;
	vector<TimeTable> time_tables_;//一个年级的总课表
	vector<Teacher> teachers_;
	vector<Course> courses_;

	void Init(vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable> &time_tables);
	void CalRes();
	void Mutate(double mp);
	void Cross(Schedule &another, double cp);
	void CalFitness();
	void Modify();
	bool operator < (Schedule &s) {
		return crash_ == s.crash_ ? fitness_ > s.fitness_ : crash_ < s.crash_;
	}
};