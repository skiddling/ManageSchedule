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

//给该节课赋值上时间和老师的信息
void ClassUnit::AddUnit(int x, int y, vector<Teacher> &teachers) {
	class_time_.first = x;
	class_time_.second = y;
	int tid = teacher_.id_, period = (y < pim ? 0 : 1);
	pair<int, int> pt = make_pair(x, y);
	pair<int, int> rt = make_pair(x, class_id_);
	//pair<int, int> nt = make_pair(x, y < pim ? 0 : 1);
	if (teachers[tid].class_table_.find(pt) == teachers[tid].class_table_.end()) {
		teachers[tid].class_table_[pt] = 1;
	}
	else teachers[tid].class_table_[pt]++;

	if (teachers[tid].room_time_.find(rt) == teachers[tid].room_time_.end()) {
		teachers[tid].room_time_[rt] = 1;
	}
	else teachers[tid].room_time_[rt]++;

	teachers[tid].num_of_period[x][period]++;
	//删除教师在这个时间段的空余，表明该时间段该老师是上课的
	map<pair<int, int>, bool>::iterator it;
	if (teachers[tid].available_time.find(pt) != teachers[tid].available_time.end()) {
		it = teachers[tid].available_time.find(pt);
		teachers[tid].available_time.erase(it);
	}
}

//让删除该节课，其实只更新老师的信息
void ClassUnit::DelUnit(vector<Teacher> &teachers) {
	int x = class_time_.first, y = class_time_.second;
	int tid = teacher_.id_, period = (y < pim ? 0 : 1);
	//pair<int, int> pt = make_pair(x, y);
	pair<int, int> pt = class_time_;
	pair<int, int> rt = make_pair(x, class_id_);
	cout << rt.first << ' ' << rt.second << endl;
	//pair<int, int> nt = make_pair(x, y < pim ? 0 : 1);
	if (teachers[tid].class_table_[pt] > 1) {
		teachers[tid].class_table_[pt]--;
	}
	else {
		//表明该时间段老师没有其他的课程，也就是该时间段老师又有空了
		map<pair<int, int>, int> ::iterator it = teachers[tid].class_table_.find(pt);
		teachers[tid].class_table_.erase(it);
		teachers[tid].available_time[pt] = 1;
	}

	if (teachers[tid].room_time_[rt] > 1) {
		teachers[tid].room_time_[rt]--;
	}
	else {
		map<pair<int, int>, int> ::iterator it = teachers[tid].room_time_.find(rt);
		teachers[tid].room_time_.erase(it);
	}

	teachers[tid].num_of_period[x][period]--;
}

bool ClassUnit::IsConflict(vector<Teacher> &teachers) {
	int tid = teacher_.id_;
	int x = class_time_.first, y = class_time_.second;
	pair<int, int> pt = make_pair(x, y);
	pair<int, int> rt = make_pair(x, class_id_);
	if (teachers[tid].class_table_[pt] > 1)return 1;
	if (teachers[tid].room_time_[rt] > 1)return 1;
	return 0;
}
