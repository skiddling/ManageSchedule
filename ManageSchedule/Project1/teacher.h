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
	bool in_school_;//��¼��ʦ�Ƿ���Ҫ���ڼ���
	int id_;
	string teacher_name_;
	map<int, vector<int> > class_que_;//��¼����ʦ�����༶���п����ܵ�ClassUnit���е����
	//map<int, int> teacher_time_table_;//��¼��ʦÿ�οε�ʱ�䣬�����ж��Ƿ��ͻ
	//��������mapֵ���Ǵ�1��ʼ��Ϊ0���Ǵ���û�У���Ҫɾ����Ԫ��
	map<pair<int, int>, int> class_table_;//��¼��ʦÿ��ʱ����ϼ�����,���������������
	map<pair<int, int>, int> room_time_;//��¼����ʦ��ĳ����ʦĳ���Ƿ��ϿΣ����ڿ�����ʦ��һ����ͬһ�첻���Ͽ�
	//ǰһ����x����һ����class_id
	//map<pair<int, int>, int> num_of_period_;//��¼����ʦ��һ�����϶��ٸ����죬���ڼ���reward
	vector<vector<int> > num_of_period;//��¼��ʦ��һ���϶��ٸ�����
	map<pair<int, int>, bool> available_time;//��¼��ʦ�ڸ�ʱ���Ƿ���У�����roomtime����
	set<pair<int, int> > cannt_time_;//����ʦ���ܱ������Ͽε�ʱ��
	Teacher();
	Teacher(int teacher_id, string teacher_name);
	void InitAvailable(int daysperweek, int periodperday);
};