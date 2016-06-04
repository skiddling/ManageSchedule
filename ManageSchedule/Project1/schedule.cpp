#include "schedule.h"

map<string, int> Schedule::courses_map_;
map<string, int> Schedule::teachers_map_;

void Schedule::Init(vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable> &time_tables){
	teachers_ = teachers;
	courses_ = courses;
	//time_tables_ = time_tables;
	for (int i = 0; i < teachers.size(); i++) {
		teachers_[i].InitAvailable(TimeTable::days_per_week_, TimeTable::period_per_day_);
	}
	//cout << "schedule.init\n";
	//system("PAUSE");
	time_tables_ = vector<TimeTable>(time_tables.size());
	for (int i = 0; i < time_tables.size(); i++) {
		time_tables_[i].Init(time_tables[i], teachers_);
		//cout << time_tables[i].table_.size();
	}
	cout << "schedule.init\n";
	//system("PAUSE");
}

void Schedule::CalRes() {
	//计算冲突和奖励
	//冲突种类：1.老师在同一个时间段内重复上了多少次课
	//奖励：一个老师上课的半天的次数越少越好
	int crash = 0, reward = 0, dpw = TimeTable::days_per_week_;
	map<pair<int, int>, int>::iterator it;
	for (int i = 0; i < teachers_.size(); i++) {
		for (it = teachers_[i].class_table_.begin(); it != teachers_[i].class_table_.end(); it++) {
			if (it->second > 1)crash += (it->second - 1);
		}
	}

	for (int i = 0; i < teachers_.size(); i++) {
		for (it = teachers_[i].room_time_.begin(); it != teachers_[i].room_time_.end(); it++) {
			if (it->second > 1)crash += (it->second - 1);
		}
	}

	for (int i = 0; i < teachers_.size(); i++) {
		for (int x = 0; x < dpw; x++) {
			for (int j = 0; j < 2; j++) {
				if (teachers_[i].num_of_period[x][j] > 0) {
					reward++;
				}
			}
		}
	}
	crash_ = crash;
	reward_ = reward;
}

void Schedule::Modify() {
	/*for (int i = 0; i < time_tables_.size(); i++) {
		cout << time_tables_[i].table_.size() << endl;
	}
	system("PAUSE");*/
	for (int i = 0; i < time_tables_.size(); i++) {
		time_tables_[i].Modify(teachers_);
	}
}

void Schedule::Mutate(double mp) {
	for (int i = 0; i < time_tables_.size(); i++) {
		time_tables_[i].Mutate(mp, teachers_);
	}

}

void Schedule::Cross(Schedule &another, double cp) {
	for (int i= 0; i < time_tables_.size(); i++) {
		time_tables_[i].Cross(another.time_tables_[i], cp, teachers_);
	}
}

void Schedule::CalFitness(int mxreward) {
	fitness_ = 1.0 / (double)(1 + crash_);
	if (crash_ <= 0)fitness_ = (double)reward_ / mxreward;
}