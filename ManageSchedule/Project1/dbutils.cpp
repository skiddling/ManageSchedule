#include "dbutils.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF")

string Dbutils::StartPk(string pktaskid) {
	pktaskid_ = pktaskid;
	statement_ = "";
	GetDBInfo();
	GetDataFromTable(&DButils::Get_T_SPKTask, "T_SPKTask");
	GetDataFromTable(&DButils::Get_T_SPKClassRoom, "T_SPKClassRoom");
	GetDataFromTable(&DButils::Get_T_SPKGroup, "T_SPKGroup");
	GetDataFromTable(&DButils::Get_T_SPKCourse, "T_SPKCourse");
	GetDataFromTable(&DButils::Get_T_SPKStudent, "T_SPKStudent");
	GetDataFromTable(&DButils::Get_T_SPKStudentCourse, "T_SPKStudentCourse");
	GetDataFromTable(&DButils::Get_T_SPKTeacher, "T_SPKTeacher");
	GetDataFromTable(&DButils::Get_T_SPKTeacherCourseConfig, "T_SPKTeacherCourseConfig");
	return statement_;
}

string Dbutils::StartPk() {
	//pktaskid_ = pktaskid;
	pktaskid_ = "10012";
	statement_ = "";
	GetDBInfo();
	GetDataFromTable(&DButils::Get_T_SPKTask, "T_SPKTask");
	GetDataFromTable(&DButils::Get_T_SPKClassRoom, "T_SPKClassRoom");
	GetDataFromTable(&DButils::Get_T_SPKGroup, "T_SPKGroup");
	GetDataFromTable(&DButils::Get_T_SPKCourse, "T_SPKCourse");
	GetDataFromTable(&DButils::Get_T_SPKStudent, "T_SPKStudent");
	GetDataFromTable(&DButils::Get_T_SPKStudentCourse, "T_SPKStudentCourse");
	GetDataFromTable(&DButils::Get_T_SPKTeacher, "T_SPKTeacher");
	GetDataFromTable(&DButils::Get_T_SPKTeacherCourseConfig, "T_SPKTeacherCourseConfig");
	return statement_;

}

void Dbutils::OutPutResult() {
}
