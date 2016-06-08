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

//��ÿһ�ڿζ����տ�Ŀ�ŵ��༶�α��еĿ�Ŀϵͳ���У����������������
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
	
	//�����Ѿ�Ԥ�Ȱ��źõĿγ̵�ʱ��
	for (int i = 0; i < class_que_.size(); i++) {
		ClassUnit &cu = class_que_[i];
		int d = cu.class_time_.first, p = cu.class_time_.second;
		if (d != -1) {
			//SetUnitInfo(cu, d, p, teachers);
			table_[d][p] = &cu;
			cu.AddUnit(d, p, teachers);
		}
	}
	
	//�������ʱ���
	vector<vector<int> > randtable;
	GetRandTable(randtable);
	//cout << "timetable.init\n";
	//system("PAUSE");
	//�������ÿ�,ע��Խ��
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
	//ֻ��ʵ��γ̵�Ԫ����Ҫ���ж�Ӧ�ο���ʦ���޸�,�޸���ʦ�Ͽ�ʱ�����Ϣ
	if (table_[x][y] != NULL) {
		//��ԭ�н�ʦ��Ϣɾ��
		table_[x][y]->DelUnit(teachers);
		//����ʦ�͵�Ԫ��ʱ����Ϣ����
		table_[x][y]->AddUnit(nx, ny, teachers);
	}
}

void TimeTable::UnitSwap(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	//���޸�ʵ��Ŀγ̵�Ԫ���еĽ�ʦ��Ϣ���ٽ�����ָ����н���
	Update(x, y, nx, ny, teachers);
	Update(nx, ny, x, y, teachers);
	swap(table_[x][y], table_[nx][ny]);
}

bool TimeTable::CanMutate(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	if (x == nx && y == ny)return 0;
	if (table_[x][y]->continue_tag_ == 0) {
		//1��1
		//���Ƿ��ǿ�
		if (table_[nx][ny] == NULL)return 1;
		else {
			if (table_[nx][ny]->alterable_ == 0)return 0;
			if (table_[x][y]->teacher_.id_ == table_[nx][ny]->teacher_.id_) return 0;
			return 1;
		}
	}
	else {
		//2��2
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
			//��֤x,y��Ϊ��,����nx,ny����֤
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
				//���ÿν����ж�
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

//��Ϊ����timetable���е�classunit�Ǹ��Էֱ�ռ�˲�ͬ�����ڴ�ĸ��Զ����Ķ������Բ�������ָ�����ֱ������ڴε�Ԫ
//��Ҫֱ���ø��Խڴε�id����
//���ÿβ�����cross����
void TimeTable::Cross(TimeTable &another, double cp, vector<Teacher> &teachers) {
	//newtable��Ķ��ǵ�ǰ�Լ�classque�ĵ�ַ
	vector<vector<ClassUnit *> > newtable = vector<vector<ClassUnit *> >(days_per_week_, vector<ClassUnit *>(period_per_day_, NULL));
	map<int, bool> needcross;
	map<int, bool> :: iterator it;

	//cp = 0.5;
	//ѡ����Ҫ���н����Ľڴ�
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			if (table_[x][y] == NULL || table_[x][y]->continue_tag_)continue;
			int uid = table_[x][y]->unit_id_;
			double r = (double)rand() * rand() / kRandPlusRand;
			//������ϸ��ʾ�Ҫ��cross���������ֱ�ӱ���������
			if (r < cp && table_[x][y]->alterable_ == 1) {
				needcross[uid] = 1;
			}
			else newtable[x][y] = table_[x][y];
		}
	}

	//cout << "timetable.cross\n";
	//���ܽ���ֱ�ӽ����Ľڴξ�ֱ�ӽ���������¼��û������������ʱ��
	//�ⲽ�����ǲ�����table_[x][y]Ϊ�յ������
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

//��Ҫ�õ���ʦ��availabletime��roomtime����map
void TimeTable::Modify(vector<Teacher> &teachers) {
	map<pair<int, int>, bool>::iterator it;
	for (int x = 0; x < days_per_week_; x++) {
		for (int y = 0; y < period_per_day_; y++) {
			if (table_[x][y] != NULL) {
				ClassUnit *cu = table_[x][y];
				//����ʦ�Ͽ�ʱ���ͻ��Ҫ����
				if (cu->IsConflict(teachers)) {
					SolveConflict(cu ,teachers);
				}
			}
		}
	}
}

void TimeTable::SolveConflict(ClassUnit *cu, vector<Teacher> &teachers) {
	vector<pair<int, int> > availtime = vector<pair<int, int> >(0);
	//�ж��Ƿ������ÿΣ���������ÿ���ô��Ҫת�Ƶ����ÿεĵ�һ�ÿΣ�Ȼ��ѡ�����������Ŀ���ʱ����н���
	//����������ã���ô��ѡ��һ�ڿ����ʱ����н�������
	int tid = cu->teacher_.id_, cid = cu->class_id_;
	//��������ÿ���ô��ת�Ƶ����ÿεĵ�һ�ڵ���
	int x = cu->class_time_.first, y = (cu->continue_tag_ == 2) ? cu->class_time_.second - 1 : cu->class_time_.second;
	//�ôο����˶��
	int ctime = (cu->continue_tag_ == 0 ? 1 : 2);
	pair<int, int> rt = make_pair(x, cid), rnt, pnt;
	map<pair<int, int>, bool>::iterator it = teachers[tid].available_time.begin();
	for (; it != teachers[tid].available_time.end(); it++) {
		//��������ã���ô������������������ʱ��Ŀε�����
		if (cu->continue_tag_) {
			pnt = make_pair(it->first.first, it->first.second + 1);
			//�ж�������Ǵ������������Ŀε�
			if (teachers[tid].available_time.find(pnt) == teachers[tid].available_time.end())continue;
		}
		//1.����ʦû������һ���Ϲ��Σ�2.�������ʱ��͵�ǰ����ε��Ͽ�����һ��������ֻ���������
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
		//���ÿ����Ƕ��Ǵ����ÿεĵ�һ�ڿ�ʼ�����ж�,֮ǰ�Ѿ��������
		if (CheckUnit(x, y, nx, ny, teachers)) {
			UnitSwap(x, y, nx, ny, teachers);
			if (cu->continue_tag_) {				
				UnitSwap(x, y + 1, nx, ny + 1, teachers);
			}
			break;
		}
	}
}

//����Ƿ��ܹ����жԻ�����
bool TimeTable::CheckUnit(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	//1.���ж��Ƿ������ÿ�	
	//��Ȼ��֮ǰ��һ�����ô��������ƣ�����������Ȼ���漰���Լ������Լ������,����continuetag��Ȼ���Ǵ���1��2
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
			//continuetag��2�������	
			if (table_[nx][ny] != NULL) {
				if (table_[nx][ny]->continue_tag_ == 1)return 0;
				if (table_[nx][ny]->alterable_ == 0)return 0;
				return CheckTeacherTime(nx, ny, x, y, teachers);
			}
			else return 1;
		}
	}
	else {
		//1��1�����
		if (table_[nx][ny] != NULL) {
			if (table_[nx][ny]->alterable_ == 0)return 0;
			if (table_[nx][ny]->continue_tag_ != 0)return 0;
			return CheckTeacherTime(nx, ny, x, y, teachers);
		}
		else return 1;
		//��Ϊ1��1����������nx��ny�϶��Ǹ���ʦ����ʱ�䣬����һ���ܻ��ɹ�
	}
}

//������ʦ�ܷ񻻵�nx��ny��ʱ��ո���
bool TimeTable::CheckTeacherTime(int x, int y, int nx, int ny, vector<Teacher> &teachers) {
	int cid = table_[x][y]->class_id_;
	int tid = table_[x][y]->teacher_.id_;
	pair<int, int> pt = make_pair(nx, ny);
	pair<int, int> rt = make_pair(nx, cid);
	if (teachers[tid].available_time.find(pt) == teachers[tid].available_time.end())return 0;
	else if (teachers[tid].room_time_.find(rt) != teachers[tid].room_time_.end())return 0;
	else return 1;
}