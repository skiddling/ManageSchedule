#include "timetable.h"

int TimeTable::days_per_week_ = 0;
int TimeTable::period_in_moring_ = 0;
int TimeTable::period_per_day_ = 0;

TimeTable::TimeTable() {
	class_que_ = vector<ClassUnit>(0);
	//table_ = vector<vector<ClassUnit> >(days_per_week_, vector<ClassUnit>(period_per_day_));
}

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
void TimeTable::Init(map<string, int> &courses_map, TimeTable &time_table, vector<Teacher *> &teachers) {
	courses_map_ = courses_map;
	class_que_ = time_table.class_que_;
	course_classes = vector<vector<ClassUnit *> >(courses_map_.size());
	table_ = vector<vector<ClassUnit *> >(days_per_week_, vector<ClassUnit *>(period_per_day_, NULL));

	vector<ClassUnit> ::iterator itc = class_que_.begin();
	for (; itc != class_que_.end(); itc++) {
		course_classes[itc->course_id_].push_back(&(*itc));
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
}

void TimeTable::AddContinue(int course_id, int continue_num) {
	int k = 0;
	for (int i = 0; i < continue_num; i++) {
		course_classes[course_id][k++]->continue_tag_ = 1;
		course_classes[course_id][k++]->continue_tag_ = 2;
	}
}

void TimeTable::AddCant(int course_id, vector<pair<int, int> > &cant_time) {
	for (int i = 0; i < course_classes[course_id].size(); i++) {
		course_classes[course_id][i]->AddCant(cant_time);
	}
}

void TimeTable::AddItime(int course_id, vector<pair<int, int> > &itime) {
	for (int i = 0; i < itime.size(); i++) {
		course_classes[course_id][i]->alterable_ = 0;
		course_classes[course_id][i]->class_time_.first = itime[i].first;
		course_classes[course_id][i]->class_time_.second = itime[i].second;
	}
}

void TimeTable::Update(int x, int y, int nx, int ny, vector<Teacher *> teachers) {
	if (table_[x][y] != NULL) {
		table_[x][y]->DelUnit(teachers);
		table_[x][y]->AddUnit(nx, ny, teachers);
	}
}

void TimeTable::UnitSwap(int x, int y, int nx, int ny, vector<Teacher *> &teachers) {
	Update(x, y, nx, ny, teachers);
	Update(nx, ny, x, y, teachers);
	swap(table_[x][y], table_[nx][ny]);
}

bool TimeTable::CanMutate(int x, int y, int nx, int ny, vector<Teacher *> &teachers) {
	if (x == nx && y == ny)return 0;
	if (table_[x][y]->continue_tag_ == 0) {
		//1换1
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

void TimeTable::Mutate(double mp, vector<Teacher *> teachers) {
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
			UnitSwap(x, y, nx, ny, teachers);
			if (table_[nx][ny]->continue_tag_) {
				UnitSwap(x, y + 1, nx, ny + 1, teachers);
			}
		}
	}
}

//因为两张timetable当中的classunit是各自分别占了不同两块内存的各自独立的对象，所以不能再用指针来分别两个节次单元
//需要直接用各自节次的id来搞
//连堂课不进行cross操作
void TimeTable::Cross(TimeTable &timetable, double cp, vector<Teacher *> &teachers) {
	vector<vector<ClassUnit *> > newtable = vector<vector<ClassUnit *> >(days_per_week_, vector<ClassUnit *>(period_per_day_, NULL));
	map<int, bool> needcross;
	map<int, bool> :: iterator it;

	//选出来要进行交换的节次
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			if (table_[x][y] == NULL || table_[x][y]->continue_tag_)continue;
			int uid = table_[x][y]->unit_id_;
			double r = (double)rand() * rand() / kRandPlusRand;
			//如果符合概率就要被cross，否则就是直接被保留下来
			if (r < cp && table_[x][y]->alterable_ == 0) {
				needcross[uid] = 1;
			}
			else newtable[x][y] = table_[x][y];
		}
	}

	//将能进行直接交换的节次就直接交换，并记录还没有利用起来的时间
	vector<pair<int, int> > periods;
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			ClassUnit *cu = timetable.table_[x][y];
			if (cu->continue_tag_)continue;
			int uid = timetable.table_[x][y]->unit_id_;
			if (cu != NULL && needcross.find(uid) != needcross.end() && newtable[x][y] == NULL) {
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
}

//主要用到老师的availabletime和roomtime两个map
void TimeTable::Modify(vector<Teacher *> teachers) {
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

void TimeTable::SolveConflict(ClassUnit *cu, vector<Teacher *> teachers) {
	vector<pair<int, int> > availtime = vector<pair<int, int> >(0);
	int tid = cu->teacher_.id_;
	//这个有空时间只是说这些时间这个老师是不上课的，但是在具体修正的时候我们要考虑一个班一个老师在一天内只能去一次
	//所以我们在添加这个空余时间的时候我们需要对这些时间进行判断
	map<pair<int, int>, bool>::iterator  it = teachers[tid]->available_time.begin();
	int cid = cu->class_id_;
	pair<int, int> tu, tn = make_pair(cu->class_time_.first, cu->class_id_);
	for (; it != teachers[tid]->available_time.end(); it++) {
		tu = make_pair(it->first.first, cid);
		if ((it->first.first == cu->class_time_.first && teachers[tid]->room_time_[tn] == 1)
			|| teachers[tid]->room_time_.find(tu) == teachers[tid]->room_time_.end()) {
			//情况1：该班级该天该老师只是在这个班上了一次课，那么这个空余时间可以是该天的其他空余的时间段
			//情况2：其他老师没有在该班级上过课的那几天当中的空余的时间段
			availtime.push_back(it->first);
		}
	}
	for (int i = 0; i < availtime.size(); i++) {
		int j = rand() % availtime.size();
		if (i == j)continue;
		swap(availtime[i], availtime[j]);
	}
	//ClassUnit *ct;
	int x, y, nx, ny, tag;
	for (int i = 0; i < availtime.size(); i++) {
		x = cu->class_time_.first, nx = availtime[i].first;
		y = cu->class_time_.second, ny = availtime[i].second;
		if (cu->continue_tag_ == 2) {
			y--, ny--;
		}
		if (CheckUnit(x, y, nx, ny, teachers)) {
			UnitSwap(x, y, nx, ny, teachers);
			if (table_[x][y]->continue_tag_) {				
				tag = (table_[x][y]->continue_tag_ == 1 ? 1 : -1);
				UnitSwap(x, y + tag, nx, ny + tag, teachers);
			}
			break;
		}
	}
}

bool TimeTable::CheckUnit(int x, int y, int nx, int ny, vector<Teacher *> &teachers) {
	int cid = table_[x][y]->class_id_, tid = table_[x][y]->teacher_.id_, tnid = table_[nx][ny]->teacher_.id_;
	pair<int, int> pt = make_pair(x, y), pnt, rt = make_pair(x, cid), rnt = make_pair(nx, cid);
	if (teachers[tid]->room_time_.find(rnt) != teachers[tid]->room_time_.end())return 0;
	//对方老师没空,tid为对方老师的id
	if (table_[nx][ny] != NULL) {
		//int tid = table_[nx][ny]->teacher_.id_;
		if(tid != tnid){
		//if (tid != table_[x][y]->teacher_.id_) {
			//对方老师不存在该时间段有空,以及该老师已经在该天上过这个班的课了，不能再上了
			if (teachers[tnid]->available_time.find(pt) == teachers[tnid]->available_time.end()) return 0;
			if (teachers[tnid]->room_time_.find(rt) != teachers[tnid]->room_time_.end())return 0;
		}
		else {
			//两个老师相同
			pnt = make_pair(nx, ny);
			if (teachers[tid]->class_table_[pt] == 1 && teachers[tid]->class_table_[pnt] == 1) {
				int tg = table_[x][y]->continue_tag_ > 0 ? 2 : 1;
				int tng = table_[nx][ny]->continue_tag_ > 0 ? 2 : 1;
				if (teachers[tid]->room_time_[rt] == tg && teachers[tid]->room_time_[rnt] == tng)return 1;
				return 0;
			}
			//if (teachers[tid]->class_table_[pt] >= 1 || teachers[tid]->class_table_[pnt] >= 1) return 0;
			//if (table_[x][y]->continue_tag_ == 0 && teachers[tid]->room_time_[rt] > 1)return 0;
			//if (table_[nx][ny]->continue_tag_ == 0 && teachers[tid]->room_time_[rnt] > 1)return 0;
			//if (table_[x][y]->continue_tag_ && teachers[tid]->room_time_[rt] > 2)return 0;
			//if (table_[nx][ny]->continue_tag_ && teachers[tid]->room_time_[rnt] > 2)return 0;

		}
	}
	//对方老师有空
	if (table_[x][y]->continue_tag_ == 0) {
		//1换1
		if (table_[nx][ny] == NULL)return 1;
		if (table_[nx][ny]->alterable_ == 0)return 0;
		if (table_[nx][ny]->continue_tag_ == 0)return 1;
		return 0;
	}
	else {
		//2换2
		if (table_[x][y]->continue_tag_ == 1) {
			if (ny == period_in_moring_ - 1 || ny == period_per_day_ - 1)return 0;
			if (table_[nx][ny] == NULL || (table_[nx][ny] != NULL && table_[nx][ny]->alterable_  &&
				table_[nx][ny]->continue_tag_ != 2)) {
				return CheckUnit( x, y + 1, nx, ny + 1, teachers);
			}
			return 0;
		}
		else {
			if (ny == period_in_moring_ || ny == 0)return 0;
			if (table_[nx][ny] == NULL || (table_[nx][ny] != NULL && table_[nx][ny]->alterable_ &&
				table_[nx][ny]->continue_tag_ != 2))
				return 1;
			else return 0;
		}
	}
}