#pragma once
#include "teacher.h"

class TimeTable;

class ClassUnit
{
public:
	//data get from datebase
	int secionno_;//这节课的序号
	long long pkcombinateclassgroup_;//合班的组号，一起合班的课当做一个组有一个共同的组号
	long long pkevensection_;//连堂的组号，几个节次合在一起的
	long long dbid_;//数据库当中的教学班的id,也就是pkclasscocurse的值

	ClassUnit(TimeTable* ttb = nullptr, Teacher* teacher = nullptr, Course* couptr = nullptr);
	ClassUnit();
	map<string, void*> maptrs_;//用来做反射用的指针表
	Course course_;//这节是什么课
	Course* couptr_;
	Teacher* teacher_;//这节课的任课老师的指针
	pair<int, int> stime_ = make_pair(-1, -1);//具体这节课的起始时间
	TimeTable* ttbptr_;//指向自己所在课表的指针，这个是要在赋值的时候修改的
	bool hasbeenput_;//表示这节课是否已经被放到课表当中，用于课表初始化
	ClassUnit** headptr_;//用来标示是否是连堂课的第一节，指针内容是ttb当中指向第一节这个课的指针
	int type_;//这节课的类型，1表示普通，0表示辅助，2表示是普通类型的连堂课，但是辅助类型不能和他一起在同一天出现
	int duration_;//课时长度

	//vector<int> unioncls_;//合班课程，数值都是指向同一个schedule当中的下标
	vector<int> union_cls_index_;//合班课程，数值都是指向同一个schedule当中的下标
	vector<ClassUnit*> unioncls_;//合班课程，数值都是指向同一个schedule当中的指针
	vector<int> unioclsid_;//合班课程id
	pair<int, int> headunit_;

	//这个集合是最核心的两个集合
	set<pair<int, int>> canntbeput_;//具体之这节课不能安排时间
	set<pair<int, int>> canbeput_;//与canntbeput是补集
	
	//方便课表初始化操作
	bool operator < (const ClassUnit c) {
		if (duration_ + union_cls_index_.size() != c.duration_ + c.union_cls_index_.size())
			return duration_ + union_cls_index_.size() > c.duration_ + c.union_cls_index_.size();
		/*else if (unioclsid_.size() != c.unioclsid_.size())
			return unioclsid_.size() > c.unioclsid_.size();*/
		else if (canntbeput_.size() != c.canntbeput_.size())
			return canntbeput_.size() > c.canntbeput_.size();
	}
	
	string GetTeaName();
	string GetCouName();
	int GetDuration();
	bool GetType();
	bool PutIntoTable(int day, int period, bool flag);
	int CalFitness();
	int GetTeacherIdInVec();
	int GetTimeTableIdInVec();
	vector<pair<int, int>> GetRandAvailTime();
	bool CheckPeriod(pair<int, int> per);
	void ChangeTime(pair<int, int> period);
	//初始化的时候检查这个时间段是否有空
	bool CheckTimeEmpty(int d, int p);
	
private:
	default_random_engine e_;
	string teaname_, couname_;
	int cuid_;//该节课的id
	

	void GetRandSet(vector<pair<int, int>>& randset);
};
