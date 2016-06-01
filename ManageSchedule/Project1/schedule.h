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
	vector<TimeTable> time_tables_;//һ���꼶���ܿα�
	map<string, int> teachers_map_;
	map<string, int> courses_map_;

	vector<Teacher *> teachers_;
	vector<ClassUnit *> class_units_;
	vector<Course *> courses_;

	void Init(map<string, int> &teachers_map, map<string, int> &courses_map,
		vector<Teacher *> &teachers, vector<ClassUnit *> &class_units,
		vector<Course *> &courses, vector<TimeTable> &time_tables);
	void CalRes();
	void Mutate(double mp);
	void Cross(Schedule &another, double cp);
	void CalFitness(int mxreward);
	void Modify();
	bool operator < (Schedule &s) {
		return crash_ == s.crash_ ? fitness_ > s.fitness_ : crash_ < s.crash_;
	}
};