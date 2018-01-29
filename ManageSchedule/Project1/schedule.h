#pragma once
#include "timetable.h"
#include "interruptflag.h"
//#include "InterruptibleThread.h"
//#include "InterruptibleThread.h"

class InterruptibleThread;

class Schedule
{
public:
	vector<ClassUnit> clsque_;//ȫ���ڴεĿ�
	vector<Teacher> teachers_;//���еĽ�ʦ��ʵ��
	vector<TimeTable> timetables_;//���еİ༶�α��൱����һ��ָ������
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
	void UpdatePtrs();//ָ�����

	set<pair<int, int>> allunits_;//��������е�ʱ��
	
private:
	default_random_engine e_;
	void CalFitness();
	void Cross();
	void Modify();
	//void UpdatePtrs();

	//void SwapClsUnit(ClassUnit& firstcls);
	//flag = 1��ʾ����modify����ʹ�ã�0��ʾֻ����cross����ʹ��
	//bool UnionClsUnits(bool flag, ClassUnit& firstcls, pair<int, int> timedelta);

	////flag = 1��ʾ����modify����ʹ�ã�0��ʾֻ����cross����ʹ��
	//bool AddUnitPtrIntoVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
	//	int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	////flag = 1��ʾ����modify����ʹ�ã�0��ʾֻ����cross����ʹ��
	//bool PutInSetVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta, 
	//	int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab);

	//bool CanBeSwap(bool flag, ClassUnit* c, pair<int, int> cperiod, ClassUnit* target, pair<int, int> tperiod);

	//void SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap);
	void NeedToSwap(ClassUnit& firstcls, int tag);//tag 0 ��ʾ��cross��1��ʾ��modify
	void GetUnitsAvailableTime();//������нڴε����ŵ�ʱ�䣬��ʵ�ǲ����ŵĲ���
	void GetTeachTime();//ÿ����ʦ�������Ӧ�ĸ����Ͽ�ʱ��
	void GetRoomCourseTime();//ÿ�����Ҷ������Ӧ�Ŀ�Ŀ�Ͽ�ʱ��
	pair<bool, vector<vector<Node>>> GetUnionUnitsVec(ClassUnit* firstunitptr, pair<int, int> delta, int tag);//tag��ʾcross����modify
	void SwapUnits(vector<vector<ClassUnit**>>& units);
	pair<int, int> GetOpposeTime(int pos, pair<int, int> tim, pair<int, int> delta);
	bool CheckPutIntoVec(TimeTable* tbptr, pair<int, int> origin, int pos, vector<vector<Node>>&vec, set<Node>& unitset, pair<int, int> delta, int tag);
	bool CheckRoomIllegal(TimeTable* tbptr, pair<int, int> origin, pair<int ,int> opt, int cnt);
	void SwapUnitsVec(vector<vector<Node>> vec);
	void UpdateUnitPrt(TimeTable* ttbptr, pair<int, int> ori, pair<int, int> opt);
};