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
	const double mxpocross_ = 0.2;

	Schedule();
	//copy construction
	Schedule(const Schedule& s);
	Schedule& operator=(const Schedule& s);
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	void init();
	
private:
	default_random_engine e_;
	void CalFitness();
	void Cross();
	void Modify();
	void UpdatePtrs();

	void SwapClsUnit(ClassUnit& firstcls);
	bool UnionClsUnits(ClassUnit& firstcls, pair<int, int> timedelta);

	bool AddUnitPtrIntoVec(ClassUnit** cptr, pair<int, int> timedelta, 
		int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	bool PutInSetVec(ClassUnit** cptr, pair<int, int> timedelta, 
		int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	bool CanBeSwap(ClassUnit c, ClassUnit target);
};