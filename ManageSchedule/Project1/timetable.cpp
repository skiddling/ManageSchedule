#include "timetable.h"

int TimeTable::days_per_week_ = 0;
int TimeTable::period_in_moring_ = 0;
int TimeTable::period_per_day_ = 0;

TimeTable::TimeTable() {
	class_que_ = vector<ClassUnit>(0);
	table_ = vector<vector<ClassUnit> >(days_per_week_, vector<ClassUnit>(period_per_day_));
}

//让每一节课都按照科目放到班级课表当中的科目系统当中，方便添加限制条件
void TimeTable::Init(int courses_num) {
	course_classes = vector<vector<ClassUnit *> >(courses_num);
	vector<ClassUnit> ::iterator itc = class_que_.begin();
	for (; itc != class_que_.end(); itc++) {
		course_classes[itc->course_id_].push_back(&(*itc));
	}
}

void TimeTable::AddContinue(int course_id, int continue_num) {
	int k = 0;
	for (int i = 0; i < continue_num; i++) {
		course_classes[course_id][k++]->continue_tag_ = 1;
		course_classes[course_id][k++]->continue_tag_ = 2;
	}
}

void TimeTable::AddCant(int course_id, vector<pair<int, int> > &cant_time) {
	for (int i = 0; i < course_classes[course_id].size(); i++) {
		course_classes[course_id][i]->AddCant(cant_time);
	}
}

void TimeTable::AddItime(int course_id, vector<pair<int, int> > &itime) {
	for (int i = 0; i < itime.size(); i++) {
		course_classes[course_id][i]->alterable_ = 0;
		course_classes[course_id][i]->class_time_.first = itime[i].first;
		course_classes[course_id][i]->class_time_.second = itime[i].second;
	}
}

void TimeTable::Mutate(double mp) {

}

void TimeTable::Cross(TimeTable &timetable) {

}

void TimeTable::CalCrash() {

}
