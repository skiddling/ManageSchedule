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
	int day_num_;//�Ͽ�����
	int am_num_;
	int pm_num_;
	int ev_num_;
	vector<Course> couque_;
	map<long long, Course> coutab_;//�γ̺���Ӧid
	map<long long, int> couinque_;//���ݿ�id����Ӧ���е��е�ӳ��

	vector<Teacher> teaque_;
	map<long long, Teacher> teatab_;//��ʦ��idӳ��
	map<long long, int> teainque_;//���ݿ�id����Ӧ�Ķ��е���ӳ��
	
	vector<TimeTable> roomque_;
	map<long long, TimeTable> roomtab_;
	map<long long, int> roominque_;//���ݿ�id����Ӧ�Ķ��е��е�ӳ��

	//��ѧ��ӳ���Ҳ���ǽ�ѧ���id��Ӧ�����ļ��ڿ�
	map<long long, vector<ClassUnit*>> class_course_units_;

	//����γ̺���Ӧ�Ľ���ӳ����ĸ��γ����ĸ������Ͽ�
	map<long long, int> cou_room_tab_;

	//ͨ���༶����Ŀ����ʦ���ڴ�������ҵ���Ӧ��ĳһ���ڿ�
	map<long long, map<int, ClassUnit*>> unitstab_;

	//�ϰ�����
	map<long long, vector<ClassUnit*>> unionclstab_;
	
	//���ÿε���ţ���Ҫͨ���ⲿ�����������ݿ⵱�е����ݺ��㷨����һ�οε����ݵ�ͳһ
	map<long long, vector<ClassUnit*>> continues_cls_tab_;
	
	//��ɾ���Ľڴεļ�¼��
	//set<long long> deleted_units_set_;
	unordered_set<ClassUnit*> deleted_units_set_;
	
private:
	string db_server_;
	string dbname_;
	string dbuser_name_;
	string dbuser_pwd_;
	string db_port_;
	string pktaskid_;//��¼����ſε�id���������Ĳ���
	string statement_;//��ȡ���ݷ���ֵ��һ��ûʲô��
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
};