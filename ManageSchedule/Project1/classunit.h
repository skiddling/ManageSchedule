#pragma once
#include "teacher.h"

class ClassUnit
{
public:
	ClassUnit();
	Teacher* teacher_;//这节课的任课老师的指针
	pair<int, int> stime_;//具体这节课的起始时间
	TimeTable* ttbptr_;//指向自己所在课表的指针，这个是要在赋值的时候修改的
	bool hasbeenput_;//表示这节课是否已经被放到课表当中，用于课表初始化
	ClassUnit** headptr_;//用来标示是否是连堂课的第一节，指针内容是ttb当中指向第一节这个课的指针

	vector<ClassUnit*> unioncls_;//合班课程
	vector<int> unioclsid_;//合班课程id
	pair<int, int> headunit_;
	
	//方便课表初始化操作
	bool operator < (const ClassUnit c) {
		if (duration_ != c.duration_)
			return duration_ > c.duration_;
		else if (unioclsid_.size() != c.unioclsid_.size())
			return unioclsid_.size() > c.unioclsid_.size();
		else if (canntbeput_.size() != c.canntbeput_.size())
			return canntbeput_.size() > c.canntbeput_.size();
	}
	
	string GetTeaName();
	string GetCouName();
	int GetDuration();
	bool GetType();
	void PutIntoTable(int day, int period);
	int CalFitness();
	int GetTeacherIdInVec();
	int GetTimeTableIdInVec();
	vector<pair<int, int>> GetRandAvailTime();
	bool CheckPeriod(pair<int, int> per);

private:
	default_random_engine e_;
	string teaname_, couname_;
	int duration_;//课时长度
	int type_;//这节课的类型，1表示普通，0表示辅助，2表示是普通类型的连堂课，但是辅助类型不能和他一起在同一天出现
	int cuid_;//该节课的id
	set<pair<int, int>> canntbeput_;//具体之这节课不能安排时间
	set<pair<int, int>> canbeput_;//与canntbeput是补集

	void GetRandSet(vector<pair<int, int>>& randset);
};
