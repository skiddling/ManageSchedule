#include "schedule.h"
#include "InterruptibleThread.h"

int flag;
mutex mtx;

Schedule::Schedule(){
	e_ = default_random_engine(time(NULL));
}

Schedule::Schedule(vector<Course> couque, vector<Teacher> teachers, vector<TimeTable> timetables, vector<ClassUnit> clsque):
	couque_(couque), teachers_(teachers), timetables_(timetables), clsque_(clsque){ 

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
		if (crash_ == 0) {
			break;
		}
		if (t2 - t1 > dur) {
			break;
		}
		try {
			interruption_point();
		}
		catch(const thread_interrupted& interrupt){
			break;
		}
		Cross();
		CalFitness();
		Modify();
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
		if ((c.hasbeenput_ == false) && !(c.ttbptr_->PutIntoTable(&c))) {
			cout << "false unit " << i << endl;
			return false;
		}
		i++;
		cout << i << endl;
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
	for (auto& c : clsque_)
		crash_ += c.CalFitness();
}


void Schedule::Modify() {
	//先判冲突再解决冲突
	pair<int, int> tmp;
	for (auto& c : clsque_) {
		//if ((!c.CheckPeriod(c.stime_)) || (!c.teacher_->CheckUnit(&c, c.stime_))) {
		//新版本的判断操作
		for (auto i = 0; i < c.duration_; i++) {
			tmp = make_pair(c.stime_.first, c.stime_.second + i);
			if (c.CheckTimeIllegal(tmp)) {
				//发现需要进行调换	
				NeedToSwap(c);
				break;
			}
		}
	}

}

void Schedule::NeedToSwap(ClassUnit& firstcls) {
	//新版本去进行交换
	//1.获得相应的可以进行交换的时间
	//2.再进行交换
	vector<pair<int, int>> canbeput = firstcls.GetRandAvailTime();
	pair<int, int> tmp;
	//如果是预排课
	if (firstcls.preput_) {
		tmp = firstcls.pretime_;
	}
	else {
		bool flag;
		if (canbeput.empty())return;
		for (auto tim : canbeput) {
			flag = true;
			if (firstcls.CheckTimeIllegal(tim))flag = false;
			//如果是连堂课
			if (firstcls.duration_) {
				for (auto i = 1; i < firstcls.duration_; i++) {
					tmp = make_pair(tim.first, tim.second + i);
					if (firstcls.CheckTimeIllegal(tmp)) {
						flag = false;
						break;
					}
				}
			}
			else if (!firstcls.union_cls_index_.empty()) {
				for (auto& c : firstcls.unioncls_) {
					if (c->CheckTimeIllegal(tim)) {
						flag = false;
						break;
					}
				}
			}
			//找到合适的时间
			if (flag) {
				tmp = tim;
				break;
			}
		}
	}
	SwapClsUnit(firstcls);
}

void Schedule::Cross() {
	/*
	这里比modify少判断教师上课情况，
	也就是这里只判断该节课自己的能否被安排的情况，
	*/
	double cp;
	uniform_real_distribution<double> u(0.0, 1.0);
	for (auto& c : clsque_) {
		cp = u(e_);
		if (cp < mxpocross_) {
			SwapClsUnit(c);
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

