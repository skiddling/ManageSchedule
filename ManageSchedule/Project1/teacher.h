#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <utility>
#include <set>

using namespace std;

const double kRandPlusRand = (double)RAND_MAX * RAND_MAX;

class Teacher
{
public:
	bool in_school_;//记录教师是否需要天内集中
	int id_;
	string teacher_name_;
	map<int, vector<int> > class_que_;//记录该老师各个班级当中课在总的ClassUnit当中的序号
	//map<int, int> teacher_time_table_;//记录教师每次课的时间，用于判断是否冲突
	//以下三个map值都是从1开始，为0就是代表没有，需要删除该元素
	map<pair<int, int>, int> class_table_;//记录教师每次时间段上几个班,方便最后的输入输出
	map<pair<int, int>, int> room_time_;//记录该老师在某个教师某天是否上课，用于控制老师在一个班同一天不多上课
	//前一个是x，后一个是class_id
	//map<pair<int, int>, int> num_of_period_;//记录该老师在一周中上多少个半天，用于计算reward
	vector<vector<int> > num_of_period;//记录老师在一周上多少个半天
	map<pair<int, int>, bool> available_time;//记录老师在该时段是否空闲，常与roomtime合用
	set<pair<int, int> > cannt_time_;//该老师不能被分配上课的时间
	Teacher();
	Teacher(int teacher_id, string teacher_name);
	void InitAvailable(int daysperweek, int periodperday);
};