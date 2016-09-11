#include "teacher.h"

Teacher::Teacher() {
	in_school_ = 0;
	teacher_name_ = "";
	id_ = -1;
}

Teacher::Teacher(int teacher_id, string teacher_name) {
	id_ = teacher_id;
	teacher_name_ = teacher_name;
}

void Teacher::InitAvailable(int daysperweek, int periodperday) {
	num_of_period = vector<vector<int> >(daysperweek, vector<int>(2, 0));
	for (int x = 0; x < daysperweek; x++) {
		for (int y = 0; y < periodperday; y++) {
			pair<int, int> pt = make_pair(x, y);
			//if(cannt_time_.find(pt) == cannt_time_.end()) available_time[pt] = 1;
			available_time[pt] = 1;
		}
	}
}