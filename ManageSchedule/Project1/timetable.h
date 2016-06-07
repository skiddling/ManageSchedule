#pragma once
#include "classunit.h"

/*
table�������һ����Ŀα�
Ҳ���Ǳ���mutex�ͽ���cross������ʵ�ʶ���
����mutex�Ƕ���һ�����嵱��һ����Ŀα���ԣ�Ҳ����һ���൱�е�ĳЩ���໥�����Ͽ�ʱ��
����cross�Ƕ����������嵱�У�ÿ���γ���0.5�ĸ��ʾ���ȡ����һ���α��ʱ�䣬�Ӷ�ȷ��ÿһ�ڿε��Ͽ�ʱ�䣬
��ĳһ���γ����
*/

class TimeTable
{
public:
	static int days_per_week_;//dpw for short
	static int period_per_day_;//ppd for short
	static int period_in_moring_;//pim for short
	static map<string, int> courses_map_;
	//the real timetable of one class,ָ��class_que_���е�λ��
	vector<vector<ClassUnit *> > table_;
	vector<ClassUnit> class_que_;//all the classes in this class, they have their own address in memory
	//ָ��ÿ��ÿ�Ƶ��еĿ�Ŀ
	vector<vector<ClassUnit *> > course_classes_;
	//map<string, int> courses_map_;

	void Init(TimeTable &time_table, vector<Teacher> &teachers);
	void AddContinue(int course_id, int continue_num);
	void AddCant(int course_id, vector<pair<int, int> > &cant_time);
	void AddItime(int course_id, vector<pair<int, int> > &itime);
	void Update(int x, int y, int nx, int ny, vector<Teacher> &teachers);
	//mutate��cross������������Ҫ���ǵ����ÿεĿ�����
	void Mutate(double mp, vector<Teacher> &teachers);
	void Cross(TimeTable &timetable, double cp, vector<Teacher> &teachers);
	void Modify(vector<Teacher> &teachers);
	void GetRandTable(vector<vector<int> > &randtable);
	bool CanMutate(int x, int y, int nx, int ny, vector<Teacher> &teachers);
	void SolveConflict(ClassUnit *cu, vector<Teacher> &teachers);
	void UnitSwap(int x, int y, int nx, int ny, vector<Teacher> &teachers);
	bool CheckUnit(int x, int y, int nx, int ny, vector<Teacher> &teachers);

	TimeTable& operator = (TimeTable &tb) {
		class_que_ = tb.class_que_;
		course_classes_ = tb.course_classes_;
		for (int i = 0; i < days_per_week_; i++) {
			for (int j = 0; j < period_per_day_; j++) {
				table_[i][j] = NULL;
			}
		}
		pair<int, int> ct;
		int f, s, p;
		for (int i = 0; i < tb.class_que_.size(); i++) {
			ct = tb.class_que_[i].class_time_;
			f = ct.first;
			s = ct.second;
			p = tb.table_[f][s]->unit_id_;
			table_[f][s] = &(class_que_[p]);
		}
		return *this;
	}
};