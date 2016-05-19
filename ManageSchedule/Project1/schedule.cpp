#include "schedule.h"

void Schedule::Init(map<string, int> &teachers_map, map<string, int> &courses_map,
	vector<Teacher *> &teachers, vector<ClassUnit *> &class_units,
	vector<Course *> &courses, vector<TimeTable> &time_tables) {
	teachers_map_ = teachers_map;
	courses_map_ = courses_map;
	teachers_ = teachers;
	courses_ = courses;
	time_tables_ = time_tables;
	for (int i = 0; i < time_tables_.size(); i++) {
		time_tables[i].Init(courses_map, time_tables_[i], teachers_);
	}
}


void Schedule::CalRes() {

}

void Schedule::Mutate(double mp) {
	for (int i = 0; i < time_tables_.size(); i++) {
		time_tables_[i].Mutate(mp, teachers_);
	}

}

void Schedule::Cross(Schedule &another, double cp) {
	for (int i= 0; i < time_tables_.size(); i++) {
		time_tables_[i].Cross(another.time_tables_[i], cp);
	}
}

void Schedule::CalFitness(int mxreward) {
	fitness_ = 1.0 / (double)(1 + crash_);
	if (crash_ <= 0)fitness_ = (double)reward_ / mxreward;
}