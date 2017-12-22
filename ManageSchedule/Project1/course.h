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
	long long dbid_;//数据库当中这个课程的id
	string dbjname_;//数据库当中这个课程的简称
	string course_name_;
	vector<int> clsqueindex_;//所有这门课下的所有的具体的教学班，所有的都对应同一个schedule当中的节次
	vector<ClassUnit*> clsque_;//所有这门课下的所有的具体的教学班，所有的都对应同一个schedule当中的节次

	Course();
	Course(string cname);
	Course(string dbjname, string course_name, int dbid);
	
	static string GetName() {
		return "Course";
	}

private:
	vector<Course> unioncous_;//合班的课程
	//Course(int course_id, string course_name);
};