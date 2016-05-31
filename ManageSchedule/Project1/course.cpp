#include "course.h"

Course :: Course() {
	course_id_ = -1;
	course_name_ = "";
	teacher_queue_ = vector<Teacher *>(0);
}

Course :: Course(int course_id, string course_name) {
	course_id_ = course_id;
	course_name_ = course_name;
	teacher_queue_ = vector<Teacher *>(0);
}