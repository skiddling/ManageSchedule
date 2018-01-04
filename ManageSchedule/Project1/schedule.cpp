#include "schedule.h"
#include "InterruptibleThread.h"

int flag = 1;
mutex mtx;

Schedule::Schedule(){
	e_ = default_random_engine(time(NULL));
}

Schedule::Schedule(vector<Course> couque, vector<Teacher> teachers, vector<TimeTable> timetables, vector<ClassUnit> clsque):
	couque_(couque), teachers_(teachers), timetables_(timetables), clsque_(clsque){ 
	int d = timetables[0].days_, p = timetables[0].periods_;
	for (auto i = 0; i < d; i++) {
		for (auto j = 0; j < p; j++) {
			allunits_.insert(make_pair(i, j));
		}
	}
	for (auto i = 0; i < clsque_.size(); i++) {
		clsque_[i].allunits_ = allunits_;
	}
}


Schedule::Schedule(const Schedule& s):
	crash_(s.crash_), 
	outtime_(s.outtime_),
	timetables_(s.timetables_),
	clsque_(s.clsque_),
	teachers_(s.teachers_),
	e_(s.e_){
	//每个课表进行拷贝的时候需要进行相应的修改
	/*因为当前只有时间是准确的，对于classunit当中的ttbptr_
	和timetable_当中的所有的值都需要进行修正
	*/
	UpdatePtrs();
}

Schedule & Schedule::operator=(const Schedule & s){
	crash_ = s.crash_;
	outtime_ = s.outtime_;
	timetables_ = s.timetables_;
	clsque_ = s.clsque_;
	teachers_ = s.teachers_;
	e_ = s.e_;
	UpdatePtrs();
	return *this;
}

void Schedule::GetSchedule(InterruptibleThread * t, future<Schedule>* fut) {
	auto t1 = chrono::system_clock::now(), t2 = t1;
	chrono::duration<int, ratio<60, 1>> dur(outtime_);
	
	while (true) {
		t2 = chrono::system_clock::now();
		CalFitness();
		cout << "thread id is " << this_thread::get_id() << " carsh is " << crash_ << endl;
		if (crash_ == 0) {
			flag = 2;
			break;
		}
		//cout << "end of first if statement" << endl;

		if (t2 - t1 > dur) {
			flag = 1;
			break;
		}
		//cout << "end of second if statement" << endl;
		try {
			interruption_point();
			//cout << "end of try" << endl;
		}
		catch(const thread_interrupted& interrupt){
			flag = 0;
			break;
		}
		//cout << "start cross" << endl;
		Cross();
		//cout << "thread id is " << this_thread::get_id() << " end of cross" << endl;
		CalFitness();
		//cout << "start modify" << endl;
		Modify();
		//cout << "thread id is " << this_thread::get_id() << " end of modify" << endl;
	}
	if (flag) {
		mtx.lock();
		if (flag && crash_ == 0) {
			flag = 0;
			t->pro_ptr_->set_value(*this);
		}
		mtx.unlock();
	}
}

void Schedule::GetUnitsAvailableTime() {
	//暴力获得每个课能够摆放的时间
	int days = timetables_[0].days_, periods = timetables_[0].periods_;
	pair<int, int> tmp;
	for (auto& c : clsque_) {
		for (auto i = 0; i < days; i++) {
			for (auto j = 0; j < periods; j++) {
				tmp = make_pair(i, j);
				if (c.canntbeput_.find(tmp) == c.canntbeput_.end()) {
					c.canbeput_.insert(tmp);
				}
			}
		}
	}
}

void Schedule::GetTeachTime() {
	//每个老师获得相应的上课时间
	int days = timetables_[0].days_, periods = timetables_[0].periods_;
	for (auto& t : teachers_) {
		t.teach_time_ = vector<vector<int>>(days, vector<int>(periods, 0));
	}
	for (auto& c : clsque_) {
		for (auto i = 0; i < c.duration_; i++) {
			c.teacher_->teach_time_[c.stime_.first][c.stime_.second + i]++;
		}
	}
}

void Schedule::GetRoomCourseTime() {
	//获得每个教室的具体的科目上课时间
	int days = timetables_[0].days_;
	vector<int> tmp(days, 0);
	for (auto& c : clsque_) {
		if (c.ttbptr_->course_time_.find(c.couptr_->dbid_) == c.ttbptr_->course_time_.end()) {
			c.ttbptr_->course_time_[c.couptr_->dbid_] = tmp;
		}
		c.ttbptr_->course_time_[c.couptr_->dbid_][c.stime_.first]++;
	}
}

bool Schedule::init() {
	//这个部分已经在数据库当中完成
	//UpdatePtrs();
	//2.然后安排每节课的时间
	int i = 0;
	for (auto& c : clsque_) {
		i++;
		cout << i << endl;
		c.scptr_ = this;
		if ((c.hasbeenput_ == false) && !(c.ttbptr_->PutIntoTable(&c))) {
			cout << "false unit " << i << endl;
			return false;
		}
	}
	GetUnitsAvailableTime();
	GetTeachTime();
	GetRoomCourseTime();
	cout << "end of init table" << endl;

}

template<typename T>
void GetPtrsFromClsSet(vector<T>& vec, vector<ClassUnit>& clsque) {
	//此处将教师，教室，科目三个类别的指针全部都进行相应的更新
	for (auto& v : vec) {
		v.clsque_.clear();
		for (auto& t : v.clsqueindex_) {
			v.clsque_.push_back(&(clsque[t]));
			//*(static_cast<v.GetName()>(unitset[t].maptrs[v.GetName()])) = &t;
			clsque[t].maptrs_[v.GetName()] = &v;
		}
	}
}

void Schedule::UpdatePtrs() {
	//开始进行深拷贝当中的指针的跟新的工作
	//这里一共分成科目，老师，教室三部分来进行相应的指针的跟新
	GetPtrsFromClsSet(teachers_, clsque_);
	GetPtrsFromClsSet(timetables_, clsque_);
	GetPtrsFromClsSet(couque_, clsque_);
	for (auto& c : clsque_) {
		c.couptr_ = static_cast<Course*>(c.maptrs_["Course"]);
		c.teacher_ = static_cast<Teacher*>(c.maptrs_["Teacher"]);
		c.ttbptr_ = static_cast<TimeTable*>(c.maptrs_["TimeTable"]);
		//合班的课给补上指针
		if (!c.union_cls_index_.empty()) {
			c.unioncls_.clear();
			for (auto index : c.union_cls_index_) {
				c.unioncls_.push_back(&clsque_[index]);
			}
		}
	}
	
}

void Schedule::CalFitness() {
	/*
	检查每节课是否都符合要求
	*/
	crash_ = 0;
	for (auto& c : clsque_) {
		crash_ += c.GetCrash();
	}
	cout << "end of get crash" << endl;
}


void Schedule::Modify() {
	//先判冲突再解决冲突
	pair<int, int> tmp;
	for (auto& c : clsque_) {
		//if ((!c.CheckPeriod(c.stime_)) || (!c.teacher_->CheckUnit(&c, c.stime_))) {
		//新版本的判断操作
		for (auto i = 0; i < c.duration_; i++) {
			tmp = make_pair(c.stime_.first, c.stime_.second + i);
			//if (c.CheckTimeIllegal(c.stime_, tmp)) {
			if(c.CheckTimeCrash()){
				//发现需要进行调换	
				NeedToSwap(c, 1);
				break;
			}
		}
	}

}

void Schedule::NeedToSwap(ClassUnit& firstcls, int tag) {
	//预排课不进行任何的交换
	if (firstcls.preput_)return;
	//新版本去进行交换
	//1.获得相应的可以进行交换的时间，这里相互调换指的是换过去和换过来的课都是合理的
	//如果是预排课那么就不需要考虑换过去的课是否是合理的
	//2.再进行交换
	vector<pair<int, int>> canbeput;
	/*if (firstcls.preput_) {
		if(firstcls.stime_ == firstcls.pretime_)return;
		else canbeput.push_back(firstcls.pretime_);
	}
	else canbeput = firstcls.GetRandAvailTime();*/
	if (tag) {
		//如果是modify操作，那就要进行可以挑选的时间进行选择
		//这里需要对连堂的时间做一个放宽
		canbeput = firstcls.GetRandAvailTime();
	}
	else {
		canbeput = firstcls.GetRandTime();
	}
	//新老结合版本，bfs得到所有的交换序列，pair获得尝试结果
	//会得到一个2排的数组，0表示当前待交换的节次，1组表示目标需要交换的节次
	//第一个bool当中true表示成功，所有组都可以进行交换，false表示这个delta不行
	//解释一下为什么这里是二级指针，因为在系统当中连堂课其实是一次课，而这个在做交换的时候不会在一个队列当中出现两次，
	//所以我们交换其实是timetable当中的指针的值，所以我们要用二级指针指向timetable当中的一级指针
	//12.20版本当中交换的内容更具体一些，是pair<TimeTable*, pair<int, int>>，也就是具体指出是哪个课表
	//和哪个时间进行交换，因为在获得这个队列的过程当中需要进行节次的空间上的判断
	//pair<bool, vector<vector<ClassUnit**>>> ret;
	pair<bool, vector<vector<Node>>> ret;
	pair<int, int> delta;
	for (auto& t : canbeput) {
		delta = make_pair(firstcls.stime_.first - t.first, firstcls.stime_.second - t.second);
		ret = GetUnionUnitsVec(&firstcls, delta, tag);
		if (ret.first)break;
	}
	//if (ret.first)SwapUnits(ret.second);
	if (ret.first)SwapUnitsVec(ret.second);
}

//以下这两个函数其实在cross当中也是同样适用的
//最终的规则就是：连堂课只能进行不同天的交换，而非连堂课的可以进行任意的交换
//连堂课只进行stime的判断
pair<bool, vector<vector<Node>>> Schedule::GetUnionUnitsVec(ClassUnit* firstunitptr, pair<int, int> delta, int tag) {
	//利用bfs获得相应的所有这次需要换的节次
	//在队列当中的节次表示是能够进行交换的节次，如果在新增过程当中发现不能进行交换的那就返回失败
	//typedef pair<ClassUnit*, pair<int, int>> Node;
	vector<vector<Node>> vec(2);
	//pair<bool, vector<vector<ClassUnit**>>> res(make_pair(false, vec));
	//vector<vector<Node>::iterator> it{ vec[0].begin(), vec[1].begin() };
	vector<int> it{0, 0};
	//初始化把两个队列的最开始的两个给添加进去
	//连堂也默认从第一个时间开始，因为后面会进行剩余时间的判断
	set<Node> unitset;
	bool ret = CheckPutIntoVec(firstunitptr->ttbptr_, firstunitptr->stime_, 0, vec, unitset, delta, tag);
	if (ret == false)return make_pair(false, vec);
	ClassUnit **tbptr, *cptr, *tmp;
	pair<int, int> tim;
	//while (it[0] != vec[0].end() && it[1] != vec[1].end()) {
	while (it[0] < vec[0].size() && it[1] < vec[1].size()) {
		//通过指针指向的节次来添加对应的节次
		for (auto i = 0; i < 2; i++) {
			//整个的顺序是先扩展当前的节次，也就是如果当前节次是连堂或者合班那么先把这些课加到当前的队列当中
			//扩展完当前的节次之后，再在对面添加当前节次的对应节次，再一直向右扩展，一直到队列尾部
			//然后再到对面队列当中进行以上动作
			//之所以需要按照以上的运动轨迹是因为要保证在连堂和合班这两个情况当中都能没有bug的运行
			//而且要保证对应位置的两个指针是正好对应的两个节次
			//每次扩展的实际操作其实就是获得相应的一个课表的指针，然后装入set当中用来去重
			//对于连堂课，我们只会对stime对应的opt进行检查，而其他时间段的不会进行检查
			//while (it[i] != vec[i].end()) {
			while (it[i] < vec[i].size()) {
				//1.先扩展当前的节次
				//先判是否是连堂
				//cptr = *(*it[i]);
				//cptr = (*it[i]).first->roomtable_[(*it[i]).second.first][(*it[i]).second.second];
				cptr = (vec[i][it[i]]).first->roomtable_[(vec[i][it[i]]).second.first][(vec[i][it[i]]).second.second];
				if (cptr != nullptr) {
					//如果是连堂课
					if (cptr->duration_) {
						//如果是同一天则表示不嫩进行交换
						if (delta.first == 0) {
							return make_pair(false, vec);
						}
						//不是同一天的	
						for (auto j = 0; j < cptr->duration_; j++) {
							//这个节次的时间
							tim = make_pair(cptr->stime_.first, cptr->stime_.second + j);
							ret = CheckPutIntoVec(cptr->ttbptr_, tim, i, vec, unitset, delta, tag);
						}
					}
					else if (!cptr->union_cls_index_.empty()) {
						//表示如果是合班课
						for (auto j = 0; j < cptr->union_cls_index_.size(); j++) {
							tmp = cptr->unioncls_[j];
							ret = CheckPutIntoVec(tmp->ttbptr_, tmp->stime_, i, vec, unitset, delta, tag);
						}
					}
					if (ret == false)return make_pair(false, vec);
				}
				it[i]++;
			}
		}
	}
	return make_pair(true, vec);
}

//这当中需要对节次做时间和空间两个方面的检查
//最终规则就是连堂课只能进行不同天的交换，而非连堂课的可以进行任意时间段的交换
//这里要考虑到cross还是modify操作
//return false 表示不行，true表示可以
bool Schedule::CheckPutIntoVec(TimeTable* tbptr, pair<int, int> origin, int pos, vector<vector<Node>>& vec, set<Node>& unitset, pair<int, int> delta, int tag) {
	ClassUnit *cptr = tbptr->roomtable_[origin.first][origin.second];
	Node node = make_pair(tbptr, origin);
	if (unitset.find(node) != unitset.end())return true;
	auto opt = GetOpposeTime(pos, origin, delta);
	if (cptr != nullptr) {
		//预排不能换
		if (cptr->preput_)return false;
		//连堂课必须进行不同天的交换
		if (cptr->duration_ > 1 && delta.first == 0)return false;
		//先对自己做时间和空间两方面的检查，再检查对方的这两方面
		//auto a = make_pair(tbptr, origin), b = make_pair(tbptr, opt);
		//最终版本不再做空间的检查
		//检查空间是否符合要求
		//if (CheckRoomIllegal(tbptr, origin, opt, 0))return false;
		//if (cptr1->CheckTimeIllegal(origin, opt, tag) || cptr2->CheckTimeIllegal(opt, origin, tag))return false;
		if (cptr->CheckTimeIllegal(origin, opt, tag))return false;
	}
	vec[pos].push_back(node);
	unitset.insert(node);
	return CheckPutIntoVec(tbptr, opt, 1 - pos, vec, unitset, delta, tag);
	//ClassUnit** cptr = &(tbptr->roomtable_[origin.first][origin.second]);
	//pair<int, int> opt = GetOpposeTime(pos, origin, delta);
	////如果这个节次已经被放入了，那么它和它本身对应的节次都已经被放入了，所以不用再继续
	//if (unitset.find(cptr) != unitset.end())return true;
	////检查这个节次能不能被放到对面
	//if (*cptr == nullptr)return CheckPutIntoVec(tbptr, opt, 1 - pos, vec, unitset, delta);
	//if ((*cptr)->CheckTimeIllegal(opt))return false;
	//vec[pos].push_back(cptr);
	//unitset.insert(cptr);
	//return CheckPutIntoVec(tbptr, opt, 1 - pos, vec, unitset, delta);
}

//cnt用来计数次数，这个函数用来检测origin这个节次能不能换到opt
//这里会特判同一天的节次，也就是对于同一天内的课进行交换的时候，遇到连堂课其实是一种平移
//而这种平移能否成功其实就是判断交换的两者之间的距离是否足够大，也就是核心公式
//abs(period1 - period2) >= max(size1, size2)
//但是做了这种平移之后，在交换的时候这个连堂课就要进行相应的stime的更新
//更新部分用的是最小交换策略，也就是在同一天的交换的时候只要关注交换位置之后的节次就可以，也就是管尾不管头，头部在修正的时候会进行维护
//这个函数专门用来检测连堂课用的
bool Schedule::CheckRoomIllegal(TimeTable * tbptr, pair<int, int> origin, pair<int, int> opt, int cnt) {
	ClassUnit *cptr1 = tbptr->roomtable_[origin.first][origin.second], *cptr2 = tbptr->roomtable_[opt.first][opt.second];
	if (cnt == 2)return false;
	if (cptr1 == nullptr || cptr1->duration_ == 1) {
		return CheckRoomIllegal(tbptr, opt, origin, cnt + 1);
	}
	//有连堂的，先判断是否是同一天的，然后检测连堂的第一节课和最后一节课是否超过时间限制
	int sz = cptr2 == nullptr ? 0 : cptr2->duration_;
	if (origin.first == opt.first) {
		if (abs(origin.second - opt.second) < max(cptr1->duration_, sz))return true;
	}
	//判断空间上是否合理
	if (opt.second < 0 || opt.second >= tbptr->periods_)return true;
	return CheckRoomIllegal(tbptr, opt, origin, cnt + 1);
}

void Schedule::SwapUnitsVec(vector<vector<Node>> vec) {
	//对换相应的节次
	TimeTable* ttbptr;
	pair<int, int> ori, opt;
	for (auto i = 0; i < vec[0].size(); i++) {
		ttbptr = vec[0][i].first;
		ori = vec[0][i].second;
		opt = vec[1][i].second;
		UpdateUnitPrt(ttbptr, vec[0][i].second, vec[1][i].second);
		UpdateUnitPrt(ttbptr, vec[1][i].second, vec[0][i].second);
		swap(ttbptr->roomtable_[ori.first][ori.second], ttbptr->roomtable_[opt.first][opt.second]);
	}
}

void Schedule::UpdateUnitPrt(TimeTable * ttbptr, pair<int, int> ori, pair<int, int> opt) {
	auto unitptr = ttbptr->roomtable_[ori.first][ori.second];
	//连堂课判断
	if (unitptr != nullptr) {
		unitptr->teacher_->teach_time_[ori.first][ori.second]--;
		unitptr->teacher_->teach_time_[opt.first][opt.second]++;

		if(unitptr->duration_ > 1 && unitptr->stime_ != ori)return;
		(ttbptr->course_time_)[unitptr->couptr_->dbid_][ori.first]--;
		(ttbptr->course_time_)[unitptr->couptr_->dbid_][opt.first]++;
		unitptr->stime_ = opt;
	}
}


void Schedule::SwapUnits(vector<vector<ClassUnit**>>& units) {
	
}

pair<int, int> Schedule::GetOpposeTime(int pos, pair<int, int> tim, pair<int, int> delta) {
	//s - t = d => t = s - d, s = t + d
	//s == 0, t == 1
	if (pos == 0)return pair<int, int>(tim.first - delta.first, tim.second - delta.second);
	else return pair<int, int>(tim.first + delta.first, tim.second + delta.second);
}

void Schedule::Cross() {
	/*
	这里比modify少判断教师上课情况，
	也就是这里只判断该节课自己的能否被安排的情况，
	*/
	double cp;
	uniform_real_distribution<double> u(0.0, 1.0);
	//for (auto& c : clsque_) {
	//cout << clsque_.size() << endl;
	for(auto i = 0; i < clsque_.size(); i++){
		cp = u(e_);
		//cout << i << " " << cp << endl;
		if (cp < mxpocross_) {
			//SwapClsUnit(c);
			//cout << "start cross " << i << endl;
			NeedToSwap(clsque_[i], 0);
			//cout << "end cross " << i << endl;
		}
	}

}

void Schedule::SwapClsUnit(ClassUnit& firstcls) {
	vector<pair<int, int>> canbeput = firstcls.GetRandAvailTime();
	if (canbeput.empty())return;
	//0用来装当前的节次组，1用来装将来的节次组
	pair<int, int> tmp;
	for (auto i = 0; i < canbeput.size(); i++) {
		//timedelta对于0组来说是要进行相减的，1组的是要进行相加
		tmp.first = firstcls.stime_.first - canbeput[i].first;
		tmp.second = firstcls.stime_.second - canbeput[i].second;
		//表示融合成功并且交换结束
		if (UnionClsUnits(false, firstcls, tmp)) {
			//表示已经成功放入wait4swap当中，然后准备相互之间的交换
			break;
		}
	}
}

bool Schedule::UnionClsUnits(bool flag, ClassUnit& firstcls, pair<int, int> timedelta) {
	//准备工作
	//wait4swap用来装ttb当中的指针，而不是clsunit的地址
	vector<vector<ClassUnit**>> wait4swap{ 2 };
	//wait4swap[0].push_back(firstcls.ttbptr_->GetClsUnitPtr(firstcls.stime_.first, firstcls.stime_.second));
	vector<vector<ClassUnit**>::iterator> it;
	for (auto i = 0; i < 2; i++) {
		it.push_back(wait4swap[i].begin());
	}
	
	//通过bfs整个队列进行union操作
	set<ClassUnit**> clstab;
	if (!AddUnitPtrIntoVec(flag, firstcls.headptr_, timedelta, 0, wait4swap, clstab))return false;
	//clstab.insert(firstcls.ttbptr_->GetClsUnitPtr(firstcls.stime_.first, firstcls.stime_.second));
	//lambda
	auto fun = [&]()-> bool {
		for (auto i = 0; i < 2; i++) {
			if (it[i] != wait4swap[i].end())
				return true;
		}
		return false;
	};
	//用来标示是否成功把某节次的课放入队列当中，如果当中有一个失败则这次就都失败
	while (fun()) {
		for (auto i = 0; i < 2; i++) {
			if (it[i] != wait4swap[i].end()) {
				if ((*(it[i]) != NULL) && (!AddUnitPtrIntoVec(flag, *(it[i]), timedelta, i, wait4swap, clstab))) {
					return false;
				}
				it[i]++;
			}
		}
	}
	SwapUnitsInVec(wait4swap);
	return true;
}

bool Schedule::AddUnitPtrIntoVec(bool flag, ClassUnit** cptr, pair<int, int> timedelta,
	int k, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab) {
	//将当前这个节次的课和这个节次对应的课都放到列表当中
	auto cls = *(*cptr);//cls是这节课
	//从当前这个节次开始
	for (auto i = 0; i < cls.GetDuration(); i++) {
		//当前这个部分
		if(!PutInSetVec(flag, cptr, timedelta, k, i, wait4swap, clstab))return false;
		//if(!PutInSetVec(cptr, timedelta, 1 - k, i, wait4swap, clstab))return false;
	}
	//再判断这个是否是合班课
	if (cls.unioclsid_.size()) {
		for (auto& uc : cls.unioncls_) {
			//if (!PutInSetVec(flag, clsque_[uc].headptr_, timedelta, k, 0, wait4swap, clstab))return false;
			if (!PutInSetVec(flag, uc->headptr_, timedelta, k, 0, wait4swap, clstab))return false;
			//if (!PutInSetVec(uc->headptr_, timedelta, 1 - k, 0, wait4swap, clstab))return false;
		}
	}
	return true;
}

bool Schedule::PutInSetVec(bool flag, ClassUnit ** cptr, pair<int, int> timedelta,
	int k, int add, vector<vector<ClassUnit**>>& wait4swap, set<ClassUnit**>& clstab) {
	//此函数将需要相互交换的两节课都一起放进去
	//此部分需要加入检查机制，检查调换区域是否能够被放过去
	//都改正指针版本
	auto cls = *cptr;//cls是当前这节课
	pair<int, int> tmp;
	//这里的tmp表示将要被调过去的时间
	if (k)tmp = pair<int, int>(cls->stime_.first + timedelta.first, cls->stime_.second + timedelta.second + add);
	else tmp = pair<int, int>(cls->stime_.first - timedelta.first, cls->stime_.second - timedelta.second + add);
	//target表示需要被交换的那节课，可能是空指针
	auto target = cls->ttbptr_->roomtable_[tmp.first][tmp.second];
	pair<int, int> cperiod{ cls->stime_.first, cls->stime_.second + add };
	if (!CanBeSwap(flag, cls, cperiod, target, tmp))return false;

	//c表示要放到vec和set当中的这个节次
	auto c = &(cls->ttbptr_->roomtable_[cls->stime_.first][cls->stime_.second + add]);
	//target可能是个空指针
	auto tp = &(cls->ttbptr_->roomtable_[tmp.first][tmp.second]);
	if (clstab.find(c) == clstab.end()) {
		clstab.insert(c);
		clstab.insert(tp);
		wait4swap[k].push_back(c);
		wait4swap[1 - k].push_back(tp);
	}
	return true;
}

bool Schedule::CanBeSwap(bool flag, ClassUnit* c, pair<int, int> cperiod, ClassUnit* target, pair<int, int> tperiod) {
	//检查两个将要被放入set和vec当中的两节课能否相互被交换
	if (!c->CheckPeriod(tperiod))return false;
	if (flag && !c->teacher_->CheckUnit(c, tperiod))return false;
	if (target != NULL && !(target->CheckPeriod(cperiod)))return false;
	if (flag && target != NULL && !(target->teacher_->CheckUnit(target, cperiod)))return false;
	return true;
}

void Schedule::SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap) {
	//把队列当中的节次都进行相互交换
	//注意当中存在空指针的情况
	for (auto i = 0; i < wait4swap.size(); i++) {
		auto origin = *(wait4swap[0][i]);
		auto target = *(wait4swap[1][i]);
		if(origin !=NULL && origin->headptr_ == wait4swap[0][i])
			origin->ttbptr_->SwapUnits(origin, target);
		else if(target !=NULL && target->headptr_ == wait4swap[1][i])
			target->ttbptr_->SwapUnits(origin, target);
		//存在空指针
		if(target != NULL)*(wait4swap[0][i]) = &(*target);
		else *(wait4swap[0][i]) = NULL;
		if(origin != NULL)*(wait4swap[1][i]) = &(*origin);
		else *(wait4swap[1][i]) = NULL;
	}
}

