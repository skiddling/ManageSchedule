#pragma once
#include "course.h"

class Teacher
{
public:
	//data from database
	string dbname_;//��ʦ����
	long long pkcourse_id_;
	long long dbteacher_id_;
	long long dbid_;//���ݿ⵱������Ψһ������ʦ��id

	Teacher(string dbname, long long pkcourse_id, long long dbteacher_id, long long dbid);

	int teacherid_;
	int idinque_;//��schedule��teachers���е����
	vector<Course> couque_;//��ѧ�Ŀ�Ŀ
	vector<ClassUnit*> clsque_;//���������ʦ���еĿεĶ���
	map<TimeTable, vector<ClassUnit*>> clstabque_;//�����ʦ�������ϵĿ�
	Teacher();
	vector<vector<int>> unitstimecount_;//�ڴ�ʱ��ļ���
	vector<int> dayavailable_;//�пյ���
	vector<int> normalappear_;//��ͨ���ͳ���
	
	int GetClsNumInPeriod(pair<int, int> period);//����ĳһʱ��������ʦ�϶��ٽڿ�
	int HasNormalClsInDay(int d);//�ж������Ƿ������ͨ���͵Ŀ�
	void AddClsInPeriod(int day, int period);
	//target��Ŀ��ʱ��,cls�ǵ�ǰ��ڿΣ����ʾ��鵱ǰ��ڿκ����Ŀ��ʱ��Ĺ�ϵ
	bool CheckUnit(ClassUnit* cls, pair<int, int> target);
	void UpdateUnit(ClassUnit* cls, pair<int, int> target);
	set<pair<int, int>> GetAvailPeriods();

	bool operator == (const Teacher& a) {
		if (a.teacherid_ == teacherid_)return true;
		return false;
	}
private:
	map<pair<int, int>, int> clsinperiod_;//��ʱ����Ͽδ���
	set<pair<int, int>> availperiods_;//��ʦ�п�ʱ��
};