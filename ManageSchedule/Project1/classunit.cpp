#include "ClassUnit.h"
#include "timetable.h"

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

void ClassUnit::PutIntoTable(int day, int period) {
	hasbeenput_ = true;
	stime_.first = day;
	stime_.second = period;
	if (type_ == 1)teacher_->normalappear_[day] = 1;
	headptr_ = &(ttbptr_->roomtable_[day][period]);
	for (auto i = 0; i < duration_; i++) {
		//���½�����Ϣ
		ttbptr_->roomtable_[day][period + i] = this;
		//���½�ʦ��Ϣ
		teacher_->AddClsInPeriod(day, period + i);
	}
	//���úϰ��
	if (unioclsid_.size()) {
		for (auto i = 0; i < unioncls_.size(); i++) {
			if (unioncls_[i]->hasbeenput_ == false) {
				unioncls_[i]->PutIntoTable(day, period);
			}
		}
	}
}

int ClassUnit::CalFitness() {
	pair<int, int> tmp = stime_;
	for (auto i = 0; i < duration_; i++) {
		//1.�������˲��ܷŵ�ʱ���
		if (canntbeput_.find(tmp) != canntbeput_.end())
			return 1;
		//2.���ŵ�ʱ����ʦ�г�ͻ
		if (teacher_->GetClsNumInPeriod(tmp) > 1)
			return 1;
		tmp.second++;
	}
	//3.�������͵Ŀ�û�к���ͨ���͵Ŀη���ͬһ��
	if (type_ == 0 && teacher_->HasNormalClsInDay(stime_.first) == 0)
		return 1;
	return 0;
}

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

bool ClassUnit::CheckPeriod(pair<int, int> per) {
	if(canntbeput_.find(per) != canntbeput_.end()) return false;
	return true;
}

void ClassUnit::GetRandSet(vector<pair<int, int>>& randset) {
	int id, sz = randset.size();
	uniform_int_distribution<int> u(0, sz - 1);
	for (auto i = 0; i < sz; i++) {
		id = u(e_);
		if (id != i)swap(randset[i], randset[id]);
	}
}

