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
	int roomid_;//����༶��id��Ҳ����vector<timetalbe>���е����,��Ҫ�Լ�������Ҫȥ����
	string roomname_;//������ҵ�����
	vector<vector<ClassUnit*>> roomtable_;//���ҿα�ָ��ֻ�뵱ǰ�Լ�clsque���е�����

	//�ӽڴ�ָ��ĳɽڴ���ţ����㿽����ֵ�Ȳ���
	vector<int> clsque_;
	//vector<ClassUnit> cls_que_;//�����ָ������
	//vector<vector<bool>> tabletag_;//������ʱ�ڶ��Ƿ��ܱ����ŷſΣ���ʼ���α���
	void CalFitness(int& crash);

	TimeTable(string roomname, int days, int periods, long long class_id, long long dbid);
	TimeTable(const TimeTable& t);
	TimeTable& operator=(const TimeTable& t);
	void Init();
	void UpdatePtrs();
	ClassUnit** GetClsUnitPtr(int day, int period);

	void SwapUnits(ClassUnit* origin, ClassUnit* target);
private:
	

};