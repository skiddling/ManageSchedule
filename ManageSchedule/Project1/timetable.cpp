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

void TimeTable::DelUnit(ClassUnit &cu, vector<Teacher *> teachers) {
	int x = cu.class_time_.first, y = cu.class_time_.second;
	pair<int, int> pt = make_pair(x, y);
	if (teachers[cu.teacher_.id_]->class_table_[pt] > 1) {
		teachers[cu.teacher_.id_]->class_table_[pt]--;
	}
	else {
		map<pair<int, int>, int> ::iterator it = teachers[cu.teacher_.id_]->class_table_.find(pt);
		teachers[cu.teacher_.id_]->class_table_.erase(it);
	}
}

void TimeTable::AddUnit(ClassUnit &cu, int x, int y, vector<Teacher *> teachers) {
	cu.class_time_.first = x;
	cu.class_time_.second = y;
	pair<int, int> pt = make_pair(x, y);
	if (teachers[cu.teacher_.id_]->class_table_.find(pt) == teachers[cu.teacher_.id_]->class_table_.end()) {
		teachers[cu.teacher_.id_]->class_table_[pt] = 1;
	}
	else teachers[cu.teacher_.id_]->class_table_[pt]++;
}

void TimeTable::SetUnitInfo(ClassUnit &cu, int x, int y, vector<Teacher *> &teachers) {
	table_[x][y] = &cu;
	AddUnit(cu, x, y, teachers);

}

//��ÿһ�ڿζ����տ�Ŀ�ŵ��༶�α��еĿ�Ŀϵͳ���У����������������
void TimeTable::Init(map<string, int> &courses_map, TimeTable &time_table, vector<Teacher *> &teachers) {
	courses_map_ = courses_map;
	class_que_ = time_table.class_que_;
	course_classes = vector<vector<ClassUnit *> >(courses_map_.size());
	table_ = vector<vector<ClassUnit *> >(days_per_week_, vector<ClassUnit *>(period_per_day_, NULL));

	vector<ClassUnit> ::iterator itc = class_que_.begin();
	for (; itc != class_que_.end(); itc++) {
		course_classes[itc->course_id_].push_back(&(*itc));
	}
	
	//�����Ѿ�Ԥ�Ȱ��źõĿγ̵�ʱ��
	for (int i = 0; i < class_que_.size(); i++) {
		ClassUnit &cu = class_que_[i];
		int d = cu.class_time_.first, p = cu.class_time_.second;
		if (d != -1) {
			SetUnitInfo(cu, d, p, teachers);
		}
	}
	
	//�������ʱ���
	vector<vector<int> > randtable;
	GetRandTable(randtable);

	//�������ÿ�
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
				ClassUnit &cu = class_que_[i + j];
				SetUnitInfo(cu, x, y + j, teachers);
			}
			i++;
		}
	}

	GetRandTable(randtable);
	//������ͨ�γ�
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
			ClassUnit &cu = class_que_[i];
			SetUnitInfo(cu, x, y, teachers);
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
	if (table_[nx][ny] != NULL) {
		ClassUnit &ncu = *table_[nx][ny];
		DelUnit(ncu, teachers);
		AddUnit(ncu, x, y, teachers);
	}
	ClassUnit &cu = *table_[x][y];
	DelUnit(cu, teachers);
	AddUnit(cu, nx, ny, teachers);
}

void TimeTable::Mutate(double mp, vector<Teacher *> teachers) {
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			if (table_[x][y] == NULL)continue;
			if (table_[x][y]->alterable_ == 0)continue;
			double rd = (double)rand() * rand() / kRandPlusRand;
			if (rd > mp)continue;
			int nx = rand() % days_per_week_, ny = rand() % period_per_day_;
			while ((nx == x && ny == y) || (table_[nx][ny]->alterable_ == 0) || 
				(table_[nx][ny] != NULL && table_[nx][ny]->teacher_.id_ == table_[x][y]->teacher_.id_)) {
				nx = rand() % days_per_week_, ny = rand() % period_per_day_;
			}
			Update(x, y, nx, ny, teachers);
		}
	}
}

void TimeTable::Cross(TimeTable &timetable) {

}

void TimeTable::CalCrash() {

}
