#include "schedule.h"

int flag;
mutex mtx;

void Schedule::GetSchedule(InterruptibleThread * t, future<Schedule>* fut) {
	auto t1 = chrono::system_clock::now(), t2 = t1;
	chrono::duration<int, ratio<60, 1>> dur(outtime_);
	
	while (true) {
		t2 = chrono::system_clock::now();
		CalFitness();
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
		if (flag) {
			flag = 0;
			t->pro_ptr_->set_value(*this);
		}
		mtx.unlock();
	}
}

void Schedule::UpdateFromRes() {
}

void Schedule::init() {
	for (auto& t : timetables_)
		t.Init();
}

void Schedule::CalFitness() {
}

void Schedule::Cross() {

}

void Schedule::Modify() {

}
