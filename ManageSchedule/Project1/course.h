#pragma once
#include "teacher.h"

//course��ֻ��Ϊ�˷����ҳ�ÿ����ʦ����Ϣ
class Course
{
public:
	int course_id_;
	vector<Teacher *> teacher_queue_;//�ÿ�Ŀ�µ�������ʦ�����ڷ������������Լ�����α���
	string course_name_;
	Course();
	Course(int course_id, string course_name);
};