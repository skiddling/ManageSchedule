#pragma once
#include "course.h"

class TimeTable;

class ClassUnit
{
public:
	string GetTeaName();
	string GetCouName();
	int GetDuration();
	bool GetType();
	pair<int, int> stime_;//������ڿε���ʼʱ��
	TimeTable* ttbptr_;//ָ���Լ����ڿα��ָ��
	void PutIntoTable(int day, int period);
	bool hasbeenput_;//��ʾ��ڿ��Ƿ��Ѿ����ŵ��α��У����ڿα��ʼ��
	
	//����α��ʼ������
	bool operator < (const ClassUnit c) {
		if (duration_ != c.duration_)
			return duration_ > c.duration_;
		else if (unioclsid_.size() != c.unioclsid_.size())
			return unioclsid_.size() > c.unioclsid_.size();
		else if (canntbeput_.size() != c.canntbeput_.size())
			return canntbeput_.size() > c.canntbeput_.size();
	}

private:
	string teaname_, couname_;
	int duration_;//��ʱ����
	int type_;//��ڿε����ͣ�1��ʾ��ͨ��0��ʾ������2��ʾ����ͨ���͵����ÿΣ����Ǹ������Ͳ��ܺ���һ����ͬһ�����
	int cuid_;//�ýڿε�id
	vector<ClassUnit*> unioncls_;//�ϰ�γ�
	vector<int> unioclsid_;//�ϰ�γ�id
	pair<int, int> headunit_;
	set<pair<int, int>> canntbeput_;//����֮��ڿβ��ܰ���ʱ��
};
