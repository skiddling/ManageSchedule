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
	//将这个班级上的课给取出来
	GetDataFromTable(&Dbutils::Get_T_PKClassCourse, "T_PKClassCourse");

	//取出这个班级当中的连堂课和预排和合班这类的信息
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseOrgSectionSet, "T_PKClassCourseOrgSectionSet");
	//更新策略，先让所有的节次都关联上，然后再进行删除，也就是先把所有信息都补上再做删除处理
	//取出所有相关的不排课的时间
	//一共分四种：教学班，行政班（教室），教师，科目
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseNonSection, "T_PKClassCourseNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKClassNonSection, "T_PKClassNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKTeacherNonSection, "T_PKTeacherNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKCourseNonSection, "T_PKCourseNonSection");

	//因为这里系统当中明确连堂不和合班同时发生
	//新版本这里不用这个步骤，这个步骤在排序之后处理
	//判断是否有合班的情况
	/*if (!unionclstab_.empty()) {
		UpdateUnionCls();
	}*/
	//判断是否有连堂的情况
	//这个部分是重点，因为之前生成了所有对的教学班
	//然后这里有很多教学班会因为连堂的关系需要被删除
	//这里需要删除的就是教学班，教室，教师，科目这三个当中的部分
	if (!continues_cls_tab_.empty()) {
		UpdateContinueCls();
	}

	//此处需要更新连堂和合班这两个集合当中所有节次的信息
	//对节次进行一个排序，方便进行课表在初始化的时候的操作
	sort(clsque_.begin(), clsque_.end());
	//节次排序之后需要再将节次的index给到相应的教室，教师，科目的队列当中去
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
	//将这个班级上的课给取出来
	GetDataFromTable(&Dbutils::Get_T_PKClassCourse, "T_PKClassCourse");

	//取出这个班级当中的连堂课和预排和合班这类的信息
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseOrgSectionSet, "T_PKClassCourseOrgSectionSet");
	//更新策略，先让所有的节次都关联上，然后再进行删除，也就是先把所有信息都补上再做删除处理
	//取出所有相关的不排课的时间
	//一共分四种：教学班，行政班（教室），教师，科目
	GetDataFromTable(&Dbutils::Get_T_PKClassCourseNonSection, "T_PKClassCourseNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKClassNonSection, "T_PKClassNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKTeacherNonSection, "T_PKTeacherNonSection");
	GetDataFromTable(&Dbutils::Get_T_PKCourseNonSection, "T_PKCourseNonSection");

	//因为这里系统当中明确连堂不和合班同时发生
	//新版本这里不用这个步骤，这个步骤在排序之后处理
	//判断是否有合班的情况
	/*if (!unionclstab_.empty()) {
		UpdateUnionCls();
	}*/
	//判断是否有连堂的情况
	//这个部分是重点，因为之前生成了所有对的教学班
	//然后这里有很多教学班会因为连堂的关系需要被删除
	//这里需要删除的就是教学班，教室，教师，科目这三个当中的部分
	if (!continues_cls_tab_.empty()) {
		UpdateContinueCls();
	}

	//此处需要更新连堂和合班这两个集合当中所有节次的信息
	//对节次进行一个排序，方便进行课表在初始化的时候的操作
	sort(clsque_.begin(), clsque_.end());
	//节次排序之后需要再将节次的index给到相应的教室，教师，科目的队列当中去
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
	//cout << "连接数据库，从SQL Server  读入PKTaskInfo信息" << endl;
	CoInitialize(NULL);
	_ConnectionPtr  sqlSp;
	HRESULT hr = sqlSp.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)) {
		cout << "_ConnectionPtr对象指针实例化失败！！！" << endl;
		return;
	}
	else {
		try {
			//本地服务器 打开PKTaskInfo数据库 登录名为abc, 密码为123
			/*string s = "Provider=SQLOLEDB;Server=127.0.0.1,1433;Database=" + dbname_ + ";uid="
			+ dbuser_name_ + ";pwd=" + dbuser_pwd_ + ";";*/
			string s = SetDBProperties();
			_bstr_t strConnect = s.c_str();
			sqlSp->Open(strConnect, "", "", adModeUnknown);
		}
		catch (_com_error &e) {
			cerr << static_cast<string>(e.Description());
		}
		_RecordsetPtr m_pRecordset; //记录集对象指针，用来执行SQL语句并记录查询结果
		if (FAILED(m_pRecordset.CreateInstance(_uuidof(Recordset)))) {
			cout << "记录集对象指针实例化失败！" << endl;
			return;
		}
		try {
			//string s = "SELECT * FROM T_PKTask where id=" + pktaskid_;
			string s = "SELECT * FROM " + tablename + " where pkTaskId=" + pktaskid_;
			//string s = "SELECT * FROM T_PKTask where id=2";
			m_pRecordset->Open(s.c_str(), (IDispatch*)sqlSp, adOpenDynamic, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句		
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
	var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"));
	task_name_ = static_cast<const char*>(static_cast<_bstr_t>(var));
	
	term_id_ = static_cast<long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("termId")));

	var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("termName"));
	term_name_ = static_cast<const char*>(static_cast<_bstr_t>(var));

	var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("gradeName"));
	grade_name_ = static_cast<const char*>(static_cast<_bstr_t>(var));


	day_num_ = static_cast<long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("dayNum")));

	am_num_ = static_cast<long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("amNum")));

	pm_num_ = static_cast<long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pmNum")));

	pm_num_ = static_cast<long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pmNum")));

	ev_num_ = static_cast<long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("evNum")));
}

void Dbutils::Get_T_PKCourse(_RecordsetPtr & m_pRecordset) {
	string name, jname;
	_variant_t var;
	long long dbid;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"));
		name = static_cast<const char*>(static_cast<_bstr_t>(var));	

		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("jname"));
		jname = static_cast<const char*>(static_cast<_bstr_t>(var));	

		dbid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id")));

		couque_.push_back(Course(jname, name, dbid));
		couque_.back().course_id_ = couque_.size() - 1;
		coutab_[dbid] = couque_.back();//课程id和课程做相应的映射
		couinque_[dbid] = couque_.size() - 1;
	}
}

void Dbutils::Get_T_PKTeacher(_RecordsetPtr & m_pRecordset) {
	_variant_t var;
	string name;
	long long pkcourseid, teacherid, dbid;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF){
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"));
		name = static_cast<const char*>(static_cast<_bstr_t>(var));	
	
		pkcourseid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCourseId")));
		teacherid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("teacherId")));
		dbid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id")));

		teaque_.push_back(Teacher(name, pkcourseid, teacherid, dbid));
		teaque_.back().teacherid_ = teaque_.size() - 1;
		teaque_.back().couque_.push_back(coutab_[pkcourseid]);
		teainque_[dbid] = couque_.size() - 1;
	}
}

void Dbutils::Get_T_PKClass(_RecordsetPtr & m_pRecordset) {
	_variant_t var;
	string name;
	long long classid, dbid;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		var = m_pRecordset->Fields->GetItem(static_cast<_variant_t>("name"));
		name = static_cast<const char*>(static_cast<_bstr_t>(var));	
		
		classid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("classId")));
		dbid = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id")));
		
		roomque_.push_back(TimeTable(name, day_num_, am_num_ + pm_num_, classid, dbid));
		roomque_.back().roomid_ = roomque_.size() - 1;
		roomtab_[roomque_.back().roomid_] = roomque_.back();
		roominque_[dbid] = roomque_.size() - 1;
	}
}

void Dbutils::Get_T_PKClassCourse(_RecordsetPtr & m_pRecordset) {
	//从数据库当中把相应的具体每个班的课给去取出来`
	//其实是将每个班的节次给产生出来，需要用到new
	long long pkclass, pkteacher, pkcourse, id;
	//int evensection, lessonnum;
	int lessonnum;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkclass = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClass")));
		pkcourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCourse")));
		pkteacher = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkTeacher")));
		lessonnum = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("lessonNum")));
		id = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("id")));
		//具体某一个课和具体班级的映射
		cou_room_tab_[id] = roominque_[pkclass];
		//1.把每个节次都生成出来
		//2.把每节课和相应的班级挂上关系
		//3.把课和老师之间的的关系连起来
		//4.把这个具体的节次和这个具体的教学班，一个教学班对应多个节次
		//5.把这个具体的节次和这个具体的科目联系起来
		for (auto i = 0; i < lessonnum; i++) {
			//这个构造函数非常的重要，直接为后面省去了很多指针和下标更新的操作
			clsque_.push_back(ClassUnit(&roomque_[roominque_[pkclass]], &teaque_[teainque_[pkteacher]], &couque_[couinque_[pkcourse]]));
			//auto clsptr = new ClassUnit(&roomque_[roominque_[pkclass]], &teaque_[teainque_[pkteacher]], couque_[couinque_[pkcourse]]);
			//因为数据库是以一节课为单位，而算法当中是以一次课为单位，所以当前产生的需要在最后进行更新
			//设置当前这节课的序号，接下来在设置这节课的一些信息的时候要用到
			auto clsindex = clsque_.size() - 1;
			//clsptr->secionno_ = i + 1;
			clsque_[clsindex].secionno_ = i + 1;
			//unitstab_[pkclass][pkcourse][pkteacher][i + 1] = clsptr;
			unitstab_[id][i + 1] = clsindex;
			//具体这个班的这个课对应哪几节课
			class_course_units_[id].push_back(clsindex);
			//这个代表了教学班的课程，一个教学班的课程含有多节课
			roomque_[roominque_[pkclass]].clsqueindex_.push_back(clsindex);
			teaque_[teainque_[pkteacher]].clsqueindex_.push_back(clsindex);
			couque_[couinque_[pkcourse]].clsqueindex_.push_back(clsindex);

			//设置课程当中的指向关系	
			//这个好像在构造函数当中已经做了
			/*clsque_[clsptr].ttbptr_ = &roomque_[roominque_[pkclass]];
			clsque_[clsptr].teacher_ = &teaque_[teainque_[pkteacher]];
			clsque_[clsptr].couptr_ = &couque_[couinque_[pkcourse]];*/
			clsque_[clsindex].type_ = 1;
		}
	}
}

void Dbutils::Get_T_PKClassCourseOrgSectionSet(_RecordsetPtr & m_pRecordset) {
	//通过获得相应的每节课的信息然	
	long long pkclasscourse, pkcombinateclassgroup, pkevensection;
	long long pkteacher, pkcourse;
	int secionno, sfevensection, sfpre, section, sectionperweek, sfcombinate;
	int cptr;
	m_pRecordset->MoveFirst();
	while (!m_pRecordset->adoEOF) {
		pkclasscourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClassCourse")));
		secionno = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("secionno")));
		sfpre = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sfpre")));
		sfcombinate = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sfCombinate")));
		sfevensection = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sfEvenSection")));
		//连堂这部分需要单独判断
		if(sfevensection){
			//连堂设置	
			pkevensection = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkEvenSection")));
			clsque_[unitstab_[pkclasscourse][secionno]].pkevensection_ = pkevensection;
			clsque_[unitstab_[pkclasscourse][secionno]].type_ = 2;//表示这个是个连堂课
			cptr = unitstab_[pkclasscourse][secionno];
			continues_cls_tab_[pkevensection].push_back(cptr);
		}
		//最新因为上一个表已经更新过所有信息，需要在所有信息都搞定之后最终删除掉多余的节次
		//不是连堂的课都可以先把老师的信息和节次信息进行更新
		//更新老师和教室的信息	
		//roomque_[cou_room_tab_[pkclasscourse]].clsque_.push_back(cptr);
		//teaque_[teainque_[pkteacher]].clsque_.push_back(cptr);
		if (sfpre) {
			//这节课已经被预排了
			section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section")));
			sectionperweek = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("sectionPerWeek")));
			//clsque_[unitstab_[pkclasscourse][secionno]].hasbeenput_ = true;
			clsque_[unitstab_[pkclasscourse][secionno]].preput_ = true;
			//clsque_[unitstab_[pkclasscourse][secionno]].stime_ = make_pair(sectionperweek - 1, section - 1);
			clsque_[unitstab_[pkclasscourse][secionno]].pretime_ = make_pair(sectionperweek - 1, section - 1);
		}
		else if (sfcombinate) {
			//已经合班了，这里合班和连堂认为是不能同时发生的条件
			pkcombinateclassgroup = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCombinateClassGroup")));
			clsque_[unitstab_[pkclasscourse][secionno]].pkcombinateclassgroup_ = pkcombinateclassgroup;
			unionclstab_[pkcombinateclassgroup].push_back(unitstab_[pkclasscourse][secionno]);
		}
		else {
			clsque_[unitstab_[pkclasscourse][secionno]].type_ = 1;//表示这个是一个普通的课程
		}
	}
}

void Dbutils::Get_T_PKClassCourseNonSection(_RecordsetPtr & m_pRecordset) {
	//教学班不排课时间，需要直接更新到具体每节课当中的不排课时间上	
	int weekday, section;
	long long pkclasscourse;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (m_pRecordset->adoEOF) {
		pkclasscourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClassCourse")));
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday")));
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section")));
		for (auto c : class_course_units_[pkclasscourse]) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canbeput_.find(tmp) == clsque_[c].canbeput_.end())
				clsque_[c].canbeput_.insert(tmp);
		}
	}
}

void Dbutils::Get_T_PKClassNonSection(_RecordsetPtr & m_pRecordset) {
	//教室不上课时间
	int weekday, section;
	long long pkclass;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (m_pRecordset->adoEOF) {
		pkclass = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkClass")));
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday")));
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section")));
		for (auto c : roomque_[roominque_[pkclass]].clsqueindex_) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canbeput_.find(tmp) == clsque_[c].canbeput_.end())
				clsque_[c].canbeput_.insert(tmp);
		}
	}
}

void Dbutils::Get_T_PKTeacherNonSection(_RecordsetPtr & m_pRecordset) {
	//老师不上课时间
	int weekday, section;
	long long pkteacher;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (m_pRecordset->adoEOF) {
		pkteacher = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkTeacher")));
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday")));
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section")));
		for (auto c : teaque_[teainque_[pkteacher]].clsqueindex_) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canbeput_.find(tmp) == clsque_[c].canbeput_.end())
				clsque_[c].canbeput_.insert(tmp);
		}
	}
}

void Dbutils::Get_T_PKCourseNonSection(_RecordsetPtr & m_pRecordset) {
	//科目不上课时间
	int weekday, section;
	long long pkcourse;
	pair<int, int> tmp;
	m_pRecordset->MoveFirst();
	while (m_pRecordset->adoEOF) {
		pkcourse = static_cast<long long>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("pkCourse")));
		weekday = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("weekday")));
		section = static_cast<int>(m_pRecordset->Fields->GetItem(static_cast<_variant_t>("section")));
		for (auto c : couque_[couinque_[pkcourse]].clsqueindex_) {
			tmp = make_pair(weekday - 1, section - 1);
			if (clsque_[c].canbeput_.find(tmp) == clsque_[c].canbeput_.end())
				clsque_[c].canbeput_.insert(tmp);
		}
	}	
}

void Dbutils::UpdateUnionCls() {
	//将每个合班的节次都联系起来
	for (auto& vec : unionclstab_) {
		for (auto& cls : vec.second) {
			for (auto& ptr : vec.second) {
				if (ptr != cls) {
					//clsque_[cls].unioncls_.push_back(ptr);
					clsque_[cls].union_cls_index_.push_back(ptr);
				}
			}
		}
	}
}

void Dbutils::UpdateContinueCls() {
	//这里需要进行将数据库当中的一节课编程排课当中的一次课，这个需要删除已经生成的连堂课的多余的部分
	//并且这个部分需要更新教室和教师关于这个课的信息
	//这里需要重新设计，为了和原本的系统进行对接，把这里多余的节次去掉，然后做映射到新的队列位置上
	for (auto& vec : continues_cls_tab_) {
		auto cptr = (vec.second)[0];
		//cptr->teacher_->clsque_.push_back(cptr);
		//cptr->ttbptr_->clsque_.push_back(cptr);
		clsque_[cptr].duration_ = (vec.second).size();
		//clsque_[cptr].type_ = 2;
		for (auto i = 1; i < (vec.second).size(); i++) {
			//直接记录哪几个节次被删除掉，但是并没有真正的去删除这些节次
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
	//至此clsque当中要删除的节次都已经被删除了
}

template <typename T>
void ClearQueIndex(vector<T>& vec){
	for (auto& t : vec) {
		t.clsqueindex_.clear();
	}
}

void Dbutils::UpdateQueIndex() {
	//先全部重新清除原先的index，然后再按照排序过后的节次列表进行相应的恢复
	ClearQueIndex(couque_);
	ClearQueIndex(teaque_);
	ClearQueIndex(roomque_);
	for (auto i = 0; i < clsque_.size(); i++) {
		clsque_[i].couptr_->clsqueindex_.push_back(i);
		clsque_[i].teacher_->clsqueindex_.push_back(i);
		clsque_[i].ttbptr_->clsqueindex_.push_back(i);
	}
	//暴力更新所有合班课的班级数组下标
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
