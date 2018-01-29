#pragma once
#include "course.h"

class Teacher
{
public:
	//data from database
	string dbname_;//教师名字
	long long pkcourse_id_;
	long long dbteacher_id_;
	long long dbid_;//数据库当中用来唯一区分老师的id


	int teacherid_;
	int idinque_;//在schedule的teachers当中的序号
	vector<Course> couque_;//教学的科目
	vector<ClassUnit*> clsque_;//属于这个老师所有的课的队列，所有的下标都来自于同一个schedule当中的所有的节次表
	vector<int> clsqueindex_;
	map<TimeTable, vector<ClassUnit*>> clstabque_;//这个老师的所有上的课

	//新版本需要的，教师上课时间
	vector<vector<int>> teach_time_;//所有的具体的上课时间
	

	Teacher(string dbname, long long pkcourse_id, long long dbteacher_id, long long dbid);
	Teacher();
	vector<vector<int>> unitstimecount_;//节次时间的计数
	//vector<int> dayavailable_;//有空的天
	//vector<int> normalappear_;//普通类型出现
	
	//int GetClsNumInPeriod(pair<int, int> period);//返回某一时间段这个老师上多少节课
	//int HasNormalClsInDay(int d);//判断这天是否存在普通类型的课
	//void AddClsInPeriod(int day, int period);
	//target是目标时间,cls是当前这节课，这表示检查当前这节课和这个目标时间的关系
	//bool CheckUnit(ClassUnit* cls, pair<int, int> target);
	//void UpdateUnit(ClassUnit* cls, pair<int, int> target);
	//set<pair<int, int>> GetAvailPeriods();

	bool operator == (const Teacher& a) {
		if (a.teacherid_ == teacherid_)return true;
		return false;
	}
	
	static string GetName() {
		return "Teacher";
	}

private:
	//map<pair<int, int>, int> clsinperiod_;//该时间段上课次数
	//set<pair<int, int>> availperiods_;//教师有空时间
};