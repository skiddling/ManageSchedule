#include "ClassUnit.h"

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