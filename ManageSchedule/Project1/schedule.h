#pragma once
#include "timetable.h"
#include "interruptflag.h"
//#include "InterruptibleThread.h"
//#include "InterruptibleThread.h"

class InterruptibleThread;

class Schedule
{
public:
	vector<ClassUnit> clsque_;//全部节次的课
	vector<Teacher> teachers_;//所有的教师的实体
	vector<TimeTable> timetables_;//所有的班级课表，相当于是一个指针容器
	vector<Course> couque_;
	int outtime_ = 1;
	int crash_;
	const double mxpocross_ = 0;

	Schedule();
	Schedule(vector<Course> couque, vector<Teacher> teachers, 
		vector<TimeTable> timetables, vector<ClassUnit> clsque);
	//copy construction
	Schedule(const Schedule& s);
	Schedule& operator=(const Schedule& s);
	void GetSchedule(InterruptibleThread* t, future<Schedule>* fut);
	bool init();
	void UpdatePtrs();//指针深拷贝

	set<pair<int, int>> allunits_;//存放了所有的时间
	
private:
	default_random_engine e_;
	void CalFitness();
	void Cross();
	void Modify();
	//void UpdatePtrs();

	//void SwapClsUnit(ClassUnit& firstcls);
	//flag = 1表示是在modify当中使用，0表示只是在cross当中使用
	//bool UnionClsUnits(bool flag, ClassUnit& firstcls, pair<int, int> timedelta);

	////flag = 1表示是在modify当中使用，0表示只是在cross当中使用
	//bool AddUnitPtrIntoVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
	//	int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	////flag = 1表示是在modify当中使用，0表示只是在cross当中使用
	//bool PutInSetVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
	//	int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	//bool CanBeSwap(bool flag, ClassUnit* c, pair<int, int> cperiod, ClassUnit* target, pair<int, int> tperiod);

	//void SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap);
	void NeedToSwap(ClassUnit& firstcls, int tag);//tag 0 表示是cross，1表示是modify
	void GetUnitsAvailableTime();//获得所有节次的能排的时间，其实是不能排的补集
	void GetTeachTime();//每个老师都获得相应的各自上课时间
	void GetRoomCourseTime();//每个教室都获得相应的科目上课时间
	pair<bool, vector<vector<Node>>> GetUnionUnitsVec(ClassUnit* firstunitptr, pair<int, int> delta, int tag);//tag表示cross还是modify
	void SwapUnits(vector<vector<ClassUnit**>>& units);
	pair<int, int> GetOpposeTime(int pos, pair<int, int> tim, pair<int, int> delta);
	bool CheckPutIntoVec(TimeTable* tbptr, pair<int, int> origin, int pos, vector<vector<Node>>&vec, set<Node>& unitset, pair<int, int> delta, int tag);
	bool CheckRoomIllegal(TimeTable* tbptr, pair<int, int> origin, pair<int ,int> opt, int cnt);
	void SwapUnitsVec(vector<vector<Node>> vec);
	void UpdateUnitPrt(TimeTable* ttbptr, pair<int, int> ori, pair<int, int> opt);
};