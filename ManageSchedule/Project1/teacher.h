#pragma once
#include "course.h"

class Teacher
{
public:
	int teacherid_;
	int idinque_;//在schedule的teachers当中的序号
	vector<Course> couque_;//教学的科目
	vector<ClassUnit*> clsque_;//属于这个老师所有的课的队列
	map<TimeTable, vector<ClassUnit*>> clstabque_;//这个老师的所有上的课
	Teacher();
	vector<vector<int>> unitstimecount_;//节次时间的计数
	vector<int> dayavailable_;//有空的天
	vector<int> normalappear_;//普通类型出现
	
	int GetClsNumInPeriod(pair<int, int> period);//返回某一时间段这个老师上多少节课
	int HasNormalClsInDay(int d);//判断这天是否存在普通类型的课
	void AddClsInPeriod(int day, int period);
	//target是目标时间,cls是当前这节课，这表示检查当前这节课和这个目标时间的关系
	bool CheckUnit(ClassUnit* cls, pair<int, int> target);
	void UpdateUnit(ClassUnit* cls, pair<int, int> target);
	set<pair<int, int>> GetAvailPeriods();

	bool operator == (const Teacher& a) {
		if (a.teacherid_ == teacherid_)return true;
		return false;
	}
private:
	map<pair<int, int>, int> clsinperiod_;//该时间段上课次数
	set<pair<int, int>> availperiods_;//教师有空时间
};