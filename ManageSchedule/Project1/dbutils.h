#pragma once
#include "ga.h"

class Dbutils
{
public:
	void GetDBData(char* argv[], map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable> &timetables);
	void OutPutPKTeaching(Schedule res);

private:
	string db_server_;
	string dbname_;
	string dbuser_name_;
	string dbuser_pwd_;
	string db_port_;
	string pk_task_id_;//记录这次排课的id，传进来的参数
	vector<vector<ClassUnit *> > classunits;
	map<int, vector<ClassUnit*> > dbid2cls_;

	void CutString(string &s);
	string SetDBProperties();
	void GetDBInfo(char* argv[]);
	void GetPKTaskInfo();
	void GetPKCourse(map<string, int> &coursesmap, vector<Course> &courses);
	void GetClassCourseLessonConfigInfo(map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable>& timetables);
	void Out(map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable>& timetables);
	void GetPKCourseNonTeachingInfo();
};