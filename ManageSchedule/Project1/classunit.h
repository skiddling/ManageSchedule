#pragma once
#include "course.h"

/*
这是条件限制类，因为对于实际需求而言会在将来不断增加，
所以这个类当中的内容可能会在将来不断的添加,
所以需要把该类设定为模块，以便将来的功能添加
目前就先设定以下几种条件：
1.某老师在某些节次当中不能安排上课
2.某老师的某些课要被预先安排在一些节次当中上课
3.某些科目的课程不能被安排在某些时间段上课
4.某些老师的某几次课需要连续上，也就是所谓的连堂课
*/
/*
这个改为一次课的类，也就是一个单位表示一次课
自带三种限制条件
1.因为是某老师在某个班级的课不能在某些时间段上课，不排课时间序列（存在多个时间段不能被排课）
2.因为是某老师在某个班级的课已经被预先排好在某些时间段，预排课时间
3.连堂标志，表明该节课是否是连堂课，以及是第几节连堂课，限定一次连堂只连堂两节课
*/
class ClassUnit
{
public:
	int dbid_;//数据库读取出来的id号
	static int pim;//shrot for period in moring
	int unit_id_;//该节课的id，是在所有课当中的顺序,和teacher当中的class_que当中的值对应
	int class_id_;//班级号
	string class_name_;//班级名
	int course_id_;
	string course_name_;
	Teacher teacher_;//该节课的任课老师
	//限制条件由老师的信息输入进来
	bool alterable_;//预排课标志，1表示需要被排课，0表示已经限定该节课的时间，不能被安排
	int	continue_tag_;//连堂标志，0表示该节次不是连堂客，1表示这个是连堂客的第一节，2表示第二节
	vector<pair<int, int> > cant_time_;//该节课不能被安排上课的时间序列
	set<pair<int, int> > canttime_;
	pair<int, int> class_time_;//该节课的具体的上课时间

	ClassUnit();
	ClassUnit(Teacher teacher, int class_id, string class_name, string course_name, int course_id, int dbid);
	void AddCant(vector<pair<int, int> > &cant_time);
	inline pair<int, int> get_time();
	void AddUnit(int x, int y, vector<Teacher> &teachers);
	void DelUnit(vector<Teacher> &teachers);
	bool IsConflict(vector<Teacher> &teachers);
	bool CheckUnitTime(int x, int y);
};
