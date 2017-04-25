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
	int duration_;//课时长度
	bool type_;//这节课的类型，1表示普通，0表示辅助
	vector<ClassUnit*> unioncls_;//合班课程
};
