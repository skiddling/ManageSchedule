#include "teacher.h"

int Teacher::GetClsNumInPeriod(pair<int, int> period) {
	return clsinperiod_[period];
}

int Teacher::HasNormalClsInDay(int d) {
	return normalappear_[d];
}

void Teacher::AddClsInPeriod(int day, int period) {
	pair<int, int> tmp{ day, period };
	if (availperiods_.find(tmp) != availperiods_.end()) {
		availperiods_.erase(availperiods_.find(tmp));
		clsinperiod_[tmp] = 1;
	}
	else clsinperiod_[tmp]++;
}


