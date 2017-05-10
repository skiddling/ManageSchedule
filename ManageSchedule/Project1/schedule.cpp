#include "schedule.h"

int flag;
mutex mtx;

Schedule::Schedule(const Schedule& s):
	crash_(s.crash_), 
	outtime_(s.outtime_),
	timetables_(s.timetables_){
	//ÿ���α���п�����ʱ����Ҫ������Ӧ���޸�
	/*��Ϊ��ǰֻ��ʱ����׼ȷ�ģ�����classunit���е�ttbptr_
	��timetable_���е����е�ֵ����Ҫ��������
	*/
	//step1.�޸�clsque����ÿ��Ԫ�ص�ttbptr_
	for (auto& tb : timetables_)
		tb.UpdatePtrs();
}

Schedule & Schedule::operator=(const Schedule & s){
	crash_ = s.crash_;
	outtime_ = s.outtime_;
	timetables_ = s.timetables_;
	// TODO: �ڴ˴����� return ���
	for (auto& tb : timetables_)
		tb.UpdatePtrs();
	return *this;
}

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
