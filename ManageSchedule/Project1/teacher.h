#pragma once
#include "course.h"

class Teacher
{
public:
	vector<Course> couque_;//教学的科目
	map<TimeTable, vector<ClassUnit*>> clstabque_;
	Teacher();
};