#include "teacher.h"
#include "classunit.h"
#include "timetable.h"

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

bool Teacher::CheckUnit(ClassUnit * cls, pair<int, int> period) {
	//auto period = cls->stime_;
	//根据节次的类型
	if (!cls->GetType()) {
		//辅助节次没有和主要节次放同一天
		if (!normalappear_[period.first])return false;
		//辅助节次和主要节次连在了一起
		auto day = period.first;
		auto before = period.second - 1;
		if (before > 0) {
			auto tea = (*(cls->ttbptr_->roomtable_[day][before])).teacher_;
			if(*tea == *(cls->teacher_))
				return false;
		}
		auto after = period.second + 1;
		if (after < cls->ttbptr_->periods_ - 1) {
			auto tea = (*(cls->ttbptr_->roomtable_[day][after])).teacher_;
			if(*tea == *(cls->teacher_))
				return false;
		}
	}
	else {
		//有两个主要节次放在同一天
		if (normalappear_[period.first] > 1)return false;
	}
	//检查老师是否同一个时间段上两节课
	for (auto i = 0; i < cls->GetDuration(); i++) {
		if (clsinperiod_.find(period) != clsinperiod_.end()) {
			if (clsinperiod_[period] > 1)return false;
		}
		period.second++;
	}
	return false;
}

void Teacher::UpdateUnit(ClassUnit* cls, pair<int, int> target) {
	//更新这节课的信息
	if (cls->GetType()) {
		normalappear_[cls->stime_.first]--;
		normalappear_[target.first]++;
	}
	if (clsinperiod_[cls->stime_] == 1) {
		clsinperiod_.erase(clsinperiod_.find(cls->stime_));
		availperiods_.insert(cls->stime_);
	}
	if (clsinperiod_.find(target) == clsinperiod_.end()) {
		clsinperiod_[target] = 1;
		availperiods_.erase(availperiods_.find(target));
	}
	else clsinperiod_[target]++;
}

set<pair<int, int>> Teacher::GetAvailPeriods() {
	return availperiods_;
}
