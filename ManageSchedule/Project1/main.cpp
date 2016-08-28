#include "dbutils.h"

map<string, int> teachersmap;//record the id of the teacher 
map<string, int> coursesmap;//record the id of the course
//teachers不再用指针形式，因为我们需要在每个学校都各自拥有一个教师的情况
//而指针会造成公用一个地址，也就是导致了所有的generation公用一个teachers
//所以我们需要改变teachers为实体化
//总结就是指针可以这种效果可以static相类似的功能
//vector<Teacher *> teachers;//the queue of all the teachers
vector<Teacher> teachers;
vector<vector<ClassUnit *> > classunits;//all the class units
vector<Course> courses;//the queue of all the courses
vector<TimeTable> timetables;
//所有的行政班的所有的课排成一个队列，并没有安排具体课的时间
//每个班所有的课，只用了一个class_que

void Init() {
	coursesmap.clear();
	teachersmap.clear();
	ifstream fin("input.txt");
	int dpw, ppd, pim;
	fin >> dpw >> ppd >> pim;
	TimeTable::days_per_week_ = dpw;
	TimeTable::period_in_moring_ = pim;
	TimeTable::period_per_day_ = ppd;
	ClassUnit::pim = pim;
	int coursenum, classnum;
	fin >> coursenum >> classnum;
	string coursename;
	courses = vector<Course>(coursenum);
	for (int i = 0; i < coursenum; i++) {
		fin >> coursename;
		coursesmap[coursename] = i;
		Course *cou = new Course(i, coursename);
		//courses.push_back(cou);
		courses[i] = *cou;
	}
	int courselenth;
	string teachername, classname;
	classunits = vector<vector<ClassUnit *> >(classnum);
	for (int i = 0; i < classnum; i++) {
		fin >> classname;
		for (int j = 0; j < coursenum; j++) {
			fin >> courselenth;
			if (courselenth != 0) {
				fin >> teachername;
				//cout << i << ' ' << j << ' ' << teachername << endl;
				map<string, int> :: iterator it = teachersmap.find(teachername);
				if (it == teachersmap.end()) {
					teachersmap[teachername] = teachersmap.size();
					Teacher *tea = new Teacher(teachersmap[teachername], teachername);
					teachers.push_back(*tea);
					courses[j].teacher_queue_.push_back(*tea);
				}
				//i is the classid, class_que is map
				teachers[teachersmap[teachername]].class_que_[i] = vector<int>(courselenth);
				for (int k = 0; k < courselenth; k++) {
					ClassUnit *clsu = new ClassUnit(teachers[teachersmap[teachername]], i, classname, courses[j].course_name_, courses[j].course_id_);
					teachers[teachersmap[teachername]].class_que_[i][k] = classunits[i].size();
					clsu->unit_id_ = classunits[i].size();
					classunits[i].push_back(clsu);
				}
			}
		}
	}
	fin.close();
	Schedule::teachers_map_ = teachersmap;
	Schedule::courses_map_ = coursesmap;
	TimeTable::courses_map_ = coursesmap;
	//put the class unit to their each class
	//timetables = vector<TimeTable>(classnum, TimeTable(coursesmap));
	timetables = vector<TimeTable>(classnum);
	for (int i = 0; i < classnum; i++) {
		vector<ClassUnit *>::iterator itc = classunits[i].begin();
		for (; itc != classunits[i].end(); itc++) {
			timetables[(*itc)->class_id_].class_que_.push_back(*(*itc));
		}
	}
}

void Out() {
	ofstream fout("out.txt");
	fout << "输出老师的个数和老师各自的序号" << endl;
	vector<Teacher> :: iterator itt = teachers.begin();
	for (; itt != teachers.end(); itt++) {
		fout << itt->id_ << ' ' << itt->teacher_name_ << "\n";
	}
	fout << endl << endl;
	fout << "输出每个科目的下有哪些老师" << endl;
	vector<Course> :: iterator itc = courses.begin();
	for (; itc != courses.end(); itc++) {
		fout << itc->course_id_ << ' ' << itc->course_name_ << "\n";
		itt = itc->teacher_queue_.begin();
		for (; itt != itc->teacher_queue_.end(); itt++) {
			fout << itt->id_ << ' ' << itt->teacher_name_ << "\n";
		}
		fout << endl;
	}
	fout << endl << endl;
	fout << "输出所有的班级当中所有的课程数量" << endl;
	/*vector<ClassUnit *> :: iterator itu = classunits.begin();
	for (; itu != classunits.end(); itu++) {
		fout << (*itu)->class_id_ << ' ' << (*itu)->class_name_ << ' ' << (*itu)->teacher_.id_ << ' ' << (*itu)->teacher_.teacher_name_ << "\n";
	}*/
	for (int i = 0; i < classunits.size(); i++) {
		vector<ClassUnit *>::iterator itu = classunits[i].begin();
		for (; itu != classunits[i].end(); itu++) {
			fout << (*itu)->class_id_ << ' ' << (*itu)->class_name_ << ' ' << (*itu)->teacher_.id_ << ' ' << (*itu)->teacher_.teacher_name_ << "\n";
		}
	}
	fout << endl << endl;
	fout << "输出每个班级当中有哪些老师上课，上多少次课" << endl;
	for (int i = 0; i < timetables.size(); i++) {
		fout << "class " << i << "\n";
		for (int j = 0;j < timetables[i].class_que_.size(); j++) {
			fout << timetables[i].class_que_[j].class_id_ << ' ' 
				<< timetables[i].class_que_[j].class_name_ << ' ' 
				<< timetables[i].class_que_[j].teacher_.teacher_name_ << "\n";
		}
		fout << endl;
	}
	fout.close();
}

//设置连堂
void AddContinue() {
	ifstream fin("continue.txt");
	int classid, courseid, continuenum, nrows;
	fin >> nrows;
	for (int i = 0; i < nrows; i++) {
		fin >> classid >> courseid >> continuenum;
		timetables[classid].AddContinue(courseid, continuenum);
	}
	fin.close();
}

//不能上课的时间
void AddCant() {
	ifstream fin("cant.txt");
	int nrows, classid, courseid, num, x, y;
	fin >> nrows;
	for (int i = 0; i < nrows; i++) {
		fin >> classid >> courseid >> num;
		vector<pair<int, int> > canttime = vector<pair<int, int> > (num);
		for (int j = 1; j < num; j++) {
			fin >> x >> y;
			canttime[j].first = x;
			canttime[j].second = y;
		}
		timetables[classid].AddCant(courseid, canttime);
	}
	fin.close();
}

//被预定时间的课程
void AddItime() {
	ifstream fin("itime.txt");
	int nrows, classid, courseid, num, x, y;
	fin >> nrows;
	for (int i = 0; i < nrows; i++) {
		fin >> classid >> courseid >> num;
		vector<pair<int, int> > itime = vector<pair<int, int> >(num);
		for (int j = 0; j < num; j++) {
			fin >> x >> y;
			itime[j].first = x;
			itime[j].second = y;
		}
		timetables[classid].AddItime(courseid, itime);
	}
	fin.close();
}

//输出结果
void Output(Schedule res) {
	ofstream fout("res.txt");
	string name;
	for (int i = 0; i < res.time_tables_.size(); i++) {
		fout << "班级 " << i << endl << "   ";
		for (int x = 0; x < TimeTable::days_per_week_; x++) {
			fout << "星期" << x + 1 << "      ";
		}
		fout << endl;
		for (int y = 0; y < TimeTable::period_per_day_; y++) {	
			fout << y + 1 << "  ";
			for (int x = 0; x < TimeTable::days_per_week_; x++) {
				if (res.time_tables_[i].table_[x][y] != NULL)
					name = res.time_tables_[i].table_[x][y]->teacher_.teacher_name_;
				else name = "";
				fout << name << string(11 - name.length(), ' ');
			}
			fout << endl;
		}
		fout << endl;
	}
	fout.close();
}

int main(int argc, char* argv[]) {
	srand((unsigned int)time(0));
	Dbutils db;
	db.GetDBData(argv, teachersmap, coursesmap, teachers, courses, timetables);
	//system("PAUSE");
	//return 0;
	//Init();
	//AddContinue();
	//AddCant();
	//AddItime();
	//Out();
	cout << "end of init\n";
	//system("PAUSE");
	//return 0;
	//GA ga(teachersmap, coursesmap, teachers, classunits, courses, timetables);
	GA ga(teachersmap, coursesmap, teachers, courses, timetables);
	ga.Generate();
	Output(ga.res);
	return 0;
}