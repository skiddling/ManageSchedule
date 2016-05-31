#pragma once
#include "teacher.h"

//course类只是为了方便找出每个老师的信息
class Course
{
public:
	int course_id_;
	vector<Teacher *> teacher_queue_;//该科目下的所有老师，用于方便输入数据以及输出课表用
	string course_name_;
	Course();
	Course(int course_id, string course_name);
};