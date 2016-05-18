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
	bool in_school_;//��¼��ʦ�Ƿ���Ҫ���ڼ���
	int id_;
	string teacher_name_;
	map<int, vector<int> > class_que_;//��¼����ʦ�����༶���п����ܵ�ClassUnit���е����
	//map<int, int> teacher_time_table_;//��¼��ʦÿ�οε�ʱ�䣬�����ж��Ƿ��ͻ
	map<pair<int, int>, int> class_table_;//��¼��ʦÿ��ʱ������ĸ��༶�Ͽ�,���������������
	Teacher();
	Teacher(int teacher_id, string teacher_name);
};