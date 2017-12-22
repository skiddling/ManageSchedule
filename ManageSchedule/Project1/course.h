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
#include <thread>
#include <future>
#include <mutex>
#include <functional>
#include <chrono>
#include <unordered_set>

using namespace std;

class ClassUnit;
class TimeTable;

typedef pair<TimeTable*, pair<int, int>> Node;

class Course
{
public:
	int course_id_;
	long long dbid_;//���ݿ⵱������γ̵�id
	string dbjname_;//���ݿ⵱������γ̵ļ��
	string course_name_;
	vector<int> clsqueindex_;//�������ſ��µ����еľ���Ľ�ѧ�࣬���еĶ���Ӧͬһ��schedule���еĽڴ�
	vector<ClassUnit*> clsque_;//�������ſ��µ����еľ���Ľ�ѧ�࣬���еĶ���Ӧͬһ��schedule���еĽڴ�

	Course();
	Course(string cname);
	Course(string dbjname, string course_name, int dbid);
	
	static string GetName() {
		return "Course";
	}

private:
	vector<Course> unioncous_;//�ϰ�Ŀγ�
	//Course(int course_id, string course_name);
};