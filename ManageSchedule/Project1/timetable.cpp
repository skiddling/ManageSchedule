#include "timetable.h"

int TimeTable::days_per_week_ = 0;
int TimeTable::period_in_moring_ = 0;
int TimeTable::period_per_day_ = 0;
map<string, int> TimeTable::courses_map_;

void TimeTable::GetRandTable(vector<vector<int> > &randtable) {
	randtable.clear();
	for (int i = 0; i < days_per_week_; i++) {
		vector<int> ps;
		for (int j = 0; j < period_per_day_; j++) {
			ps.push_back(j);
		}
		for (int j = 0; j < ps.size(); j++) {
			int r = rand() % ps.size();
			if (j != r)swap(ps[j], ps[r]);
		}
		randtable.push_back(ps);
	}
}

//让每一节课都按照科目放到班级课表当中的科目系统当中，方便添加限制条件
void TimeTable::Init(TimeTable &time_table, vector<Teacher> &teachers) {
	//courses_map_ = courses_map;
	class_que_ = vector<ClassUnit>(time_table.class_que_.size());
	class_que_ = time_table.class_que_;
	course_classes_ = vector<vector<ClassUnit *> >(courses_map_.size());
	table_ = vector<vector<ClassUnit *> >(days_per_week_, vector<ClassUnit *>(period_per_day_, NULL));

	vector<ClassUnit> ::iterator itc = class_que_.begin();
	for (; itc != class_que_.end(); itc++) {
		course_classes_[itc->course_id_].push_back(&(*itc));
	}
	
	//分配已经预先安排好的课程的时间
	for (int i = 0; i < class_que_.size(); i++) {
		ClassUnit &cu = class_que_[i];
		int d = cu.class_time_.first, p = cu.class_time_.second;
		if (d != -1) {
			//SetUnitInfo(cu, d, p, teachers);
			table_[d][p] = &cu;
			cu.AddUnit(d, p, teachers);
		}
	}
	
	//生成随机时间表
	vector<vector<int> > randtable;
	GetRandTable(randtable);
	//cout << "timetable.init\n";
	//system("PAUSE");
	//安排连堂课,注意越界
	for (int i = 0; i < class_que_.size(); i++) {
		if (class_que_[i].continue_tag_ && (class_que_[i].class_time_.first == -1)) {
			int d = rand() % days_per_week_;
			int x = -1, y = -1;
			while (x == -1){
				while (!randtable[d].empty() && (randtable[d].back() != (period_per_day_ - 1)) && 
					(randtable[d].back() != (period_in_moring_ - 1))&& table_[d][randtable[d].back()] != NULL){
					randtable[d].pop_back();
				}
				if (randtable[d].empty()) {
					d = (d + 1) % days_per_week_;
				}
				else {
					x = d;
					y = randtable[d].back();
					randtable[d].pop_back();
				}
			}
			for (int j = 0; j < 2; j++) {
				//ClassUnit &cu = class_que_[i + j];
				//SetUnitInfo(cu, x, y + j, teachers);
				table_[x][y + j] = &class_que_[i + j];
				class_que_[i + j].AddUnit(x, y + j, teachers);
			}
			i++;
		}
	}
	//cout << "timetable.init";
	//system("PAUSE");
	GetRandTable(randtable);
	//安排普通课程
	for (int i = 0; i < class_que_.size(); i++) {
		if ((class_que_[i].continue_tag_ == 0) && (class_que_[i].class_time_.first == -1)) {
			int d = rand() % days_per_week_;
			int x = -1, y = -1;
			while (x == -1){
				while ((!randtable[d].empty()) && (table_[d][randtable[d].back()] != NULL)){
					randtable[d].pop_back();
				}
				if (randtable[d].empty()) {
					d = (d + 1) % days_per_week_;
				}
				else {
					x = d;
					y = randtable[d].back();
					randtable[d].pop_back();
				}
			}
			//ClassUnit &cu = class_que_[i];
			table_[x][y] = &class_que_[i];
			//SetUnitInfo(cu, x, y, teachers);
			class_que_[i].AddUnit(x, y, teachers);
		}
	}
	//cout << "timetable.init\n";
	//system("PAUSE");
}

void TimeTable::AddContinue(int course_id, int continue_num) {
	int k = 0;
	for (int i = 0; i < continue_num; i++) {
		course_classes_[course_id][k++]->continue_tag_ = 1;
		course_classes_[course_id][k++]->continue_tag_ = 2;
	}
}

void TimeTable::AddCant(int course_id, vector<pair<int, int> > &cant_time) {
	for (int i = 0; i < course_classes_[course_id].size(); i++) {
		course_classes_[course_id][i]->AddCant(cant_time);
	}
}

void TimeTable::AddItime(int course_id, vector<pair<int, int> > &itime) {
	for (int i = 0; i < itime.size(); i++) {
		course_classes_[course_id][i]->alterable_ = 0;
		course_classes_[course_id][i]->class_time_.first = itime[i].first;
		course_classes_[course_id][i]->class_time_.second = itime[i].second;
	}
}

int TimeTable::GetNumOfContinueClasses() {
	int num = 0;
	for (int i = 0; i < class_que_.size(); i++) {
		if (class_que_[i].continue_tag_ == 2)
			num++;
	}
	return num;
}

void TimeTable::Update(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	//只有实体课程单元才需要进行对应任课老师的修改,修改老师上课时间的信息
	if (table_[x][y] != NULL) {
		//将原有教师信息删除
		table_[x][y]->DelUnit(teachers);
		//将老师和单元课时间信息更新
		table_[x][y]->AddUnit(nx, ny, teachers);
	}
}

void TimeTable::UnitSwap(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	//先修改实体的课程单元当中的教师信息，再将两个指针进行交换
	Update(x, y, nx, ny, teachers);
	Update(nx, ny, x, y, teachers);
	swap(table_[x][y], table_[nx][ny]);
}

bool TimeTable::CanMutate(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	if (x == nx && y == ny)return 0;
	if (table_[x][y]->continue_tag_ == 0) {
		//1换1
		//判是否是空
		if (table_[nx][ny] == NULL)return 1;
		else {
			if (table_[nx][ny]->alterable_ == 0)return 0;
			if (table_[x][y]->teacher_.id_ == table_[nx][ny]->teacher_.id_) return 0;
			return 1;
		}
	}
	else {
		//2换2
		if ((ny == period_in_moring_ - 1) || (ny == period_per_day_ - 1))return 0;
		if (table_[nx][ny] == NULL || (table_[nx][ny] != NULL &&
			table_[nx][ny]->alterable_ && table_[nx][ny]->continue_tag_ != 2)) {
			if (table_[nx][ny + 1] == NULL)return 1;
			if (table_[nx][ny + 1]->continue_tag_ == 1 || table_[nx][ny + 1]->alterable_ == 0)return 0;
			return 1;
		}
		else return 0;
	}

}

void TimeTable::Mutate(double mp, vector<Teacher> &teachers) {
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			//保证x,y不为空,但是nx,ny不保证
			if (table_[x][y] == NULL)continue;
			if (table_[x][y]->alterable_ == 0)continue;
			double rd = (double)rand() * rand() / kRandPlusRand;
			if (rd > mp)continue;
			int nx = rand() % days_per_week_, ny = rand() % period_per_day_, t = 0;
			while (!CanMutate(x, y, nx, ny, teachers) && t < 15) {
				nx = rand() % days_per_week_, ny = rand() % period_per_day_;
				t++;
			}
			if (t == 15)continue;
			cout << "timetable.mutate\n";
			if(table_[x][y]->continue_tag_ == 0) UnitSwap(x, y, nx, ny, teachers);
			else {
				//连堂课进行判断
				int k[3];
				k[0] = (table_[x][y]->continue_tag_ == 1 ? 0 : -1);
				k[1] = (table_[x][y]->continue_tag_ == 1 ? 1 : 0);
				k[2] = (table_[x][y]->continue_tag_ == 1 ? 1 : -1);
				if (ny + k[1] == period_in_moring_ || 
					ny + k[1] == period_per_day_ || ny + k[0] == -1)continue;
				UnitSwap(x, y, nx, ny, teachers);
				UnitSwap(x, y + k[3], nx, ny + k[3], teachers);
			}
		}
	}
}

//因为两张timetable当中的classunit是各自分别占了不同两块内存的各自独立的对象，所以不能再用指针来分别两个节次单元
//需要直接用各自节次的id来搞
//连堂课不进行cross操作
void TimeTable::Cross(TimeTable &another, double cp, vector<Teacher> &teachers) {
	//newtable存的都是当前自己classque的地址
	vector<vector<ClassUnit *> > newtable = vector<vector<ClassUnit *> >(days_per_week_, vector<ClassUnit *>(period_per_day_, NULL));
	map<int, bool> needcross;
	map<int, bool> :: iterator it;

	//cp = 0.5;
	//选出来要进行交换的节次
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			if (table_[x][y] == NULL || table_[x][y]->continue_tag_)continue;
			int uid = table_[x][y]->unit_id_;
			double r = (double)rand() * rand() / kRandPlusRand;
			//如果符合概率就要被cross，否则就是直接被保留下来
			if (r < cp && table_[x][y]->alterable_ == 1) {
				needcross[uid] = 1;
			}
			else newtable[x][y] = table_[x][y];
		}
	}

	//cout << "timetable.cross\n";
	//将能进行直接交换的节次就直接交换，并记录还没有利用起来的时间
	//这步当中是不存在table_[x][y]为空的情况的
	vector<pair<int, int> > periods;
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			ClassUnit *cu = another.table_[x][y];
			if (cu == NULL || cu->continue_tag_)continue;
			int uid = another.table_[x][y]->unit_id_;
			if (needcross.find(uid) != needcross.end() && newtable[x][y] == NULL) {
				newtable[x][y] = &class_que_[uid];
				it = needcross.find(uid);
				needcross.erase(it);
				Update(class_que_[uid].class_time_.first, class_que_[uid].class_time_.second, x, y, teachers);
			}
			if (newtable[x][y] == NULL) {
				periods.push_back(make_pair(x, y));
			}
		}
	}
	
	for (int i = 0; i < periods.size(); i++) {
		int j = rand() % periods.size();
		if (i == j)continue;
		swap(periods[i], periods[j]);
	}

	it = needcross.begin();
	for (int i = 0; it != needcross.end(); it++, i++) {
		ClassUnit &cu = class_que_[it->first];
		newtable[periods[i].first][periods[i].second] = &cu;
		Update(cu.class_time_.first, cu.class_time_.second, periods[i].first, periods[i].second, teachers);
	}
	table_ = newtable;
	//cout << "timetable.cross\n";
}

//主要用到老师的availabletime和roomtime两个map
void TimeTable::Modify(vector<Teacher> &teachers) {
	map<pair<int, int>, bool>::iterator it;
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			if (table_[x][y] != NULL) {
				ClassUnit *cu = table_[x][y];
				//该老师上课时间冲突需要调整
				if (cu->IsConflict(teachers)) {
					SolveConflict(cu ,teachers);
				}
			}
		}
	}
}

void TimeTable::SolveConflict(ClassUnit *cu, vector<Teacher> &teachers) {
	vector<pair<int, int> > availtime = vector<pair<int, int> >(0);
	//判断是否是连堂课，如果是连堂课那么就要转移到连堂课的第一堂课，然后选出两个连续的空余时间进行交换
	//如果不是连堂，那么就选出一节空余的时间进行交换即可
	int tid = cu->teacher_.id_, cid = cu->class_id_;
	//如果是连堂课那么都转移到连堂课的第一节当中
	int x = cu->class_time_.first, y = (cu->continue_tag_ == 2) ? cu->class_time_.second - 1 : cu->class_time_.second;
	//该次课上了多久
	int ctime = (cu->continue_tag_ == 0 ? 1 : 2);
	pair<int, int> rt = make_pair(x, cid), rnt, pnt;
	map<pair<int, int>, bool>::iterator it = teachers[tid].available_time.begin();
	for (; it != teachers[tid].available_time.end(); it++) {
		//如果是连堂，那么就找有连续两个空余时间的课的那种
		if (cu->continue_tag_) {
			pnt = make_pair(it->first.first, it->first.second + 1);
			//判断这个课是存在两个连续的课的
			if (teachers[tid].available_time.find(pnt) == teachers[tid].available_time.end())continue;
		}
		//1.该老师没有在那一天上过课，2.这个空余时间和当前这个课的上课日期一样，而且只上了这个课
		rnt = make_pair(it->first.first, cid);
		if (rnt == rt) {
			if(ctime == teachers[tid].room_time_[rt])
				availtime.push_back(it->first);
			else continue;
		}
		else {
			if (teachers[tid].room_time_.find(rnt) == teachers[tid].room_time_.end())
				availtime.push_back(it->first);
			else continue;
		}
	}
	for (int i = 0; i < availtime.size(); i++) {
		int j = rand() % availtime.size();
		if (i == j)continue;
		swap(availtime[i], availtime[j]);
	}
	//ClassUnit *ct;
	int nx, ny;
	//cout << "timetable.modify\n";
	for (int i = 0; i < availtime.size(); i++) {
		x = cu->class_time_.first, nx = availtime[i].first;
		y = cu->class_time_.second, ny = availtime[i].second;
		//连堂课我们都是从连堂课的第一节开始进行判断,之前已经处理过了
		if (CheckUnit(x, y, nx, ny, teachers)) {
			UnitSwap(x, y, nx, ny, teachers);
			if (cu->continue_tag_) {				
				UnitSwap(x, y + 1, nx, ny + 1, teachers);
			}
			break;
		}
	}
}

//检查是否能够进行对换操作
bool TimeTable::CheckUnit(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	//1.先判断是否是连堂课	
	//虽然在之前第一个调用处做了限制，但是这里仍然会涉及到自己调用自己的情况,所以continuetag仍然会是存在1和2
	if (table_[x][y]->continue_tag_) {
		if (table_[x][y]->continue_tag_ == 1) {
			if (y == period_in_moring_ - 1 || y == period_per_day_ - 1)return 0;
			if (table_[nx][ny] != NULL) {
				if (table_[nx][ny]->continue_tag_ == 2)return 0;
				if (table_[nx][ny]->alterable_ == 0)return 0;
				return (CheckTeacherTime(nx, ny, x, y, teachers) && CheckUnit(x, y + 1, nx, ny + 1, teachers));
			}
			else return CheckUnit(nx, ny + 1, x, y + 1, teachers);
		}
		else {
			//continuetag是2的情况下	
			if (table_[nx][ny] != NULL) {
				if (table_[nx][ny]->continue_tag_ == 1)return 0;
				if (table_[nx][ny]->alterable_ == 0)return 0;
				return CheckTeacherTime(nx, ny, x, y, teachers);
			}
			else return 1;
		}
	}
	else {
		//1换1的情况
		if (table_[nx][ny] != NULL) {
			if (table_[nx][ny]->alterable_ == 0)return 0;
			if (table_[nx][ny]->continue_tag_ != 0)return 0;
			return CheckTeacherTime(nx, ny, x, y, teachers);
		}
		else return 1;
		//因为1换1的情况下这个nx和ny肯定是该老师空余时间，所以一定能换成功
	}
}

//检查该老师能否换到nx和ny的时间空格当中
bool TimeTable::CheckTeacherTime(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	int cid = table_[x][y]->class_id_;
	int tid = table_[x][y]->teacher_.id_;
	pair<int, int> pt = make_pair(nx, ny);
	pair<int, int> rt = make_pair(nx, cid);
	if (teachers[tid].available_time.find(pt) == teachers[tid].available_time.end())return 0;
	else if (teachers[tid].room_time_.find(rt) != teachers[tid].room_time_.end())return 0;
	else return 1;
}