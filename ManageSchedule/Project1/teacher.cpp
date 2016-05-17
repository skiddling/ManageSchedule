#include "teacher.h"

Teacher::Teacher() {
	in_school_ = 0;
	teacher_name_ = "";
	id_ = -1;
}

Teacher::Teacher(int teacher_id, string teacher_name) {
	id_ = teacher_id;
	teacher_name_ = teacher_name;
}