#pragma once
#include "course.h"


class ClassUnit
{
public:
	string GetTeaName();
	string GetCouName();
	int GetDuration();
	bool GetType();
private:
	string teaname_, couname_;
	int duration_;//��ʱ����
	bool type_;//��ڿε����ͣ�1��ʾ��ͨ��0��ʾ����
	vector<ClassUnit*> unioncls_;//�ϰ�γ�
};
