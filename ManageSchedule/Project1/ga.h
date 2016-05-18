#pragma once
#include "schedule.h"

class GA
{
public:
	int population_;
	const double mx_pof_mutate_ = 0.02;
	const double c_pof_mutate_ = 0.01;
	const double mx_pof_mutate_gene_ = 0.3;
	const double c_pof_mutate_gene_ = 0.1;
	const double c_pof_cross_ = 0.2;
	double max_fit_;
	double pof_mutate_;
	double pof_mutate_gene_;
	double pof_cross_;
	vector<Schedule> generation[2];
	vector<double> fits;//相当于是一个概率轮盘，把所有个体的适应度结合起来的一个轮盘
	Schedule res;
	map<string, int> teachers_map_;
	map<string, int> courses_map_;
	vector<Teacher *> teachers_;
	vector<ClassUnit *> class_units_;
	vector<Course *> courses_;
	vector<TimeTable> time_tables_;
	
	GA(map<string, int> teachers_map, map<string, int> courses_map,
		vector<Teacher *> teachers, vector<ClassUnit *> class_nuits,
		vector<Course *> courses, vector<TimeTable> time_table);
	~GA();
	//void Out();
	//void AddContinue();
	//void AddCant();
	//void AddItime();
	void Init();
	void Select();
	void Mutate();
	void Cross();
	void Generate();
	void CalFit();
	void Transform(Schedule &s);
};
