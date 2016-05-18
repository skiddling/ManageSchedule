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
	map<pair<int, int>, int> class_table_;//记录教师每次时间段在哪个班级上课,方便最后的输入输出
	Teacher();
	Teacher(int teacher_id, string teacher_name);
};