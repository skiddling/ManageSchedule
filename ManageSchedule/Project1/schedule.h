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
	vector<Course> couque_;
	int outtime_;
	int crash_;
	const double mxpocross_ = 0.2;

	Schedule();
	Schedule(vector<Course> couque, vector<Teacher> teachers, 
		vector<TimeTable> timetables, vector<ClassUnit> clsque);
	//copy construction
	Schedule(const Schedule& s);
	Schedule& operator=(const Schedule& s);
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	bool init();
	void UpdatePtrs();//ָ�����
	
private:
	default_random_engine e_;
	void CalFitness();
	void Cross();
	void Modify();
	//void UpdatePtrs();

	void SwapClsUnit(ClassUnit& firstcls);
	//flag = 1��ʾ����modify����ʹ�ã�0��ʾֻ����cross����ʹ��
	bool UnionClsUnits(bool flag, ClassUnit& firstcls, pair<int, int> timedelta);

	//flag = 1��ʾ����modify����ʹ�ã�0��ʾֻ����cross����ʹ��
	bool AddUnitPtrIntoVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
		int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	//flag = 1��ʾ����modify����ʹ�ã�0��ʾֻ����cross����ʹ��
	bool PutInSetVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
		int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	bool CanBeSwap(bool flag, ClassUnit* c, pair<int, int> cperiod, ClassUnit* target, pair<int, int> tperiod);
	void SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap);
	void NeedToSwap(ClassUnit& firstcls);
};