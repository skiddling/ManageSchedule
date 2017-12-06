#pragma once
#include "teacher.h"

class TimeTable;

class ClassUnit
{
public:
	//data get from datebase
	int secionno_;//��ڿε����
	long long pkcombinateclassgroup_;//�ϰ����ţ�һ��ϰ�Ŀε���һ������һ����ͬ�����
	long long pkevensection_;//���õ���ţ������ڴκ���һ���
	long long dbid_;//���ݿ⵱�еĽ�ѧ���id,Ҳ����pkclasscocurse��ֵ

	ClassUnit(TimeTable* ttb, Teacher* teacher, Course course);
	ClassUnit();
	Course course_;//�����ʲô��
	Course* couptr_;
	Teacher* teacher_;//��ڿε��ο���ʦ��ָ��
	pair<int, int> stime_;//������ڿε���ʼʱ��
	TimeTable* ttbptr_;//ָ���Լ����ڿα��ָ�룬�����Ҫ�ڸ�ֵ��ʱ���޸ĵ�
	bool hasbeenput_;//��ʾ��ڿ��Ƿ��Ѿ����ŵ��α��У����ڿα��ʼ��
	ClassUnit** headptr_;//������ʾ�Ƿ������ÿεĵ�һ�ڣ�ָ��������ttb����ָ���һ������ε�ָ��
	int type_;//��ڿε����ͣ�1��ʾ��ͨ��0��ʾ������2��ʾ����ͨ���͵����ÿΣ����Ǹ������Ͳ��ܺ���һ����ͬһ�����
	int duration_;//��ʱ����

	vector<int> unioncls_;//�ϰ�γ̣���ֵ����ָ��ͬһ��schedule���е��±�
	vector<int> unioclsid_;//�ϰ�γ�id
	pair<int, int> headunit_;

	//�������������ĵ���������
	set<pair<int, int>> canntbeput_;//����֮��ڿβ��ܰ���ʱ��
	set<pair<int, int>> canbeput_;//��canntbeput�ǲ���
	
	//����α��ʼ������
	bool operator < (const ClassUnit c) {
		if (duration_ != c.duration_)
			return duration_ > c.duration_;
		else if (unioclsid_.size() != c.unioclsid_.size())
			return unioclsid_.size() > c.unioclsid_.size();
		else if (canntbeput_.size() != c.canntbeput_.size())
			return canntbeput_.size() > c.canntbeput_.size();
	}
	
	string GetTeaName();
	string GetCouName();
	int GetDuration();
	bool GetType();
	void PutIntoTable(int day, int period);
	int CalFitness();
	int GetTeacherIdInVec();
	int GetTimeTableIdInVec();
	vector<pair<int, int>> GetRandAvailTime();
	bool CheckPeriod(pair<int, int> per);
	void ChangeTime(pair<int, int> period);

private:
	default_random_engine e_;
	string teaname_, couname_;
	int cuid_;//�ýڿε�id
	

	void GetRandSet(vector<pair<int, int>>& randset);
};
