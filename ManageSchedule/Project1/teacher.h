#pragma once
#include "course.h"

class Teacher
{
public:
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
private:
	map<pair<int, int>, int> clsinperiod_;//��ʱ����Ͽδ���
	set<pair<int, int>> availperiods_;//��ʦ�п�ʱ��
};