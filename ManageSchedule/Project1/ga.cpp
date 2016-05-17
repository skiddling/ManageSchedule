#include "ga.h"

const int mxcrash = 100000000;
const int timeout = 7200000;
const int loctimeout = 600000;
const int crashout = 20;
const double mxmp = 0.03;
const double mxmpg = 0.25;
const double mxcp = 0.35;
const double step = 1.3;

//GA::GA() {
//	population_ = 300;
//	ifstream fin("input.txt");
//	int dpw, ppd, pim;
//	fin >> dpw >> ppd >> pim;
//	TimeTable::days_per_week_ = dpw;
//	TimeTable::period_in_moring_ = pim;
//	TimeTable::period_per_day_ = ppd;
//
//	int course_num, class_num;
//	fin >> course_num >> class_num;
//	string course_name;
//	courses_ = vector<Course *>(course_num);
//	for (int i = 0; i < course_num; i++) {
//		fin >> course_name;
//		courses_map_[course_name] = i;
//		Course *cou = new Course(i, course_name);
//		courses_[i] = cou;
//	}
//
//	int course_length;
//	string teacher_name, class_name;
//	for (int i = 0; i < class_num; i++) {
//		fin >> class_name;
//		for (int j = 0; j < course_num; j++) {
//			fin >> course_length;
//			if (course_length != 0) {
//				fin >> teacher_name;
//				map<string, int> ::iterator itt = teachers_map_.find(teacher_name);
//				if (itt == teachers_map_.end()) {
//					teachers_map_[teacher_name] = teachers_map_.size();
//					Teacher *tea = new Teacher(teachers_map_[teacher_name], teacher_name);
//					teachers_.push_back(tea);
//					courses_[j]->teacher_queue_.push_back(tea);
//				}
//				teachers_[teachers_map_[teacher_name]]->class_que_[i] = vector<int>(course_length);
//				for (int k = 0; k < course_length; k++) {
//					ClassUnit *clsu = new ClassUnit(*teachers_[teachers_map_[teacher_name]], i, class_name, courses_[j]->course_name_, courses_[j]->course_id_);
//					teachers_[teachers_map_[teacher_name]]->class_que_[i][k] = class_units_.size();
//					clsu->unit_id_ = class_units_.size();
//					class_units_.push_back(clsu);
//				}
//			}
//		}
//	}
//	
//	fin.close();
//
//	time_tables_ = vector<TimeTable>(class_num);
//	vector<ClassUnit *> ::iterator itc = class_units_.begin();
//	for (; itc != class_units_.end(); itc++) {
//		time_tables_[(*itc)->class_id_].class_que_.push_back(*(*itc));
//	}
//	
//	Out();
//	AddCant();
//	AddContinue();
//	AddItime();
//}

//void GA::Out() {
//	ofstream fout("out.txt");
//	vector<Teacher *> ::iterator itt = teachers_.begin();
//	for (; itt != teachers_.end(); itt++) {
//		fout << (*itt)->id_ << ' ' << (*itt)->teacher_name_ << "\n";
//	}
//
//	vector<Course *> ::iterator itc = courses_.begin();
//	for (; itc != courses_.end(); itc++) {
//		fout << (*itc)->course_id_ << ' ' << (*itc)->course_name_ << "\n";
//		itt = (*itc)->teacher_queue_.begin();
//		for (; itt != (*itc)->teacher_queue_.end(); itt++) {
//			fout << (*itt)->id_ << ' ' << (*itt)->teacher_name_ << "\n";
//		}
//	}
//
//	vector<ClassUnit *> ::iterator itu = class_units_.begin();
//	for (; itu != class_units_.end(); itu++) {
//		fout << (*itu)->class_id_ << ' ' << (*itu)->class_name_ << ' ' << (*itu)->teacher_.id_ << ' ' << (*itu)->teacher_.teacher_name_ << "\n";
//	}
//	for (int i = 0; i < time_tables_.size(); i++) {
//		fout << "class " << i << "\n";
//		for (int j = 0; j < time_tables_[i].class_que_.size(); j++) {
//			fout << time_tables_[i].class_que_[j].class_id_ << ' '
//				<< time_tables_[i].class_que_[j].class_name_ << ' '
//				<< time_tables_[i].class_que_[j].teacher_.teacher_name_ << "\n";
//		}
//	}
//	fout.close();
//}
//
//void GA::AddContinue() {
//	ifstream fin("continue.txt");
//	int class_id, course_id, continue_num, nrows;
//	fin >> nrows;
//	for (int i = 0; i < nrows; i++) {
//		fin >> class_id >> course_id >> continue_num;
//		time_tables_[class_id].AddContinue(course_id, continue_num);
//	}
//	fin.close();
//}
//
//void GA::AddCant() {
//	ifstream fin("cant.txt");
//	int nrows, class_id, course_id, num, x, y;
//	fin >> nrows;
//	for (int i = 0; i < nrows; i++) {
//		fin >> class_id >> course_id >> num;
//		vector<pair<int, int> > cant_time = vector<pair<int, int> > (num);
//		for (int j = 1; j < num; j++) {
//			fin >> x >> y;
//			cant_time[j].first = x;
//			cant_time[j].second = y;
//		}
//		time_tables_[class_id].AddCant(course_id, cant_time);
//	}
//	fin.close();
//}
//
//void GA::AddItime() {
//	ifstream fin("itime.txt");
//	int nrows, class_id, course_id, num, x, y;
//	fin >> nrows;
//	for (int i = 0; i < nrows; i++) {
//		fin >> class_id >> course_id >> num;
//		vector<pair<int, int> > itime = vector<pair<int, int> >(num);
//		for (int j = 0; j < num; j++) {
//			fin >> x >> y;
//			itime[j].first = x;
//			itime[j].second = y;
//		}
//		time_tables_[class_id].AddItime(course_id, itime);
//	}
//	fin.close();
//}

GA::GA(map<string, int> teachers_map, map<string, int> courses_map,
	vector<Teacher *> teachers, vector<ClassUnit *> class_units,
	vector<Course *> courses, vector<TimeTable> time_tables):
	teachers_map_(teachers_map), courses_map_(courses_map),
	teachers_(teachers), class_units_(class_units),
	courses_(courses), time_tables_(time_tables)
{}
GA::~GA() {
	vector<Teacher *> ::iterator itt = teachers_.begin();
	for (; itt != teachers_.end(); itt++) {
		delete (*itt);
	}

	vector<ClassUnit *> ::iterator itu = class_units_.begin();
	for (; itu != class_units_.end(); itu++) {
		delete (*itu);
	}
	
	vector<Course *> ::iterator itc = courses_.begin();
	for (; itc != courses_.end(); itc++) {
		delete (*itc);
	}
}

void GA::Init() {
	population_ = 300;
	for (int i = 0; i < 2; i++) {
		generation[i] = vector<Schedule>(population_);
	}
	fits = vector<double>(population_, 0);
	for (int i = 0; i < population_; i++) {
		generation[0][i].Init(teachers_map_, courses_map_, teachers_, class_units_, courses_, time_tables_);
	}
}

void GA::Generate() {
	Init();
}

void GA::Select() {

}

void GA::Cross() {

}

void GA::Mutate() {

}

void GA::CalFit() {

}