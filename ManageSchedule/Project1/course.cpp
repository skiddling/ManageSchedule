#include "course.h"

Course::Course(){
}

Course::Course(string cname) {
}

Course::Course(string dbjname, string course_name, int dbid):
	dbjname_(dbjname), course_name_(course_name), dbid_(dbid){
}
