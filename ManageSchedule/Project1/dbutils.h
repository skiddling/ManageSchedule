#pragma once
#include "ga.h"

class Dbutils
{
public:
	void GetDBData(char* argv[], map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable> &timetables);

private:
	string dbname_;
	string dbuser_name_;
	string dbuser_pwd_;
	string task_id_;//��¼����ſε�id���������Ĳ���

	void GetDBInfo(char* argv[]);
	void GetPKTaskInfo();
};