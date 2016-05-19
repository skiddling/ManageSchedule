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
	vector<vector<ClassUnit *> > table_;//the real timetable of one class
	vector<ClassUnit> class_que_;//all the classes in this class, they have their own address in memory
	vector<vector<ClassUnit *> > course_classes;//record the point of each course's class's address, the address of the class_que's unit
	map<string, int> courses_map_;

	TimeTable();
	void Init(map<string, int> &courses_map, TimeTable &time_table, vector<Teacher *> &teachers);
	void AddContinue(int course_id, int continue_num);
	void AddCant(int course_id, vector<pair<int, int> > &cant_time);
	void AddItime(int course_id, vector<pair<int, int> > &itime);
	void DelUnit(ClassUnit &cu, vector<Teacher *> teachers);
	void AddUnit(ClassUnit &cu, int x, int y, vector<Teacher *> teachers);
	void Update(int x, int y, int nx, int ny, vector<Teacher *> teachers);
	void Mutate(double mp, vector<Teacher *> teachers);
	void Cross(TimeTable &timetable, double cp);
	void CalCrash();
	void GetRandTable(vector<vector<int> > &randtable);
	void SetUnitInfo(ClassUnit &cu, int x, int y, vector<Teacher *> &teachers);
};



