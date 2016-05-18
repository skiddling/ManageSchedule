#include "ga.h"

const int mxcrash = 100000000;
const int timeout = 7200000;
const int loctimeout = 600000;
const int crashout = 20;
const double mxmp = 0.03;
const double mxmpg = 0.25;
const double mxcp = 0.35;
const double step = 1.3;

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
	res = Schedule();
	res.crash_ = mxcrash;
	res.reward_ = 0;
	int t1 = clock(), t2 = t1, t3 = t1;
	int mxoff = timeout;
	int prvcrash = mxcrash;

	while (t2 - t1 < mxoff) {
		Select(), cout << "selected\n";
		Mutate(), cout << "mutated\n";
		Cross(), cout << "crossed\n";
		generation[0] = generation[1];
		CalFit();
		int micrash = 11111;
		for (int i = 0; i < population_; i++) {
			Transform(generation[0][i]);
			micrash = min(micrash, generation[0][i].crash_);
		}

		if (res.crash_ == 0)break;

		if (res.crash_ != prvcrash) {
			prvcrash = res.crash_;
			pof_mutate_ = c_pof_cross_;
			pof_mutate_gene_ = c_pof_mutate_gene_;
			t3 = clock();
		}

		t2 = clock();
		cout << res.crash_ << ' ' << micrash << ' ' << res.reward_ << ' ' << t1 << ' ' << t2 << ' ' << t3 << "\n";
		//遇上局部最优解无法逃出		
		if (t2 - t3 > loctimeout) {
			pof_mutate_ *= step;
			pof_mutate_gene_ *= step;
			pof_mutate_ = min(mx_pof_mutate_, pof_mutate_);
			pof_mutate_gene_ = min(mx_pof_mutate_gene_, pof_mutate_gene_);
			t3 = clock();
		}
		//初始化不够好或者产生的结果太糟糕也要重新初始化
		if (micrash > max(crashout, res.crash_ * 2)) {
			Init();
			t1 = clock();
			t3 = t1;
		}
		else {
			//如果运行时间太长
			if (t2 - t1 > mxoff && mxoff == timeout) {
				Init();
				t1 = clock();
				t3 = t1;
			}
		}
	}
}

void GA::Select() {

}

void GA::Cross() {

}

void GA::Mutate() {
	for (int i = 0; i < population_; i++) {
		double r = (double)rand() *rand() / kRandPlusRand;
		double mp = pof_mutate_ * max_fit_ / generation[1][0].fitness_;
		mp = min(mp, mxmp);
		if (r < mp) {
			mp = pof_mutate_gene_ * max_fit_ / generation[1][i].fitness_;
			mp = min(mp, mxmpg);
			generation[1][i].Mutate(mp);
		}
	}
}

void GA::CalFit() {

}

void GA::Transform(Schedule &s) {
	
}