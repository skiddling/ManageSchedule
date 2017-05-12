#include "schedule.h"

int flag;
mutex mtx;

Schedule::Schedule(const Schedule& s):
	crash_(s.crash_), 
	outtime_(s.outtime_),
	timetables_(s.timetables_),
	clsque_(s.clsque_),
	teachers_(s.teachers_){
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
