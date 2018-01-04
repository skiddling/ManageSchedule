#pragma once
#include "classunit.h"

//�༶�α�,��ʵ����һ���εļ���
//����༶���а���������Ҫ������༶�����ϿεĽڴ�
class TimeTable 
{
public:
	
	//data from database
	long long class_id_;//���ݿ⵱�е�һ���ֶ�
	long long dbid_;//���ݿ�������ţ����������ĸ���

	vector<Course> couque_;//����༶�ϵľ������Щ�γ�
	map<Course, int> coutab_;//������Щ�γ�������༶����ÿ����ϼ��ο�
	vector<Teacher> teaque_;//����������൱���Ͽε���Щ��ʦ

	int days_, periods_;//һ��������ÿ��ڴ���
	int pd = 0, pp = 0;//���ڿα��ʼ����ʱ���õ���Ϊ�α�ڷ�λ�õ�����
	int roomid_;//����༶��id��Ҳ����vector<timetalbe>���е����,��Ҫ�Լ�������Ҫȥ����
	string roomname_;//������ҵ�����
	vector<vector<ClassUnit*>> roomtable_;// =
		//vector<vector<ClassUnit*>>(days_, vector<ClassUnit*>(periods_, nullptr));//���ҿα�ָ��ֻ�뵱ǰ�Լ�clsque���е�����
		//vector<vector<ClassUnit*>>(periods_, vector<ClassUnit*>(days_, nullptr));//���ҿα�ָ��ֻ�뵱ǰ�Լ�clsque���е�����

	//�ӽڴ�ָ��ĳɽڴ���ţ����㿽����ֵ�Ȳ���
	vector<int> clsqueindex_;
	vector<ClassUnit*> clsque_;
	
	//�°汾������Ҫ�����жϿɵ�ʱ���һ�����ݣ�long long��ĳ���ε�id��int��һ���Ͽδ���
	map<long long, vector<int>> course_time_;
	//vector<ClassUnit>& unitset_;//�����Ҫ��schedule���е�units����һ�µ�
	//vector<ClassUnit> cls_que_;//�����ָ������
	//vector<vector<bool>> tabletag_;//������ʱ�ڶ��Ƿ��ܱ����ŷſΣ���ʼ���α���
	void CalFitness(int& crash);
	TimeTable();
	TimeTable(string roomname, int days, int periods, long long class_id, long long dbid);
	//TimeTable(const TimeTable& t);
	//TimeTable& operator=(const TimeTable& t);
	void Init();
	bool PutIntoTable(ClassUnit* cptr);
	void UpdatePtrs();
	ClassUnit** GetClsUnitPtr(int day, int period);

	void SwapUnits(ClassUnit* origin, ClassUnit* target);

	static string GetName() {
		return "TimeTable";
	}

private:

};