#pragma once
#include "ga.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

class Dbutils
{
public:
	void GetDBData(char* argv[], map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable> &timetables);
	void OutPutPKTeaching(Schedule res);
	void StartPK();
	
	//DButil();
	string StartPk(string pktaskid);
	string StartPk();
	void OutPutResult();

	//data get from database
	//keep name same with database
	string task_name_;
	long long term_id_;
	string term_name_;
	string grade_name_;
	int day_num_;//上课天数
	int am_num_;
	int pm_num_;
	int ev_num_;
	//此处的所有的都是索引值，因为此处的下标完全没有用，但是在排课系统当中需要进行相应的拷贝操作
	//所以还是需要用到指针，所以在拷贝赋值的构造函数当中需要进行相应操作
	vector<Course> couque_;
	map<long long, Course> coutab_;//课程和相应id
	map<long long, int> couinque_;//数据库id和相应队列当中的映射

	vector<Teacher> teaque_;
	map<long long, Teacher> teatab_;//教师和id映射
	map<long long, int> teainque_;//数据库id和相应的队列当中映射
	
	vector<TimeTable> roomque_;
	map<long long, TimeTable> roomtab_;
	map<long long, int> roominque_;//数据库id和相应的队列当中的映射

	//教学班映射表，也就是教学班的id对应具体哪几节课
	map<long long, vector<int>>class_course_units_;

	//具体课程和相应的教室映射表，哪个课程在哪个教室上课
	map<long long, int> cou_room_tab_;

	//通过具体教学班，具体节次序号来找到相应的某一个节课
	map<long long, map<int, int>> unitstab_;

	//合班的组号
	map<long long, vector<int>> unionclstab_;
	
	//连堂课的组号，需要通过这部分来进行数据库当中的数据和算法当中一次课的数据的统一
	map<long long, vector<int>> continues_cls_tab_;
	
	//被删除的节次的记录表
	//set<long long> deleted_units_set_;
	unordered_set<int> deleted_units_set_;

	//具体节次的队列
	vector<ClassUnit> clsque_;
	
private:
	string db_server_;
	string dbname_;
	string dbuser_name_;
	string dbuser_pwd_;
	string db_port_;
	string pktaskid_;//记录这次排课的id，传进来的参数
	string statement_;//读取数据返回值，一般没什么用
	vector<vector<ClassUnit *> > classunits;
	map<int, vector<ClassUnit*> > dbid2cls_;

	void CutString(string &s);
	string SetDBProperties();
	void GetDBInfo();
	void GetDataFromTable(void (Dbutils::*funcptr)(_RecordsetPtr& m_pRecordset), string tablename);
	void Get_T_PKTask(_RecordsetPtr& m_pRecordset);
	void Get_T_PKClass(_RecordsetPtr& m_pRecordset);
	void Get_T_PKClassCourse(_RecordsetPtr& m_pRecordset);
	void Get_T_PKClassCourseLessonText(_RecordsetPtr& m_pRecordset);
	void Get_T_PKClassCourseNonSection(_RecordsetPtr& m_pRecordset);
	void Get_T_PKClassCourseOrgSectionSet(_RecordsetPtr& m_pRecordset);
	void Get_T_PKClassCoursePreSection(_RecordsetPtr& m_pRecordset);
	void Get_T_PKClassNonSection(_RecordsetPtr& m_pRecordset);
	void Get_T_PKCourse(_RecordsetPtr& m_pRecordset);
	void Get_T_PKCourseNonSection(_RecordsetPtr& m_pRecordset);
	void Get_T_PKNonEvenSection(_RecordsetPtr& m_pRecordset);
	void Get_T_PKTeacher(_RecordsetPtr& m_pRecordset);
	void Get_T_PKTeacherNonSection(_RecordsetPtr& m_pRecordset);
	void GetPKCourse(map<string, int> &coursesmap, vector<Course> &courses);
	void GetClassCourseLessonConfigInfo(map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable>& timetables);
	void Out(map<string, int> &teachersmap, map<string, int> &coursesmap, vector<Teacher> &teachers, vector<Course> &courses, vector<TimeTable>& timetables);
	void GetPKCourseNonTeachingInfo();
	void UpdateUnionCls();
	void UpdateContinueCls();
	void UpdateQueIndex();
};