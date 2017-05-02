#include "ClassUnit.h"
#include "timetable.h"

string ClassUnit::GetTeaName() {
	return teaname_;
}

string ClassUnit::GetCouName() {
	return couname_;
}

int ClassUnit::GetDuration() {
	return duration_;
}

bool ClassUnit::GetType() {
	return type_;
}

void ClassUnit::PutIntoTable(int day, int period) {
	hasbeenput_ = true;
	stime_.first = day;
	stime_.second = period;
	for (auto i = 0; i < duration_; i++) {
		ttbptr_->roomtable_[day][period] = this;
	}
	if (unioclsid_.size()) {
		for (auto i = 0; i < unioncls_.size(); i++) {
			if (unioncls_[i]->hasbeenput_ == false) {
				unioncls_[i]->PutIntoTable(day, period);
			}
		}
	}
}
