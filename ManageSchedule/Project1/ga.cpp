#include "ga.h"

const int mxcrash = 100000000;
const int timeout = 7200000;
const int loctimeout = 600000;
const int crashout = 20;
const double mxmp = 0.03;//用于变异选择的概率
const double mxmpg = 0.25;//用于个体当中某一些部分的基因进行变异的概率
const double mxcp = 0.35;
const double step = 1.3;

GA::GA(map<string, int> teachers_map, map<string, int> courses_map,
	vector<Teacher> teachers, vector<Course> courses,
	vector<TimeTable> time_tables):
	teachers_map_(teachers_map), courses_map_(courses_map),
	teachers_(teachers), courses_(courses), 
	time_tables_(time_tables)
{}

void GA::Init() {
	cout << teachers_map_.size() << "  " << courses_map_.size() <<
		"  " << teachers_.size() << "  " << time_tables_.size() << endl;
	//system("PAUSE");
	//population_ = 300;
	population_ = 3;
	//c代表的是设定的概率值，不带c的表明的是代码运行当中用到的值
	pof_mutate_ = c_pof_mutate_;
	pof_mutate_gene_ = c_pof_mutate_gene_;
	pof_cross_ = c_pof_cross_;
	//滚动数组，generation[1]始终用作generation[1]的暂存变量
	//在所有的操作都始终对generation[0]进行操作
	for (int i = 0; i < 2; i++) {
		generation[i] = vector<Schedule>(population_);
	}
	//system("PAUSE");
	fits = vector<double>(population_, 0);
	for (int i = 0; i < population_; i++) {
		generation[0][i].Init(teachers_, courses_, time_tables_);
	}
	cout << "ga.init\n";
	for (int i = 0; i < population_; i++) {
		generation[0][i].CalRes();
		Transform(generation[0][i]);
	}
	//cout << "GA.init\n";
	//test(generation[0]);
	//system("PAUSE");
}

void GA::Generate() {
	res = Schedule();
	//crash和reward都是要最小最好，crash代表冲突个数，reward代表占用的半天的个数
	res.crash_ = mxcrash;
	res.reward_ = mxcrash;
	Init();
	int t1 = clock(), t2 = t1, t3 = t1;
	int mxoff = timeout;
	int prvcrash = mxcrash;
	cout << "start generate\n";
	cout << t1 << endl;
	//system("PAUSE");
	while (t2 - t1 < mxoff) {
		//会把0组的选择结果送到1组当中,再传回来
		Select(); 
		cout << "selected\n";
		Mutate(); 
		cout << "mutated\n";
		//cross之后会把1组的结果送到0组当中
		Cross(); 
		cout << "crossed\n";
		//test(generation[1]);
		//generation[0] = generation[1];
		//test(generation[0]);
		Modify();
		//break;
		CalFit();
		break;
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
			//重新设定起点，但是不必要重新回到起点
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
	//int mxreward = 1;
	max_fit_ = 0;
	//for (int i = 0; i < population_; i++) {
	//	if (generation[0][i].crash_ <= 0)
	//		//mxreward = max(mxreward, generation[0][i].reward_);
	//		mxreward = min(mxreward, generation[0][i].reward_);
	//}
	for (int i = 0; i < population_; i++) {
		generation[0][i].CalFitness();
		fits[i] = generation[0][i].fitness_;
		max_fit_ = max(max_fit_, generation[0][i].fitness_);
		//轮盘赌，形成总的适应度，然后让总的适应度成为轮盘赌的总值，然后计算每个适应度在当中所占的比例
		if (i > 0)fits[i] += fits[i - 1];
	}
	for (int i = 0; i < population_; i++) {
		fits[i] = fits[i] / fits.back();
	}
	for (int i = 0; i < population_; i++) {
		//尽可能的分出各种概率的大小
		double r = (double)rand() * rand() / kRandPlusRand;
		int id = lower_bound(fits.begin(), fits.end(), r) - fits.begin();
		//根据轮盘赌的结果来进行排序选择,选出来的可以是有重复的，但是就是要选出相同数量
		generation[1][i] = generation[0][id];
		//cout << i << ' ' << id << endl;
	}
	//将信息又重新传回给generation[0]当中
	cout << "ga.select.s\n";
	generation[0] = generation[1];
	cout << "ga.selece\n";
}

void GA::Cross() {
	for (int i = 0; i < population_; i++) {
		double rd = (double)rand() * rand() / kRandPlusRand;
		double mp = pof_cross_ * max_fit_ / generation[0][i].fitness_;
		mp = min(mp, mxcp);
		if (rd < mp) {
			double r = (double)rand() * rand() / kRandPlusRand;
			int j = lower_bound(fits.begin(), fits.end(), r) - fits.begin();
			if (i == j) {
				j = (j + 1) % population_;
			}
			generation[0][i].Cross(generation[0][j], mp);
		}
	}

}

void GA::Mutate() {
	for (int i = 0; i < population_; i++) {
		double r = (double)rand() *rand() / kRandPlusRand;
		//暂时将变异概率放大一定倍数 max_fit_ / generation[1][i].fitness_
		//适应度低的个体更需要进行变异,但是最大的变异概率不能超过mxmp
		double mp = pof_mutate_ * max_fit_ / generation[0][i].fitness_;
		mp = min(mp, mxmp);
		//变异是在所有个体当中随机选择一个个体进行变异
		//转到实现当中就是以一定的概率进行选择某一些个体
		//然后对于选中的个体当中以一定的概率随进对某些位置进行变异
		if (r < mp) {
			//类似于上面的变异的概率
			mp = pof_mutate_gene_ * max_fit_ / generation[0][i].fitness_;
			mp = min(mp, mxmpg);
			generation[0][i].Mutate(mp);
		}
	}
}

void GA::Modify() {
	for (int i = 0; i < population_; i++) {
		generation[0][i].Modify();
	}
	//system("PAUSE");
}

void GA::CalFit() {
	for (int i = 0; i < population_; i++) {
		generation[0][i].CalRes();
	}
}

void GA::Transform(Schedule &s) {
	if (res.crash_ > s.crash_ || (res.crash_ == s.crash_ && res.reward_ > s.reward_))
		res = s;
}