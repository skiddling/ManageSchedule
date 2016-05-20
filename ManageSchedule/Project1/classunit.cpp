#include "ClassUnit.h"

int ClassUnit::pim = 0;

ClassUnit::ClassUnit() {
	//this class unit is blank
	alterable_ = 1;
	continue_tag_ = 0;
	cant_time_ = vector<pair<int, int> >(0);
	//this is the symbol of the blank class unit
	class_time_ = make_pair(-1, -1);
}

ClassUnit::ClassUnit(Teacher teacher, int class_id, string class_name, string course_name, int course_id) {
	course_id_ = course_id;
	course_name_ = course_name;
	teacher_ = teacher;
	class_id_ = class_id;
	class_name_ = class_name;
	alterable_ = 1;
	continue_tag_ = 0;
	cant_time_ = vector<pair<int, int> >(0);
	class_time_ = make_pair(-1, -1);
}

void ClassUnit::AddCant(vector<pair<int, int> > &cant_time) {
	cant_time_ = vector<pair<int, int> >(cant_time.size());
	for (int i = 0; i < cant_time.size();i++) {
		cant_time_[i].first = cant_time[i].first;
		cant_time_[i].second = cant_time[i].second;
	}
}

inline pair<int, int> ClassUnit::get_time() {
	return class_time_;
}

void ClassUnit::AddUnit(int x, int y, vector<Teacher *> &teachers) {
	class_time_.first = x;
	class_time_.second = y;
	int tid = teacher_.id_;
	pair<int, int> pt = make_pair(x, y);
	pair<int, int> rt = make_pair(x, class_id_);
	pair<int, int> nt = make_pair(x, y < pim ? 0 : 1);
	if (teachers[tid]->class_table_.find(pt) == teachers[tid]->class_table_.end()) {
		teachers[tid]->class_table_[pt] = 1;
	}
	else teachers[tid]->class_table_[pt]++;

	if (teachers[tid]->room_time_.find(rt) == teachers[tid]->room_time_.end()) {
		teachers[tid]->room_time_[rt] = 1;
	}
	else teachers[tid]->room_time_[rt]++;

	if (teachers[tid]->num_of_period_.find(nt) == teachers[tid]->num_of_period_.end()) {
		teachers[tid]->num_of_period_[nt] = 1;
	}
	else teachers[tid]->num_of_period_[nt]++;
}

void ClassUnit::DelUnit(vector<Teacher *> &teachers) {
	int x = class_time_.first, y = class_time_.second;
	int tid = teacher_.id_;
	pair<int, int> pt = make_pair(x, y);
	pair<int, int> rt = make_pair(x, class_id_);
	pair<int, int> nt = make_pair(x, y < pim ? 0 : 1);
	if (teachers[tid]->class_table_[pt] > 1) {
		teachers[tid]->class_table_[pt]--;
	}
	else {
		map<pair<int, int>, int> ::iterator it = teachers[tid]->class_table_.find(pt);
		teachers[tid]->class_table_.erase(it);
	}

	if (teachers[tid]->room_time_[rt] > 1) {
		teachers[tid]->room_time_[rt]--;
	}
	else {
		map<pair<int, int>, int> ::iterator it = teachers[tid]->room_time_.find(rt);
		teachers[tid]->room_time_.erase(it);
	}

	if (teachers[tid]->num_of_period_[nt] > 1) {
		teachers[tid]->num_of_period_[nt]--;
	}
	else {
		map<pair<int, int>, int> ::iterator it = teachers[tid]->num_of_period_.find(nt);
		teachers[tid]->num_of_period_.erase(it);
	}
}
