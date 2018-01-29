#include "ClassUnit.h"
#include "timetable.h"
#include "schedule.h"

ClassUnit::ClassUnit(TimeTable* ttb, Teacher* teacher, Course* couptr):
	ttbptr_(ttb), teacher_(teacher), couptr_(couptr){
	//在表当中注册相应的地址值
	maptrs_["Course"] = static_cast<void*>(&couptr_);
	maptrs_["Teacher"] = static_cast<void*>(&teacher_);
	maptrs_["TimeTable"] = static_cast<void*>(&ttbptr_);
	e_ = default_random_engine(time(NULL));
}

ClassUnit::ClassUnit() {
	e_ = default_random_engine(time(NULL));
}

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

bool ClassUnit::PutIntoTable(int day, int period, bool flag) {
	//新版本的操作
	//最后一个flag是用来判断合班的时候用的是否要再检查一遍的作用,true表示要检查，false不用
	//1.先判断是不是合班或者是连堂，在新版本的设计当中连堂和合班是不同时发生的，所以分开判断
	if (union_cls_index_.size()) {
		if (flag) {
			for (auto& ucptr : unioncls_) {
				if (!ucptr->CheckTimeEmpty(day, period))return false;
			}
			//每个合班课都有空
			for (auto& ucptr : unioncls_) {
				ucptr->PutIntoTable(day, period, false);
			}
		}
	}
	//判断是否是连堂课
	else if (duration_ > 1) {
		for (auto i = 1; i < duration_; i++) {
			if (ttbptr_->roomtable_[day][period + i] != nullptr)
				return false;
		}
		for (auto i = 1; i < duration_; i++) {
			ttbptr_->roomtable_[day][period + i] = this;
			//teacher_->AddClsInPeriod(day, period + i);
		}
		headptr_ = &(ttbptr_->roomtable_[day][period]);
	}
	//做更新操作
	ttbptr_->roomtable_[day][period] = this;
	stime_ = make_pair(day, period);
	hasbeenput_ = true;
	//teacher_->AddClsInPeriod(day, period);
	return true;
}

//int ClassUnit::CalFitness() {
//	pair<int, int> tmp = stime_;
//	for (auto i = 0; i < duration_; i++) {
//		//1.被放在了不能放的时间段
//		if (canntbeput_.find(tmp) != canntbeput_.end())
//			return 1;
//		//2.安排的时间老师有冲突
//		if (teacher_->GetClsNumInPeriod(tmp) > 1)
//			return 1;
//		tmp.second++;
//	}
//	//3.辅助类型的课没有和普通类型的课放在同一天
//	if (type_ == 0 && teacher_->HasNormalClsInDay(stime_.first) == 0)
//		return 1;
//	return 0;
//}

int ClassUnit::GetTeacherIdInVec() {
	return teacher_->idinque_;
}

int ClassUnit::GetTimeTableIdInVec() {
	return ttbptr_->roomid_;
}

vector<pair<int, int>> ClassUnit::GetRandAvailTime() {
	set<pair<int, int>> tmp = canbeput_;
	pair<int, int> now{stime_.first, stime_.second};
	if (tmp.find(now) != tmp.end()) {
		tmp.erase(tmp.find(now));
	}
	vector<pair<int, int>> vec;
	if (!tmp.empty()) {
		for (auto p : tmp) {
			vec.push_back(p);
		}
		GetRandSet(vec);
	}
	return vec;
}

vector<pair<int, int>> ClassUnit::GetRandTime() {
	set<pair<int, int>> tmp = allunits_;
	tmp.erase(make_pair(stime_.first, stime_.second));
	vector<pair<int, int>> vec;
	if (!tmp.empty()) {
		for (auto p : tmp) {
			vec.push_back(p);
		}
		GetRandSet(vec);
	}
	return vec;
}

bool ClassUnit::CheckPeriod(pair<int, int> per) {
	if(canntbeput_.find(per) != canntbeput_.end()) return false;
	return true;
}

//void ClassUnit::ChangeTime(pair<int, int> period) {
//	//把当前这节课的时间换到目标时间当中
//	teacher_->UpdateUnit(this, period);
//	stime_ = period;
//	headptr_ = &(ttbptr_->roomtable_[period.first][period.second]);
//}

bool ClassUnit::CheckTimeEmpty(int d, int p) {
	if (ttbptr_->roomtable_[d][p] == nullptr)return true;
	return false;
}

void ClassUnit::UpdateRoomPtr() {
	for (auto i = 0; i < duration_; i++) {
		ttbptr_->roomtable_[stime_.first][stime_.second + i] = this;
	}
}

//要考虑到cross还是modify操作
//cross 0 ， modify 1
//还要考虑到是否是连堂课
//如果是连堂课，那么只会对stime进行判断
//以下所有情况都是除掉预排课的
//cross当中不进行时间上的判断
//modify当中连堂课可以不进行时间上的判断
//返回true表示非法，false表示合法
bool ClassUnit::CheckTimeIllegal(pair<int, int> tim, pair<int, int> opt, int tag) {
	//特判这个是预排的课
	if (preput_) {
		if (opt != pretime_)return true;
		return false;
	}
	if (tim != stime_) {
		//非连堂课的第一节课
		return false;
	}
	//时间不合符空间
	if (opt.first < 0 || opt.first >= ttbptr_->days_)return true;
	if (opt.second < 0 || opt.second >= ttbptr_->periods_)return true;
	if (tag == 0) {
		//cross情况
		return false;
	}
	//modify情况，需要考虑是否合法
	//if (canntbeput_.find(make_pair(tim.first, tim.second)) != canntbeput_.end())return true;
	if (canntbeput_.find(make_pair(opt.first, opt.second)) != canntbeput_.end())return true;

	//判断是否对换的是同一个课
	int val = 0;
	if (ttbptr_->roomtable_[opt.first][opt.second] != nullptr) {
		if ((ttbptr_->roomtable_[opt.first][opt.second]->couptr_->dbid_) == dbid_)
			val = 1;
	}
	//这个课在当天已经上过了
	if ((ttbptr_->course_time_)[couptr_->dbid_][opt.first] > val)return true;
	//	//这个老师同时上两节课
	if (teacher_->teach_time_[opt.first][opt.second] > val)return true;
	return false;
}

ClassUnit * ClassUnit::GetTargetUnit(pair<int, int> tim) {
	return ttbptr_->roomtable_[tim.first][tim.second];
}

ClassUnit ** ClassUnit::GetTimeTablePtr(pair<int, int> tim) {
	return &(ttbptr_->roomtable_[tim.first][tim.second]);
}

int ClassUnit::GetCrash() {
	//return CheckTimeCrash() ? 1 : 0;
	if (CheckTimeCrash()) {
		crash_ = 1;
	}
	else crash_ = 0;
	return crash_;
}

bool ClassUnit::CheckTimeCrash() {
	//特判这个是预排的课
	if (preput_) {
		if (stime_ != pretime_)return true;
		return false;
	}
	//在不能排的时间当中
	//if (canntbeput_.find(make_pair(stime_.first, stime_.second)) != canntbeput_.end())return true;
	if (canntbeput_.find(stime_) != canntbeput_.end())return true;
	//这个课一天上了两次
	if ((ttbptr_->course_time_)[couptr_->dbid_][stime_.first] > 1)return true;
	//这个老师同时上两节课
	for (auto i = 0; i < duration_; i++) {
		if (teacher_->teach_time_[stime_.first][stime_.second + i] > 1)return true;
	}
	return false;
}

void ClassUnit::GetRandSet(vector<pair<int, int>>& randset) {
	int id, sz = randset.size();
	uniform_int_distribution<int> u(0, sz - 1);
	for (auto i = 0; i < sz; i++) {
		id = u(e_);
		if (id != i)swap(randset[i], randset[id]);
	}
}

