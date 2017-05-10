#pragma once
#include "classunit.h"

//班级课表,其实就是一个课的集合
class TimeTable
{
public:
	
	int days_, periods_;//一周天数，每天节次数
	int roomid_;//这个班级的id
	string roomname_;//这个教室的名称
	vector<vector<ClassUnit*>> roomtable_;//教室课表，指针只想当前自己clsque当中的内容
	vector<ClassUnit> clsque_;//节次序列
	//vector<vector<bool>> tabletag_;//标记这个时节段是否能被安排放课，初始化课表用

	TimeTable(const TimeTable& t);
	TimeTable& operator=(const TimeTable& t);
	void Init();
	void UpdatePtrs();
private:
	

};