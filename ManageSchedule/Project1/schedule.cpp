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
	//ÿ���α���п�����ʱ����Ҫ������Ӧ���޸�
	/*��Ϊ��ǰֻ��ʱ����׼ȷ�ģ�����classunit���е�ttbptr_
	��timetable_���е����е�ֵ����Ҫ��������
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
	//2.Ȼ����ÿ�ڿε�ʱ��
	for (auto& t : timetables_)
		t.Init();
}

void Schedule::CalFitness() {
	/*
	���ÿ�ڿ��Ƿ񶼷���Ҫ��
	*/
	for (auto& c : clsque_)
		crash_ += c.CalFitness();
}

void Schedule::Cross() {
	/*
	�����modify���жϽ�ʦ�Ͽ������
	Ҳ��������ֻ�жϸýڿ��Լ����ܷ񱻰��ŵ������
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
		//1.�Ƚ���ʦ���Լ���ÿ�ڿν��йҹ�
		teachers_[c.GetTeacherIdInVec()].clsque_.push_back(&c);
		c.teacher_ = &teachers_[c.GetTeacherIdInVec()];
		
		//2.������κ���Ӧ���Ͽν��ҹҹ�
		timetables_[c.GetTimeTableIdInVec()].clsque_.push_back(&c);
		c.ttbptr_ = &timetables_[c.GetTimeTableIdInVec()];
	}
}

void Schedule::SwapClsUnit(ClassUnit& firstcls) {
	vector<pair<int, int>> canbeput = firstcls.GetRandAvailTime();
	if (canbeput.empty())return;
	//0����װ��ǰ�Ľڴ��飬1����װ�����Ľڴ���
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
