#include "schedule.h"

int flag;
mutex mtx;

Schedule::Schedule(){
	e_ = default_random_engine(time(NULL));
}

Schedule::Schedule(const Schedule& s):
	crash_(s.crash_), 
	outtime_(s.outtime_),
	timetables_(s.timetables_),
	clsque_(s.clsque_),
	teachers_(s.teachers_),
	e_(s.e_){
	//每个课表进行拷贝的时候需要进行相应的修改
	/*因为当前只有时间是准确的，对于classunit当中的ttbptr_
	和timetable_当中的所有的值都需要进行修正
	*/
	UpdatePtrs();
}

Schedule & Schedule::operator=(const Schedule & s){
	crash_ = s.crash_;
	outtime_ = s.outtime_;
	timetables_ = s.timetables_;
	clsque_ = s.clsque_;
	teachers_ = s.teachers_;
	e_ = s.e_;
	UpdatePtrs();
	return *this;
}

void Schedule::GetSchedule(InterruptibleThread * t, future<Schedule>* fut) {
	auto t1 = chrono::system_clock::now(), t2 = t1;
	chrono::duration<int, ratio<60, 1>> dur(outtime_);
	
	while (true) {
		t2 = chrono::system_clock::now();
		CalFitness();
		if (crash_ == 0) {
			break;
		}
		if (t2 - t1 > dur) {
			break;
		}
		try {
			interruption_point();
		}
		catch(const thread_interrupted& interrupt){
			break;
		}
		Cross();
		CalFitness();
		Modify();
	}
	if (flag) {
		mtx.lock();
		if (flag && crash_ == 0) {
			flag = 0;
			t->pro_ptr_->set_value(*this);
		}
		mtx.unlock();
	}
}

void Schedule::init() {
	UpdatePtrs();
	//2.然后安排每节课的时间
	for (auto& t : timetables_)
		t.Init();
}

void Schedule::CalFitness() {
	/*
	检查每节课是否都符合要求
	*/
	for (auto& c : clsque_)
		crash_ += c.CalFitness();
}

void Schedule::Cross() {
	/*
	这里比modify少判断教师上课情况，
	也就是这里只判断该节课自己的能否被安排的情况，
	*/
	double cp;
	uniform_real_distribution<double> u(0.0, 1.0);
	for (auto& c : clsque_) {
		cp = u(e_);
		if (cp < mxpocross_) {
			SwapClsUnit(c);
		}
	}

}

void Schedule::Modify() {

}

void Schedule::UpdatePtrs() {
	for (auto& c : clsque_) {
		//1.先将老师和自己的每节课进行挂钩
		teachers_[c.GetTeacherIdInVec()].clsque_.push_back(&c);
		c.teacher_ = &teachers_[c.GetTeacherIdInVec()];
		
		//2.把这个课和相应的上课教室挂钩
		timetables_[c.GetTimeTableIdInVec()].clsque_.push_back(&c);
		c.ttbptr_ = &timetables_[c.GetTimeTableIdInVec()];
	}
}

void Schedule::SwapClsUnit(ClassUnit& firstcls) {
	vector<pair<int, int>> canbeput = firstcls.GetRandAvailTime();
	if (canbeput.empty())return;
	//0用来装当前的节次组，1用来装将来的节次组
	vector<vector<ClassUnit*>> wait4swap{ 2 };
	wait4swap[0].push_back(&firstcls);
	vector<vector<ClassUnit*>::iterator> it;
	for (auto i = 0; i < 2; i++) {
		it.push_back(wait4swap[i].begin());
	}
	auto fun = [&]()-> bool{
		for (auto i = 0; i < 2; i++) {
			if (it[i] != wait4swap[i].end())
				return true;
		}
		return false;
	};
	set<ClassUnit*> clstab;
	while (fun()) {
		for (auto i = 0; i < 2; i++) {
			
		}
	}

}
