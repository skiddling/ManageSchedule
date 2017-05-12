#pragma once
#include "timetable.h"
#include "interruptflag.h"
#include "InterruptibleThread.h"
//#include "InterruptibleThread.h"

class Schedule
{
public:
	vector<Teacher> teachers_;//���еĽ�ʦ��ʵ��
	vector<ClassUnit> clsque_;//ȫ���ڴεĿ�
	vector<TimeTable> timetables_;//���еİ༶�α��൱����һ��ָ������
	int outtime_;
	int crash_;

	Schedule() = default;
	//copy construction
	Schedule(const Schedule& s);
	Schedule& operator=(const Schedule& s);
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	void init();
	
private:
	void CalFitness();
	void Cross();
	void Modify();
	void UpdatePtrs();
};