#include "dbutils.h" 

#import "C:\Program Files\Common Files\System\ado\msado15.dll"  no_namespace rename("EOF", "adoEOF") 

string Dbutils::StartPk(string pktaskid) { 
	pktaskid_ = pktaskid;
	statement_ = "";
	GetDBInfo();
	GetDataFromTable(&Dbutils::Get_T_PKTask, "T_PKTask");
	GetDataFromTable(&Dbutils::Get_T_PKCourse, "T_PKCourse");
	GetDataFromTable(&Dbutils::Get_T_PKTeacher, "T_PKTeacher");
	GetDataFromTable(&Dbutils::Get_T_PKClass, "T_PKClass");
	//������༶�ϵĿθ�ȡ����
	GetDataFromTable(&Dbutils::Get_T_PKClassCourse, "T_PKClassCourse");

	//ȡ������༶���е����ÿκ�Ԥ�źͺϰ��������Ϣ
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseOrgSectionSet, "T_PKClassCourseOrgSectionSet");
	//���²��ԣ��������еĽڴζ������ϣ�Ȼ���ٽ���ɾ����Ҳ�����Ȱ�������Ϣ����������ɾ������
	//ȡ��������صĲ��ſε�ʱ��
	//һ�������֣���ѧ�࣬�����ࣨ���ң�����ʦ����Ŀ
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseNonSection, "T_PKClassCourseNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKClassNonSection, "T_PKClassNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKTeacherNonSection, "T_PKTeacherNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKCourseNonSection, "T_PKCourseNonSection");

	//��Ϊ����ϵͳ������ȷ���ò��ͺϰ�ͬʱ����
	//�ж��Ƿ��кϰ�����
	if (!unionclstab_.empty()) {
		UpdateUnionCls();
	}
	//�ж��Ƿ������õ����
	//����������ص㣬��Ϊ֮ǰ���������жԵĽ�ѧ��
	//Ȼ�������кܶ��ѧ�����Ϊ���õĹ�ϵ��Ҫ��ɾ��
	//������Ҫɾ���ľ��ǽ�ѧ�࣬���ң���ʦ����Ŀ���������еĲ���
	if (!continues_cls_tab_.empty()) {
		UpdateContinueCls();
	}

	//�˴���Ҫ�������úͺϰ����������ϵ������нڴε���Ϣ
	//�Խڴν���һ�����򣬷�����пα��ڳ�ʼ����ʱ��Ĳ���
	sort(clsque_.begin(), clsque_.end());
	//�ڴ�����֮����Ҫ�ٽ��ڴε�index������Ӧ�Ľ��ң���ʦ����Ŀ�Ķ��е���ȥ
	UpdateQueIndex();
	return statement_;
}

string Dbutils::StartPk() {
	//pktaskid_ = pktaskid;
	pktaskid_ = "1";
	statement_ = "";
	GetDBInfo();
	GetDataFromTable(&Dbutils::Get_T_PKTask, "T_PKTask");
	GetDataFromTable(&Dbutils::Get_T_PKCourse, "T_PKCourse");
	GetDataFromTable(&Dbutils::Get_T_PKTeacher, "T_PKTeacher");
	GetDataFromTable(&Dbutils::Get_T_PKClass, "T_PKClass");
	//������༶�ϵĿθ�ȡ����
	GetDataFromTable(&Dbutils::Get_T_PKClassCourse, "T_PKClassCourse");

	//ȡ������༶���е����ÿκ�Ԥ�źͺϰ��������Ϣ
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseOrgSectionSet, "T_PKClassCourseOrgSectionSet");
	//���²��ԣ��������еĽڴζ������ϣ�Ȼ���ٽ���ɾ����Ҳ�����Ȱ�������Ϣ����������ɾ������
	//ȡ��������صĲ��ſε�ʱ��
	//һ�������֣���ѧ�࣬�����ࣨ���ң�����ʦ����Ŀ
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseNonSection, "T_PKClassCourseNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKClassNonSection, "T_PKClassNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKTeacherNonSection, "T_PKTeacherNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKCourseNonSection, "T_PKCourseNonSection");

	//��Ϊ����ϵͳ������ȷ���ò��ͺϰ�ͬʱ����
	//�ж��Ƿ��кϰ�����
	if (!unionclstab_.empty()) {
		UpdateUnionCls();
	}
	//�ж��Ƿ������õ����
	//����������ص㣬��Ϊ֮ǰ���������жԵĽ�ѧ��
	//Ȼ�������кܶ��ѧ�����Ϊ���õĹ�ϵ��Ҫ��ɾ��
	//������Ҫɾ���ľ��ǽ�ѧ�࣬���ң���ʦ����Ŀ���������еĲ���
	if (!continues_cls_tab_.empty()) {
		UpdateContinueCls();
	}

	//�˴���Ҫ�������úͺϰ����������ϵ������нڴε���Ϣ
	//�Խڴν���һ�����򣬷�����пα��ڳ�ʼ����ʱ��Ĳ���
	sort(clsque_.begin(), clsque_.end());
	//�ڴ�����֮����Ҫ�ٽ��ڴε�index������Ӧ�Ľ��ң���ʦ����Ŀ�Ķ��е���ȥ
	UpdateQueIndex();
	return statement_;
}

void Dbutils::OutPutResult() {
}

void Dbutils::CutString(string & s) {
	s = s.substr(s.find("=") + 1, s.length() - s.find("=") - 1);
}

string Dbutils::SetDBProperties() {
	string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
			+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";
	return s;
}

void Dbutils::GetDBInfo() {
	fstream fin("pkjdbc.properties");
	//fin >> dbname_ >> dbuser_name_ >> dbuser_pwd_;
	fin >> db_server_ >> db_port_ >> dbname_ >> dbuser_name_ >> dbuser_pwd_;
	CutString(db_server_);
	CutString(db_port_);
	CutString(dbname_);
	CutString(dbuser_name_);
	CutString(dbuser_pwd_);
	cout << db_port_ << " " << dbname_ << " " << dbuser_name_ << " " << dbuser_pwd_ << endl;
	fin.close();
}

void Dbutils::GetDataFromTable(void(Dbutils::* funcptr)(_RecordsetPtr &m_pRecordset), string tablename) {
	//cout << "�������ݿ⣬��SQL Server  ����PKTaskInfo��Ϣ" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr����ָ��ʵ����ʧ�ܣ�����" << endl;
		return;
	}
	else {
		try {
			//���ط����� ��PKTaskInfo���ݿ� ��¼��Ϊabc, ����Ϊ123
			/*string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
			+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";*/
			string s = SetDBProperties();
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //��¼������ָ�룬����ִ��SQL��䲢��¼��ѯ���
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "��¼������ָ��ʵ����ʧ�ܣ�" << endl;
			return;
		}
		try {
			//string s = "SELECT * FROM T_PKTask where id=" + pktaskid_;
			string s = "SELECT * FROM " + tablename + " where pkTaskId=" + pktaskid_;
			//string s = "SELECT * FROM T_PKTask where id=2";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���		
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		try {
			(this->*funcptr)(m_pRecordset);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
	}
}

void Dbutils::Get_T_PKTask(_RecordsetPtr & m_pRecordset) {
	_variant_t var;
	m_pRecordset->MoveFirst();
	var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"))->Value;
	task_name_ = static_cast<const char*>(static_cast<_bstr_t>(var));
	cout << "task_name_ " << task_name_  << endl;
	term_id_ = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("termId"))->Value);
	cout << "term_id_ " << term_id_ << endl;
	var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("termName"))->Value;
	term_name_ = static_cast<const char*>(static_cast<_bstr_t>(var));
	cout << "term_name_ " << term_name_ << endl;
	var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("gradeName"))->Value;
	grade_name_ = static_cast<const char*>(static_cast<_bstr_t>(var));
	cout << "grade_name_ " << grade_name_ << endl;

	day_num_ = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("dayNum"))->Value);
	cout << "day_num_ " << day_num_ << endl;
	am_num_ = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("amNum"))->Value);
	cout << "am_num_ " << am_num_ << endl;
	pm_num_ = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pmNum"))->Value);
	cout << "pm_num_ " << pm_num_ << endl;
	ev_num_ = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("evNum"))->Value);
	cout << "ev_num_ " << ev_num_ << endl;
}

void Dbutils::Get_T_PKCourse(_RecordsetPtr & m_pRecordset) {
	string name, jname;
	_variant_t var;
	long long dbid;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"))->Value;
		name = static_cast<const char*>(static_cast<_bstr_t>(var));	

		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("jname"))->Value;
		jname = static_cast<const char*>(static_cast<_bstr_t>(var));	

		dbid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id"))->Value);

		couque_.push_back(Course(jname, name, dbid));
		couque_.back().course_id_ = couque_.size() - 1;
		coutab_[dbid] = couque_.back();//�γ�id�Ϳγ�����Ӧ��ӳ��
		couinque_[dbid] = couque_.size() - 1;
		m_pRecordset->MoveNext();
	}
	cout << "end of get all courses" << endl;
}

void Dbutils::Get_T_PKTeacher(_RecordsetPtr & m_pRecordset) {
	_variant_t var;
	string name;
	long long pkcourseid, teacherid, dbid;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF){
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"))->Value;
		name = static_cast<const char*>(static_cast<_bstr_t>(var));	
	
		pkcourseid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCourseId"))->Value);
		teacherid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("teacherId"))->Value);
		dbid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id"))->Value);

		teaque_.push_back(Teacher(name, pkcourseid, teacherid, dbid));
		teaque_.back().teacherid_ = teaque_.size() - 1;
		teaque_.back().couque_.push_back(coutab_[pkcourseid]);
		//teainque_[dbid] = couque_.size() - 1;
		teainque_[dbid] = teaque_.size() - 1;
		m_pRecordset->MoveNext();
	}
	cout << "end of get all teachers" << endl;
}

void Dbutils::Get_T_PKClass(_RecordsetPtr & m_pRecordset) {
	_variant_t var;
	string name;
	long long classid, dbid;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"))->Value;
		name = static_cast<const char*>(static_cast<_bstr_t>(var));	
		
		classid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("classId"))->Value);
		dbid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id"))->Value);
		
		roomque_.push_back(TimeTable(name, day_num_, am_num_ + pm_num_, classid, dbid));
		roomque_.back().roomid_ = roomque_.size() - 1;
		//roomtab_[roomque_.back().roomid_] = roomque_.back();
		roominque_[dbid] = roomque_.size() - 1;
		m_pRecordset->MoveNext();
	}
	cout << "end of get all rooms" << endl;
}

void Dbutils::Get_T_PKClassCourse(_RecordsetPtr & m_pRecordset) {
	//�����ݿ⵱�а���Ӧ�ľ���ÿ����Ŀθ�ȥȡ����`
	//��ʵ�ǽ�ÿ����Ľڴθ�������������Ҫ�õ�new
	long long pkclass, pkteacher, pkcourse, id;
	//int evensection, lessonnum;
	int lessonnum;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkclass = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClass"))->Value);
		pkcourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCourse"))->Value);
		pkteacher = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkTeacher"))->Value);
		lessonnum = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("lessonNum"))->Value);
		id = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id"))->Value);
		//����ĳһ���κ;���༶��ӳ��
		cou_room_tab_[id] = roominque_[pkclass];
		//1.��ÿ���ڴζ����ɳ���
		//2.��ÿ�ڿκ���Ӧ�İ༶���Ϲ�ϵ
		//3.�ѿκ���ʦ֮��ĵĹ�ϵ������
		//4.���������Ľڴκ��������Ľ�ѧ�࣬һ����ѧ���Ӧ����ڴ�
		//5.���������Ľڴκ��������Ŀ�Ŀ��ϵ����
		for (auto i = 0; i < lessonnum; i++) {
			//������캯���ǳ�����Ҫ��ֱ��Ϊ����ʡȥ�˺ܶ�ָ����±���µĲ���
			clsque_.push_back(ClassUnit(&roomque_[roominque_[pkclass]], &teaque_[teainque_[pkteacher]], &couque_[couinque_[pkcourse]]));
			//auto clsptr = new ClassUnit(&roomque_[roominque_[pkclass]], &teaque_[teainque_[pkteacher]], couque_[couinque_[pkcourse]]);
			//��Ϊ���ݿ�����һ�ڿ�Ϊ��λ�����㷨��������һ�ο�Ϊ��λ�����Ե�ǰ��������Ҫ�������и���
			//���õ�ǰ��ڿε���ţ���������������ڿε�һЩ��Ϣ��ʱ��Ҫ�õ�
			auto clsindex = clsque_.size() - 1;
			//clsptr->secionno_ = i + 1;
			clsque_[clsindex].secionno_ = i + 1;
			clsque_[clsindex].dbid_ = id;
			//unitstab_[pkclass][pkcourse][pkteacher][i + 1] = clsptr;
			unitstab_[id][i + 1] = clsindex;
			//��������������ζ�Ӧ�ļ��ڿ�
			class_course_units_[id].push_back(clsindex);
			//��������˽�ѧ��Ŀγ̣�һ����ѧ��Ŀγ̺��ж�ڿ�
			roomque_[roominque_[pkclass]].clsqueindex_.push_back(clsindex);
			teaque_[teainque_[pkteacher]].clsqueindex_.push_back(clsindex);
			couque_[couinque_[pkcourse]].clsqueindex_.push_back(clsindex);

			//���ÿγ̵��е�ָ���ϵ	
			//��������ڹ��캯�������Ѿ�����
			/*clsque_[clsptr].ttbptr_ = &roomque_[roominque_[pkclass]];
			clsque_[clsptr].teacher_ = &teaque_[teainque_[pkteacher]];
			clsque_[clsptr].couptr_ = &couque_[couinque_[pkcourse]];*/
			clsque_[clsindex].type_ = 1;
		}
		m_pRecordset->MoveNext();
	}
	cout << "get all class units" << endl;
}

void Dbutils::Get_T_PKClassCourseOrgSectionSet(_RecordsetPtr & m_pRecordset) {
	//ͨ�������Ӧ��ÿ�ڿε���ϢȻ	
	long long pkclasscourse, pkcombinateclassgroup, pkevensection;
	long long pkteacher, pkcourse;
	int secionno, sfevensection, sfpre, section, weekday, sfcombinate;
	int cptr;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkclasscourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClassCourse"))->Value);
		secionno = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("secionno"))->Value);
		sfpre = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sfpre"))->Value);
		sfcombinate = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sfCombinate"))->Value);
		sfevensection = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sfEvenSection"))->Value);

		clsque_[unitstab_[pkclasscourse][secionno]].type_ = 1;//��ʾ�����һ����ͨ�Ŀγ�
		//�����ⲿ����Ҫ�����ж�
		if(sfevensection){
			//��������	
			pkevensection = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkEvenSection"))->Value);
			clsque_[unitstab_[pkclasscourse][secionno]].pkevensection_ = pkevensection;
			clsque_[unitstab_[pkclasscourse][secionno]].type_ = 2;//��ʾ����Ǹ����ÿ�
			cptr = unitstab_[pkclasscourse][secionno];
			continues_cls_tab_[pkevensection].push_back(cptr);
		}
		//������Ϊ��һ�����Ѿ����¹�������Ϣ����Ҫ��������Ϣ���㶨֮������ɾ��������Ľڴ�
		//�������õĿζ������Ȱ���ʦ����Ϣ�ͽڴ���Ϣ���и���
		//������ʦ�ͽ��ҵ���Ϣ	
		//roomque_[cou_room_tab_[pkclasscourse]].clsque_.push_back(cptr);
		//teaque_[teainque_[pkteacher]].clsque_.push_back(cptr);
		if (sfpre) {
			//��ڿ��Ѿ���Ԥ����
			section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section"))->Value);
			weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday"))->Value);
			//sectionperweek = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sectionPerWeek"))->Value);
			//clsque_[unitstab_[pkclasscourse][secionno]].hasbeenput_ = true;
			clsque_[unitstab_[pkclasscourse][secionno]].preput_ = true;
			//clsque_[unitstab_[pkclasscourse][secionno]].stime_ = make_pair(sectionperweek - 1, section - 1);
			clsque_[unitstab_[pkclasscourse][secionno]].pretime_ = make_pair(weekday - 1, section - 1);
		}
		if (sfcombinate) {
			//�Ѿ��ϰ��ˣ�����ϰ��������Ϊ�ǲ���ͬʱ����������
			pkcombinateclassgroup = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCombinateClassGroup"))->Value);
			clsque_[unitstab_[pkclasscourse][secionno]].pkcombinateclassgroup_ = pkcombinateclassgroup;
			unionclstab_[pkcombinateclassgroup].push_back(unitstab_[pkclasscourse][secionno]);
		}
		m_pRecordset->MoveNext();
	}
	cout << "get all units info" << endl;
}

void Dbutils::Get_T_PKClassCourseNonSection(_RecordsetPtr & m_pRecordset) {
	//��ѧ�಻�ſ�ʱ�䣬��Ҫֱ�Ӹ��µ�����ÿ�ڿε��еĲ��ſ�ʱ����	
	int weekday, section;
	long long pkclasscourse;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkclasscourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClassCourse"))->Value);
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday"))->Value);
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section"))->Value);
		for (auto c : class_course_units_[pkclasscourse]) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canntbeput_.find(tmp) == clsque_[c].canntbeput_.end())
				clsque_[c].canntbeput_.insert(tmp);
		}
		m_pRecordset->MoveNext();
	}
	cout << "get all class course not available time" << endl;
}

void Dbutils::Get_T_PKClassNonSection(_RecordsetPtr & m_pRecordset) {
	//���Ҳ��Ͽ�ʱ��
	int weekday, section;
	long long pkclass;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkclass = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClass"))->Value);
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday"))->Value);
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section"))->Value);
		for (auto c : roomque_[roominque_[pkclass]].clsqueindex_) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canntbeput_.find(tmp) == clsque_[c].canntbeput_.end())
				clsque_[c].canntbeput_.insert(tmp);
		}
		m_pRecordset->MoveNext();
	}
	cout << "get all rooms not available time" << endl;
}

void Dbutils::Get_T_PKTeacherNonSection(_RecordsetPtr & m_pRecordset) {
	//��ʦ���Ͽ�ʱ��
	int weekday, section;
	long long pkteacher;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkteacher = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkTeacher"))->Value);
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday"))->Value);
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section"))->Value);
		for (auto c : teaque_[teainque_[pkteacher]].clsqueindex_) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canntbeput_.find(tmp) == clsque_[c].canntbeput_.end())
				clsque_[c].canntbeput_.insert(tmp);
		}
		m_pRecordset->MoveNext();
	}
	cout << "get all teacher not available time" << endl;
}

void Dbutils::Get_T_PKCourseNonSection(_RecordsetPtr & m_pRecordset) {
	//��Ŀ���Ͽ�ʱ��
	int weekday, section;
	long long pkcourse;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkcourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCourse"))->Value);
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday"))->Value);
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section"))->Value);
		for (auto c : couque_[couinque_[pkcourse]].clsqueindex_) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canntbeput_.find(tmp) == clsque_[c].canntbeput_.end())
				clsque_[c].canntbeput_.insert(tmp);
		}
		m_pRecordset->MoveNext();
	}	
	cout << "get all course not available time" << endl;
}

void Dbutils::UpdateUnionCls() {
	//��ÿ���ϰ�Ľڴζ���ϵ����
	//�����������кϰ�εİ༶�����±�
	for (auto i = 0; i < clsque_.size(); i++) {
		if (clsque_[i].pkcombinateclassgroup_ != -1) {
			for (auto j = 0; j < clsque_.size(); j++) {
				if (i != j && clsque_[i].pkcombinateclassgroup_ == clsque_[j].pkcombinateclassgroup_) {
					clsque_[i].union_cls_index_.push_back(j);
				}
			}
		}
	}
}

void Dbutils::UpdateContinueCls() {
	//������Ҫ���н����ݿ⵱�е�һ�ڿα���ſε��е�һ�οΣ������Ҫɾ���Ѿ����ɵ����ÿεĶ���Ĳ���
	//�������������Ҫ���½��Һͽ�ʦ��������ε���Ϣ
	//������Ҫ������ƣ�Ϊ�˺�ԭ����ϵͳ���жԽӣ����������Ľڴ�ȥ����Ȼ����ӳ�䵽�µĶ���λ����
	for (auto& vec : continues_cls_tab_) {
		auto cptr = (vec.second)[0];
		//cptr->teacher_->clsque_.push_back(cptr);
		//cptr->ttbptr_->clsque_.push_back(cptr);
		clsque_[cptr].duration_ = (vec.second).size();
		//clsque_[cptr].type_ = 2;
		for (auto i = 1; i < (vec.second).size(); i++) {
			//ֱ�Ӽ�¼�ļ����ڴα�ɾ���������ǲ�û��������ȥɾ����Щ�ڴ�
			deleted_units_set_.insert((vec.second)[i]);
			//delete (vec.second)[i];
		}
	}
	map<ClassUnit, int> new_pos_;
	vector<ClassUnit> tmpque;
	for (auto i = 0; i < clsque_.size(); i++) {
		if (deleted_units_set_.find(i) == deleted_units_set_.end()) {
			tmpque.push_back(clsque_[i]);
			new_pos_[clsque_[i]] = tmpque.size() - 1;
		}
	}
	clsque_ = tmpque;
	//����clsque����Ҫɾ���Ľڴζ��Ѿ���ɾ����
	cout << "end of delete continues units" << endl;
}

template <typename T>
void ClearQueIndex(vector<T>& vec){
	for (auto& t : vec) {
		t.clsqueindex_.clear();
	}
}

void Dbutils::UpdateQueIndex() {
	//��ȫ���������ԭ�ȵ�index��Ȼ���ٰ����������Ľڴ��б������Ӧ�Ļָ�
	ClearQueIndex(couque_);
	ClearQueIndex(teaque_);
	ClearQueIndex(roomque_);
	for (auto i = 0; i < clsque_.size(); i++) {
		clsque_[i].couptr_->clsqueindex_.push_back(i);
		clsque_[i].teacher_->clsqueindex_.push_back(i);
		clsque_[i].ttbptr_->clsqueindex_.push_back(i);
	}
	
	cout << "end of update index and union units" << endl;
}

