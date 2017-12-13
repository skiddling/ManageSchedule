#pragma once
#include "timetable.h"
#include "interruptflag.h"
#include "InterruptibleThread.h"
//#include "InterruptibleThread.h"

class Schedule
{
public:
	vector<Teacher> teachers_;//所有的教师的实体
	vector<ClassUnit> clsque_;//全部节次的课
	vector<TimeTable> timetables_;//所有的班级课表，相当于是一个指针容器
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
	void UpdatePtrs();//指针深拷贝
	
private:
	default_random_engine e_;
	void CalFitness();
	void Cross();
	void Modify();
	//void UpdatePtrs();

	void SwapClsUnit(ClassUnit& firstcls);
	//flag = 1表示是在modify当中使用，0表示只是在cross当中使用
	bool UnionClsUnits(bool flag, ClassUnit& firstcls, pair<int, int> timedelta);

	//flag = 1表示是在modify当中使用，0表示只是在cross当中使用
	bool AddUnitPtrIntoVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
		int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	//flag = 1表示是在modify当中使用，0表示只是在cross当中使用
	bool PutInSetVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
		int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	bool CanBeSwap(bool flag, ClassUnit* c, pair<int, int> cperiod, ClassUnit* target, pair<int, int> tperiod);
	void SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap);
	void NeedToSwap(ClassUnit& firstcls);
};