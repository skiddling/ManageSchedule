#pragma once
#include "classunit.h"

/*
table��������һ����Ŀα�
Ҳ���Ǳ���mutex�ͽ���cross������ʵ�ʶ���
����mutex�Ƕ���һ�����嵱��һ����Ŀα����ԣ�Ҳ����һ���൱�е�ĳЩ���໥�����Ͽ�ʱ��
����cross�Ƕ����������嵱�У�ÿ���γ���0.5�ĸ��ʾ���ȡ����һ���α���ʱ�䣬�Ӷ�ȷ��ÿһ�ڿε��Ͽ�ʱ�䣬
��ĳһ���γ����
*/

class TimeTable
{
public:
	static int days_per_week_;//dpw for short
	static int period_per_day_;//ppd for short
	static int period_in_moring_;//pim for short
	vector<vector<ClassUnit> > table_;//the real timetable of one class
	vector<ClassUnit> class_que_;//all the classes in this class, they have their own address in memory
	vector<vector<ClassUnit *> > course_classes;//record the point of each course's class's address, the address of the class_que's unit
	

	TimeTable();
	void Init(int courses_num);
	void AddContinue(int course_id, int continue_num);
	void AddCant(int course_id, vector<pair<int, int> > &cant_time);
	void AddItime(int course_id, vector<pair<int, int> > &itime);
	void Mutate(double mp);
	void Cross(TimeTable &timetable);
	void CalCrash();
};


