#pragma once
#include "course.h"

class Teacher
{
public:
	vector<Course> couque_;//��ѧ�Ŀ�Ŀ
	map<TimeTable, vector<ClassUnit*>> clstabque_;
	Teacher();
};