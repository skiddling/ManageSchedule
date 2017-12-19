#pragma once
#include "teacher.h"

class TimeTable;

class ClassUnit
{
public:
	//data get from datebase
	int secionno_ = -1;//��ڿε����
	long long pkcombinateclassgroup_ = -1;//�ϰ����ţ�һ��ϰ�Ŀε���һ������һ����ͬ�����
	long long pkevensection_ = -1;//���õ���ţ������ڴκ���һ���
	long long dbid_ = -1;//���ݿ⵱�еĽ�ѧ���id,Ҳ����pkclasscocurse��ֵ

	ClassUnit(TimeTable* ttb = nullptr, Teacher* teacher = nullptr, Course* couptr = nullptr);
	ClassUnit();
	map<string, void*> maptrs_;//�����������õ�ָ���
	Course course_;//�����ʲô��
	//һ���Ǻ��ĵ�����ָ��
	Course* couptr_;
	Teacher* teacher_;//��ڿε��ο���ʦ��ָ��
	TimeTable* ttbptr_;//ָ���Լ����ڿα��ָ�룬�����Ҫ�ڸ�ֵ��ʱ���޸ĵ�

	pair<int, int> stime_ = make_pair(-1, -1);//������ڿε���ʼʱ��
	bool hasbeenput_ = false;//��ʾ��ڿ��Ƿ��Ѿ����ŵ��α��У����ڿα��ʼ��
	ClassUnit** headptr_ = nullptr;//������ʾ�Ƿ������ÿεĵ�һ�ڣ�ָ��������ttb����ָ���һ������ε�ָ��
	int type_;//��ڿε����ͣ�1��ʾ��ͨ��0��ʾ������2��ʾ����ͨ���͵����ÿΣ����Ǹ������Ͳ��ܺ���һ����ͬһ�����
	int duration_ = 1;//��ʱ���ȣ��տ�ʼ��������1��֮����޸ĳ����õ���ֵ

	bool preput_ = false;//��ʾ������Ƿ�Ԥ����
	pair<int, int> pretime_ = make_pair(-1, -1);

	//vector<int> unioncls_;//�ϰ�γ̣���ֵ����ָ��ͬһ��schedule���е��±�
	vector<int> union_cls_index_;//�ϰ�γ̣���ֵ����ָ��ͬһ��schedule���е��±�
	vector<ClassUnit*> unioncls_;//�ϰ�γ̣���ֵ����ָ��ͬһ��schedule���е�ָ��
	vector<int> unioclsid_;//�ϰ�γ�id
	pair<int, int> headunit_;

	//�������������ĵ���������
	set<pair<int, int>> canntbeput_;//����֮��ڿβ��ܰ���ʱ��
	//��canntbeput�ǲ�����������modify��ʱ�����ֻ��һ�����еĲ��뼯�ϣ�����һ���ǽ�ʦ��ʱ��
	//�°汾����õ���������ǲ��õı����󽻼��ķ���
	set<pair<int, int>> canbeput_;
	
	//����α��ʼ������
	bool operator < (const ClassUnit& c)const {
		if (duration_ + union_cls_index_.size() != c.duration_ + c.union_cls_index_.size())
			return duration_ + union_cls_index_.size() > c.duration_ + c.union_cls_index_.size();
		/*else if (unioclsid_.size() != c.unioclsid_.size())
			return unioclsid_.size() > c.unioclsid_.size();*/
		else if (canntbeput_.size() != c.canntbeput_.size())
			return canntbeput_.size() > c.canntbeput_.size();
		else if (dbid_ != c.dbid_)
			return dbid_ < c.dbid_;
		else if (secionno_ != c.secionno_)
			return secionno_ < c.secionno_;
	}
	
	string GetTeaName();
	string GetCouName();
	int GetDuration();
	bool GetType();
	bool PutIntoTable(int day, int period, bool flag);
	int CalFitness();
	int GetTeacherIdInVec();
	int GetTimeTableIdInVec();
	vector<pair<int, int>> GetRandAvailTime();
	bool CheckPeriod(pair<int, int> per);
	void ChangeTime(pair<int, int> period);
	//��ʼ����ʱ�������ʱ����Ƿ��п�
	bool CheckTimeEmpty(int d, int p);
	void UpdateRoomPtr();
	bool CheckTimeIllegal(pair<int, int> tim);//�������ε�ʱ���Ƿ����
	ClassUnit* GetTargetUnit(pair<int, int> tim);//���ͬһ�ű���Ŀ��ʱ��Ľڴ�
	ClassUnit** GetTimeTablePtr(pair<int, int> tim);
	
private:
	default_random_engine e_;
	string teaname_, couname_;
	int cuid_;//�ýڿε�id
	

	void GetRandSet(vector<pair<int, int>>& randset);
};
