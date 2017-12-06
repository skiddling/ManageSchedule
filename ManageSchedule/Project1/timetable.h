#pragma once
#include "classunit.h"

//班级课表,其实就是一个课的集合
//这个班级当中包含了所有要在这个班级当中上课的节次
class TimeTable
{
public:
	
	//data from database
	long long class_id_;//数据库当中的一个字段
	long long dbid_;//数据库主键序号，用来区分哪个班

	vector<Course> couque_;//这个班级上的具体的哪些课程
	map<Course, int> coutab_;//具体这些课程在这个班级当中每礼拜上几次课
	vector<Teacher> teaque_;//具体在这个班当中上课的哪些老师

	int days_, periods_;//一周天数，每天节次数
	int roomid_;//这个班级的id，也是在vector<timetalbe>当中的序号,需要自己根据需要去建立
	string roomname_;//这个教室的名称
	vector<vector<ClassUnit*>> roomtable_;//教室课表，指针只想当前自己clsque当中的内容

	//从节次指针改成节次序号，方便拷贝赋值等操作
	vector<int> clsque_;
	//vector<ClassUnit> cls_que_;//具体的指针序列
	//vector<vector<bool>> tabletag_;//标记这个时节段是否能被安排放课，初始化课表用
	void CalFitness(int& crash);

	TimeTable(string roomname, int days, int periods, long long class_id, long long dbid);
	TimeTable(const TimeTable& t);
	TimeTable& operator=(const TimeTable& t);
	void Init();
	void UpdatePtrs();
	ClassUnit** GetClsUnitPtr(int day, int period);

	void SwapUnits(ClassUnit* origin, ClassUnit* target);
private:
	

};