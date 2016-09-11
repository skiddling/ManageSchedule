#pragma once
#include "course.h"

/*
�������������࣬��Ϊ����ʵ��������Ի��ڽ����������ӣ�
��������൱�е����ݿ��ܻ��ڽ������ϵ����,
������Ҫ�Ѹ����趨Ϊģ�飬�Ա㽫���Ĺ������
Ŀǰ�����趨���¼���������
1.ĳ��ʦ��ĳЩ�ڴε��в��ܰ����Ͽ�
2.ĳ��ʦ��ĳЩ��Ҫ��Ԥ�Ȱ�����һЩ�ڴε����Ͽ�
3.ĳЩ��Ŀ�Ŀγ̲��ܱ�������ĳЩʱ����Ͽ�
4.ĳЩ��ʦ��ĳ���ο���Ҫ�����ϣ�Ҳ������ν�����ÿ�
*/
/*
�����Ϊһ�οε��࣬Ҳ����һ����λ��ʾһ�ο�
�Դ�������������
1.��Ϊ��ĳ��ʦ��ĳ���༶�Ŀβ�����ĳЩʱ����ϿΣ����ſ�ʱ�����У����ڶ��ʱ��β��ܱ��ſΣ�
2.��Ϊ��ĳ��ʦ��ĳ���༶�Ŀ��Ѿ���Ԥ���ź���ĳЩʱ��Σ�Ԥ�ſ�ʱ��
3.���ñ�־�������ýڿ��Ƿ������ÿΣ��Լ��ǵڼ������ÿΣ��޶�һ������ֻ�������ڿ�
*/
class ClassUnit
{
public:
	int dbid_;//���ݿ��ȡ������id��
	static int pim;//shrot for period in moring
	int unit_id_;//�ýڿε�id���������пε��е�˳��,��teacher���е�class_que���е�ֵ��Ӧ
	int class_id_;//�༶��
	string class_name_;//�༶��
	int course_id_;
	string course_name_;
	Teacher teacher_;//�ýڿε��ο���ʦ
	//������������ʦ����Ϣ�������
	bool alterable_;//Ԥ�ſα�־��1��ʾ��Ҫ���ſΣ�0��ʾ�Ѿ��޶��ýڿε�ʱ�䣬���ܱ�����
	int	continue_tag_;//���ñ�־��0��ʾ�ýڴβ������ÿͣ�1��ʾ��������ÿ͵ĵ�һ�ڣ�2��ʾ�ڶ���
	vector<pair<int, int> > cant_time_;//�ýڿβ��ܱ������Ͽε�ʱ������
	set<pair<int, int> > canttime_;
	pair<int, int> class_time_;//�ýڿεľ�����Ͽ�ʱ��

	ClassUnit();
	ClassUnit(Teacher teacher, int class_id, string class_name, string course_name, int course_id, int dbid);
	void AddCant(vector<pair<int, int> > &cant_time);
	inline pair<int, int> get_time();
	void AddUnit(int x, int y, vector<Teacher> &teachers);
	void DelUnit(vector<Teacher> &teachers);
	bool IsConflict(vector<Teacher> &teachers);
	bool CheckUnitTime(int x, int y);
};
