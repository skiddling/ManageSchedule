#pragma once
#include "classunit.h"

/*
table代表的是一个班的课表
也就是变异mutex和交叉cross操作的实际对象
变异mutex是对于一个个体当中一个班的课表而言，也就是一个班当中的某些课相互交换上课时间
交叉cross是对于两个个体当中，每个课程以0.5的概率决定取其中一个课表的时间，从而确定每一节课的上课时间，
即某一个课程如果
*/

class TimeTable
{
public:
	static int days_per_week_;//dpw for short
	static int period_per_day_;//ppd for short
	static int period_in_moring_;//pim for short
	static map<string, int> courses_map_;
	//the real timetable of one class,指向class_que_当中的位置
	vector<vector<ClassUnit *> > table_;
	vector<ClassUnit> class_que_;//all the classes in this class, they have their own address in memory
	//指向每个每科当中的科目
	vector<vector<ClassUnit *> > course_classes_;
	//map<string, int> courses_map_;

	void Init(TimeTable &time_table, vector<Teacher> &teachers);
	void AddContinue(int course_id, int continue_num);
	void AddCant(int course_id, vector<pair<int, int> > &cant_time);
	void AddItime(int course_id, vector<pair<int, int> > &itime);
	void Update(int x, int y, int nx, int ny, vector<Teacher> &teachers);
	//mutate和cross这两个操作都要考虑到连堂课的可能性
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