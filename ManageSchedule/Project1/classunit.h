#pragma once
#include "course.h"

class TimeTable;

class ClassUnit
{
public:
	string GetTeaName();
	string GetCouName();
	int GetDuration();
	bool GetType();
	pair<int, int> stime_;//具体这节课的起始时间
	TimeTable* ttbptr_;//指向自己所在课表的指针
	void PutIntoTable(int day, int period);
	bool hasbeenput_;//表示这节课是否已经被放到课表当中，用于课表初始化
	
	//方便课表初始化操作
	bool operator < (const ClassUnit c) {
		if (duration_ != c.duration_)
			return duration_ > c.duration_;
		else if (unioclsid_.size() != c.unioclsid_.size())
			return unioclsid_.size() > c.unioclsid_.size();
		else if (canntbeput_.size() != c.canntbeput_.size())
			return canntbeput_.size() > c.canntbeput_.size();
	}

private:
	string teaname_, couname_;
	int duration_;//课时长度
	int type_;//这节课的类型，1表示普通，0表示辅助，2表示是普通类型的连堂课，但是辅助类型不能和他一起在同一天出现
	int cuid_;//该节课的id
	vector<ClassUnit*> unioncls_;//合班课程
	vector<int> unioclsid_;//合班课程id
	pair<int, int> headunit_;
	set<pair<int, int>> canntbeput_;//具体之这节课不能安排时间
};
