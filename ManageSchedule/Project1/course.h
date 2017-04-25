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

using namespace std;

class ClassUnit;
class TimeTable;

class Course
{
public:
	int course_id_;
	string course_name_;
	Course(string cname);
private:
	vector<Course> unioncous_;//ºÏ°àµÄ¿Î³Ì
	//Course(int course_id, string course_name);
};