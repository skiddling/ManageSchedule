#pragma once
#include "course.h"

class Teacher
{
public:
	int teacherid_;
	vector<Course> couque_;//教学的科目
	map<TimeTable, vector<ClassUnit*>> clstabque_;
	Teacher();
	vector<vector<int>> unitstimecount_;//节次时间的计数
	vector<int> dayavailable_;//有空的天
	vector<int> primeappear_;//普通类型出现
};