#pragma once
#include "course.h"

class Teacher
{
public:
	int teacherid_;
	vector<Course> couque_;//��ѧ�Ŀ�Ŀ
	map<TimeTable, vector<ClassUnit*>> clstabque_;
	Teacher();
	vector<vector<int>> unitstimecount_;//�ڴ�ʱ��ļ���
	vector<int> dayavailable_;//�пյ���
	vector<int> primeappear_;//��ͨ���ͳ���
};