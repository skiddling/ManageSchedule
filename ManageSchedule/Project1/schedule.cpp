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
	//ÿ���α���п�����ʱ����Ҫ������Ӧ���޸�
	/*��Ϊ��ǰֻ��ʱ����׼ȷ�ģ�����classunit���е�ttbptr_
	��timetable_���е����е�ֵ����Ҫ��������
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
	//�������ÿ�����ܹ��ڷŵ�ʱ��
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
	//ÿ����ʦ�����Ӧ���Ͽ�ʱ��
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
	//���ÿ�����ҵľ���Ŀ�Ŀ�Ͽ�ʱ��
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
	//��������Ѿ������ݿ⵱�����
	//UpdatePtrs();
	//2.Ȼ����ÿ�ڿε�ʱ��
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
	//�˴�����ʦ�����ң���Ŀ��������ָ��ȫ����������Ӧ�ĸ���
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
	//��ʼ����������е�ָ��ĸ��µĹ���
	//����һ���ֳɿ�Ŀ����ʦ��������������������Ӧ��ָ��ĸ���
	GetPtrsFromClsSet(teachers_, clsque_);
	GetPtrsFromClsSet(timetables_, clsque_);
	GetPtrsFromClsSet(couque_, clsque_);
	for (auto& c : clsque_) {
		c.couptr_ = static_cast<Course*>(c.maptrs_["Course"]);
		c.teacher_ = static_cast<Teacher*>(c.maptrs_["Teacher"]);
		c.ttbptr_ = static_cast<TimeTable*>(c.maptrs_["TimeTable"]);
		//�ϰ�Ŀθ�����ָ��
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
	���ÿ�ڿ��Ƿ񶼷���Ҫ��
	*/
	for (auto& c : clsque_)
		crash_ += c.CalFitness();
}


void Schedule::Modify() {
	//���г�ͻ�ٽ����ͻ
	pair<int, int> tmp;
	for (auto& c : clsque_) {
		//if ((!c.CheckPeriod(c.stime_)) || (!c.teacher_->CheckUnit(&c, c.stime_))) {
		//�°汾���жϲ���
		for (auto i = 0; i < c.duration_; i++) {
			tmp = make_pair(c.stime_.first, c.stime_.second + i);
			if (c.CheckTimeIllegal(tmp)) {
				//������Ҫ���е���	
				NeedToSwap(c);
				break;
			}
		}
	}

}

void Schedule::NeedToSwap(ClassUnit& firstcls) {
	//�°汾ȥ���н���
	//1.�����Ӧ�Ŀ��Խ��н�����ʱ��
	//2.�ٽ��н���
	vector<pair<int, int>> canbeput = firstcls.GetRandAvailTime();
	pair<int, int> tmp;
	//�����Ԥ�ſ�
	if (firstcls.preput_) {
		tmp = firstcls.pretime_;
	}
	else {
		bool flag;
		if (canbeput.empty())return;
		for (auto tim : canbeput) {
			flag = true;
			if (firstcls.CheckTimeIllegal(tim))flag = false;
			//��������ÿ�
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
			//�ҵ����ʵ�ʱ��
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
	�����modify���жϽ�ʦ�Ͽ������
	Ҳ��������ֻ�жϸýڿ��Լ����ܷ񱻰��ŵ������
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
	//0����װ��ǰ�Ľڴ��飬1����װ�����Ľڴ���
	pair<int, int> tmp;
	for (auto i = 0; i < canbeput.size(); i++) {
		//timedelta����0����˵��Ҫ��������ģ�1�����Ҫ�������
		tmp.first = firstcls.stime_.first - canbeput[i].first;
		tmp.second = firstcls.stime_.second - canbeput[i].second;
		//��ʾ�ںϳɹ����ҽ�������
		if (UnionClsUnits(false, firstcls, tmp)) {
			//��ʾ�Ѿ��ɹ�����wait4swap���У�Ȼ��׼���໥֮��Ľ���
			break;
		}
	}
}

bool Schedule::UnionClsUnits(bool flag, ClassUnit& firstcls, pair<int, int> timedelta) {
	//׼������
	//wait4swap����װttb���е�ָ�룬������clsunit�ĵ�ַ
	vector<vector<ClassUnit**>> wait4swap{ 2 };
	//wait4swap[0].push_back(firstcls.ttbptr_->GetClsUnitPtr(firstcls.stime_.first, firstcls.stime_.second));
	vector<vector<ClassUnit**>::iterator> it;
	for (auto i = 0; i < 2; i++) {
		it.push_back(wait4swap[i].begin());
	}
	
	//ͨ��bfs�������н���union����
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
	//������ʾ�Ƿ�ɹ���ĳ�ڴεĿη�����е��У����������һ��ʧ������ξͶ�ʧ��
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
	//����ǰ����ڴεĿκ�����ڴζ�Ӧ�Ŀζ��ŵ��б���
	auto cls = *(*cptr);//cls����ڿ�
	//�ӵ�ǰ����ڴο�ʼ
	for (auto i = 0; i < cls.GetDuration(); i++) {
		//��ǰ�������
		if(!PutInSetVec(flag, cptr, timedelta, k, i, wait4swap, clstab))return false;
		//if(!PutInSetVec(cptr, timedelta, 1 - k, i, wait4swap, clstab))return false;
	}
	//���ж�����Ƿ��Ǻϰ��
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
	//�˺�������Ҫ�໥���������ڿζ�һ��Ž�ȥ
	//�˲�����Ҫ��������ƣ������������Ƿ��ܹ����Ź�ȥ
	//������ָ��汾
	auto cls = *cptr;//cls�ǵ�ǰ��ڿ�
	pair<int, int> tmp;
	//�����tmp��ʾ��Ҫ������ȥ��ʱ��
	if (k)tmp = pair<int, int>(cls->stime_.first + timedelta.first, cls->stime_.second + timedelta.second + add);
	else tmp = pair<int, int>(cls->stime_.first - timedelta.first, cls->stime_.second - timedelta.second + add);
	//target��ʾ��Ҫ���������ǽڿΣ������ǿ�ָ��
	auto target = cls->ttbptr_->roomtable_[tmp.first][tmp.second];
	pair<int, int> cperiod{ cls->stime_.first, cls->stime_.second + add };
	if (!CanBeSwap(flag, cls, cperiod, target, tmp))return false;

	//c��ʾҪ�ŵ�vec��set���е�����ڴ�
	auto c = &(cls->ttbptr_->roomtable_[cls->stime_.first][cls->stime_.second + add]);
	//target�����Ǹ���ָ��
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
	//���������Ҫ������set��vec���е����ڿ��ܷ��໥������
	if (!c->CheckPeriod(tperiod))return false;
	if (flag && !c->teacher_->CheckUnit(c, tperiod))return false;
	if (target != NULL && !(target->CheckPeriod(cperiod)))return false;
	if (flag && target != NULL && !(target->teacher_->CheckUnit(target, cperiod)))return false;
	return true;
}

void Schedule::SwapUnitsInVec(vector<vector<ClassUnit**>>& wait4swap) {
	//�Ѷ��е��еĽڴζ������໥����
	//ע�⵱�д��ڿ�ָ������
	for (auto i = 0; i < wait4swap.size(); i++) {
		auto origin = *(wait4swap[0][i]);
		auto target = *(wait4swap[1][i]);
		if(origin !=NULL && origin->headptr_ == wait4swap[0][i])
			origin->ttbptr_->SwapUnits(origin, target);
		else if(target !=NULL && target->headptr_ == wait4swap[1][i])
			target->ttbptr_->SwapUnits(origin, target);
		//���ڿ�ָ��
		if(target != NULL)*(wait4swap[0][i]) = &(*target);
		else *(wait4swap[0][i]) = NULL;
		if(origin != NULL)*(wait4swap[1][i]) = &(*origin);
		else *(wait4swap[1][i]) = NULL;
	}
}

