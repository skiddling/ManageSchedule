#include "ClassUnit.h"
#include "timetable.h"
#include "schedule.h"

ClassUnit::ClassUnit(TimeTable* ttb, Teacher* teacher, Course* couptr):
	ttbptr_(ttb), teacher_(teacher), couptr_(couptr){
	//�ڱ���ע����Ӧ�ĵ�ֵַ
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
	//�°汾�Ĳ���
	//���һ��flag�������жϺϰ��ʱ���õ��Ƿ�Ҫ�ټ��һ�������,true��ʾҪ��飬false����
	//1.���ж��ǲ��Ǻϰ���������ã����°汾����Ƶ������úͺϰ��ǲ�ͬʱ�����ģ����Էֿ��ж�
	if (union_cls_index_.size()) {
		if (flag) {
			for (auto& ucptr : unioncls_) {
				if (!ucptr->CheckTimeEmpty(day, period))return false;
			}
			//ÿ���ϰ�ζ��п�
			for (auto& ucptr : unioncls_) {
				ucptr->PutIntoTable(day, period, false);
			}
		}
	}
	//�ж��Ƿ������ÿ�
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
	//�����²���
	ttbptr_->roomtable_[day][period] = this;
	stime_ = make_pair(day, period);
	hasbeenput_ = true;
	//teacher_->AddClsInPeriod(day, period);
	return true;
}

//int ClassUnit::CalFitness() {
//	pair<int, int> tmp = stime_;
//	for (auto i = 0; i < duration_; i++) {
//		//1.�������˲��ܷŵ�ʱ���
//		if (canntbeput_.find(tmp) != canntbeput_.end())
//			return 1;
//		//2.���ŵ�ʱ����ʦ�г�ͻ
//		if (teacher_->GetClsNumInPeriod(tmp) > 1)
//			return 1;
//		tmp.second++;
//	}
//	//3.�������͵Ŀ�û�к���ͨ���͵Ŀη���ͬһ��
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
//	//�ѵ�ǰ��ڿε�ʱ�任��Ŀ��ʱ�䵱��
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

//Ҫ���ǵ�cross����modify����
//cross 0 �� modify 1
//��Ҫ���ǵ��Ƿ������ÿ�
//��������ÿΣ���ôֻ���stime�����ж�
//��������������ǳ���Ԥ�ſε�
//cross���в�����ʱ���ϵ��ж�
//modify�������ÿο��Բ�����ʱ���ϵ��ж�
//����true��ʾ�Ƿ���false��ʾ�Ϸ�
bool ClassUnit::CheckTimeIllegal(pair<int, int> tim, pair<int, int> opt, int tag) {
	//���������Ԥ�ŵĿ�
	if (preput_) {
		if (opt != pretime_)return true;
		return false;
	}
	if (tim != stime_) {
		//�����ÿεĵ�һ�ڿ�
		return false;
	}
	//ʱ�䲻�Ϸ��ռ�
	if (opt.first < 0 || opt.first >= ttbptr_->days_)return true;
	if (opt.second < 0 || opt.second >= ttbptr_->periods_)return true;
	if (tag == 0) {
		//cross���
		return false;
	}
	//modify�������Ҫ�����Ƿ�Ϸ�
	//if (canntbeput_.find(make_pair(tim.first, tim.second)) != canntbeput_.end())return true;
	if (canntbeput_.find(make_pair(opt.first, opt.second)) != canntbeput_.end())return true;

	//�ж��Ƿ�Ի�����ͬһ����
	int val = 0;
	if (ttbptr_->roomtable_[opt.first][opt.second] != nullptr) {
		if ((ttbptr_->roomtable_[opt.first][opt.second]->couptr_->dbid_) == dbid_)
			val = 1;
	}
	//������ڵ����Ѿ��Ϲ���
	if ((ttbptr_->course_time_)[couptr_->dbid_][opt.first] > val)return true;
	//	//�����ʦͬʱ�����ڿ�
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
	//���������Ԥ�ŵĿ�
	if (preput_) {
		if (stime_ != pretime_)return true;
		return false;
	}
	//�ڲ����ŵ�ʱ�䵱��
	//if (canntbeput_.find(make_pair(stime_.first, stime_.second)) != canntbeput_.end())return true;
	if (canntbeput_.find(stime_) != canntbeput_.end())return true;
	//�����һ����������
	if ((ttbptr_->course_time_)[couptr_->dbid_][stime_.first] > 1)return true;
	//�����ʦͬʱ�����ڿ�
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

