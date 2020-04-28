#include "sqlite_operation.h"
#include <codecvt>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

//在windows操作系统VS中，默认unicode编码
//函数作用将unicode转换为utf-8编码，兼容sqlite数据库汉字输入
//需要手动释放返回值申请的空间
char *UnicodeToUtf8(const char *str)
{
	int length = strlen(str) + 1;
	wchar_t *unicode = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(unicode, 0, length * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str), unicode, length);

	length = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(length + 1);
	memset(szUtf8, 0, length + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, length, NULL, NULL);
	free(unicode);
	return szUtf8;
}
//在windows操作系统VS中，默认unicode编码
//函数作用将utf-8转换为unicode编码，兼容sqlite数据库汉字读出
//需要手动释放返回值申请的空间
char *Utf8ToUnicode(const char* szUtf8)
{
	int length = strlen(szUtf8) + 1;
	wchar_t *unicode = (wchar_t*)malloc(sizeof(wchar_t)*length);
	memset(unicode, 0, length * sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0, szUtf8, strlen(szUtf8), unicode, length);

	length = WideCharToMultiByte(CP_ACP, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *str = (char*)malloc(length + 1);
	memset(str, 0, length + 1);
	WideCharToMultiByte(CP_ACP, 0, unicode, -1, str, length, NULL, NULL);
	free(unicode);

	return str;
	/*
	//UTF8 to Unicode
    //预转换，得到所需空间的大小
    int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
    //分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
    wchar_t* wszString = new wchar_t[wcsLen + 1];
    //转换
    ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
    //最后加上'\0'
    wszString[wcsLen] = '\0';
 
    char *m_char;
    int len = WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
    m_char = new char[len + 1];
    WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), m_char, len, NULL, NULL);
    m_char[len] = '\0';
    return m_char;
	*/
}

bool CompareChar(unsigned char *ch1, int len1, unsigned char *ch2, int len2)
{
	if (len1 != len2)
	{
		return false;
	}		
	else
	{
		int i = 0;
		for (i = 0; i < len1; ++i)
		{
			if (ch1[i] != ch2[i])
				return false;
		}
		if (i == len1)
		{
			return true;
		}
	}
}

int CharToint(const char *str)
{
	int len = strlen(str);
	int ret = 0;
	for (int i = 0; i < len; ++i)
	{
		ret = ret * 10 + (str[i] - '0');
	}
	return ret;
}

int SQLITE_OPERATION::SqliteInit(void)
{
	int ret = sqlite3_open(SqlDataBaseName, &p_sql_handle_);
	if (ret != SQLITE_OK)
		return SQL_FAILED;
	char *errmsg;
	const char open_foreign_keys[] = "PRAGMA foreign_keys = ON;";
	ret = sqlite3_exec(p_sql_handle_, open_foreign_keys, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
	{
		printf("open_foreign_keys error:%s\n", errmsg);
		return SQL_FAILED;
	}

	ret = sqlite3_exec(p_sql_handle_, SQL_CREAT_USER_APP_TABLE, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
	{ 
		printf("creat user_app table error:%s\n", errmsg);
		return SQL_FAILED;
	}	

	ret = sqlite3_exec(p_sql_handle_, SQL_CREAT_USER_INFO_TABLE, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
	{
		printf("creat user_info table error:%s\n", errmsg);
		return SQL_FAILED;
	}

	ret = sqlite3_exec(p_sql_handle_, SQL_CREATE_USER_FEATURE_TABLE, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
	{
		printf("creat user_feature table error:%s\n", errmsg);
		return SQL_FAILED;
	}

	ret = sqlite3_exec(p_sql_handle_, SQL_CREATE_USER_ACCOUNT_TABLE, NULL, NULL, &errmsg);
	if (ret != SQLITE_OK)
	{
		printf("creat user_account table error:%s\n", errmsg);
		return SQL_FAILED;
	}
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::MemeryInit(void)
{
	this->GetUserAppToMemery();
	this->GetUserMainInfoToMemery();
	this->GetFaceFeatureToMemery();
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::GetFeature2Memery(void)
{
	int ret = sqlite3_prepare_v2(p_sql_handle_, SQL_QUERY_USER_ID_FEATURE, sizeof(SQL_QUERY_USER_ID_FEATURE), &p_query_user_feature_stmt_, 0);

	while (SQLITE_ROW == sqlite3_step(p_query_user_feature_stmt_))
	{
		int id = sqlite3_column_int(p_query_user_feature_stmt_, 0);
		unsigned char *feature_data = (unsigned char *)sqlite3_column_blob(p_query_user_feature_stmt_, 1);
		int len = sqlite3_column_bytes(p_query_user_feature_stmt_, 1);

		memeryFeature[id].feature = (unsigned char *)malloc(len);
		memeryFeature[id].featureLen = len;
		memcpy(memeryFeature[id].feature, feature_data, len);
	}
	sqlite3_finalize(p_query_user_feature_stmt_);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::CreatUser(PERSON_INFORMATIONS *userInfor)
{
	int ret = sqlite3_prepare(pUserHandle, SQL_USER_INFO_RECODE, sizeof(SQL_USER_INFO_RECODE), &pInsert_stmt, 0);

	ret = sqlite3_reset(pInsert_stmt);

	char *name = UnicodeToUtf8(userInfor->personName);
	ret = sqlite3_bind_text(pInsert_stmt, 1, name, strlen(name) + 1, NULL);

	char *sex = UnicodeToUtf8(userInfor->personSex);
	ret = sqlite3_bind_text(pInsert_stmt, 2, sex, strlen(sex) + 1, NULL);

	ret = sqlite3_bind_int (pInsert_stmt, 3, userInfor->personAge);

	char *phone = UnicodeToUtf8(userInfor->phoneNum);
	ret = sqlite3_bind_text(pInsert_stmt, 4, phone, strlen(phone) + 1, NULL);

	char *cardid = UnicodeToUtf8(userInfor->personCardId);
	ret = sqlite3_bind_text(pInsert_stmt, 5, cardid, strlen(cardid) + 1, NULL);

	char *address = UnicodeToUtf8(userInfor->personAddress);
	ret = sqlite3_bind_text(pInsert_stmt, 6, address, strlen(address) + 1, NULL);

	ret = sqlite3_bind_blob(pInsert_stmt, 7, userInfor->feature, userInfor->featureLen * sizeof(unsigned char), SQLITE_STATIC);

	ret = sqlite3_bind_int(pInsert_stmt, 8, userInfor->personType);
			
	//ret = sqlite3_bind_int(pInsert_stmt, 9, userInfor->personTimes);

	ret = sqlite3_step(pInsert_stmt);
	
	free(name);
	free(sex);
	free(phone);
	free(cardid);
	free(address);
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::CreateUserApp(UserApp &user_app)          
{
	int ret = sqlite3_prepare_v2(p_sql_handle_, SQL_USER_APP_RECODE, sizeof(SQL_USER_APP_RECODE), &p_insert_user_app_stmt_, 0);

	//ret = sqlite3_reset(p_insert_user_app_stmt_);

	//ret = sqlite3_bind_int(p_insert_user_app_stmt_, 1, user_app.app_id);

	char *app_name = UnicodeToUtf8(user_app.app_name);
	ret = sqlite3_bind_text(p_insert_user_app_stmt_, 1, app_name, strlen(app_name) + 1, NULL);

	char *creat_time = UnicodeToUtf8(user_app.create_time);
	ret = sqlite3_bind_text(p_insert_user_app_stmt_, 2, creat_time, strlen(creat_time) + 1, NULL);

	ret = sqlite3_bind_int(p_insert_user_app_stmt_, 3, user_app.threshold);

	char *remark = UnicodeToUtf8(user_app.remark);
	ret = sqlite3_bind_text(p_insert_user_app_stmt_, 4, remark, strlen(remark) + 1, NULL);

	ret = sqlite3_step(p_insert_user_app_stmt_);

	free(app_name);
	free(creat_time);
	free(remark);
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::CreatUserInfo(const UserInfo &user_info, int &user_id)
{
	int ret = sqlite3_prepare(p_sql_handle_, SQL_USER_INFO_RECODE, sizeof(SQL_USER_INFO_RECODE), &p_insert_user_info_stmt_, 0);

	//ret = sqlite3_reset(p_insert_user_info_stmt_);

	ret = sqlite3_bind_int(p_insert_user_info_stmt_, 1, user_info.app_id);

	char *name = UnicodeToUtf8(user_info.user_name);
	ret = sqlite3_bind_text(p_insert_user_info_stmt_, 2, name, strlen(name) + 1, NULL);

	char *sex = UnicodeToUtf8(user_info.user_sex);
	ret = sqlite3_bind_text(p_insert_user_info_stmt_, 3, sex, strlen(sex) + 1, NULL);

	ret = sqlite3_bind_int(p_insert_user_info_stmt_, 4, user_info.user_age);

	char *phone = UnicodeToUtf8(user_info.phone_num);
	ret = sqlite3_bind_text(p_insert_user_info_stmt_, 5, phone, strlen(phone) + 1, NULL);

	char *cardid = UnicodeToUtf8(user_info.user_card_id);
	ret = sqlite3_bind_text(p_insert_user_info_stmt_, 6, cardid, strlen(cardid) + 1, NULL);

	char *address = UnicodeToUtf8(user_info.user_address);
	ret = sqlite3_bind_text(p_insert_user_info_stmt_, 7, address, strlen(address) + 1, NULL);

	ret = sqlite3_bind_int(p_insert_user_info_stmt_, 8, user_info.user_type);

	char *creat_time = UnicodeToUtf8(user_info.create_time);
	ret = sqlite3_bind_text(p_insert_user_info_stmt_, 9, creat_time, strlen(creat_time) + 1, NULL);

	char *remark = UnicodeToUtf8(user_info.remark);
	ret = sqlite3_bind_text(p_insert_user_info_stmt_, 10, remark, strlen(remark) + 1, NULL);

	ret = sqlite3_step(p_insert_user_info_stmt_);

	free(name);
	free(sex);
	free(phone);
	free(cardid);
	free(address);
	free(creat_time);
	free(remark);

	char sql[] = "SELECT user_id FROM user_info";
	char **result;
	int cols;
	int rows;
	char *errMsg;
	ret = sqlite3_get_table(p_sql_handle_, sql, &result, &rows, &cols, &errMsg);
	if (rows == 0)
		return SQL_SUCCESS;

	user_id = CharToint(result[rows]);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::AddUserFeature(UserFeature &user_feature)		
{
	int ret = sqlite3_prepare(p_sql_handle_, SQL_USER_FEATURE_RECODE, sizeof(SQL_USER_FEATURE_RECODE), &p_insert_user_feature_stmt_, 0);

	ret = sqlite3_reset(p_insert_user_feature_stmt_);

	ret = sqlite3_bind_int(p_insert_user_feature_stmt_, 1, user_feature.user_id);

	ret = sqlite3_bind_blob(p_insert_user_feature_stmt_, 2, user_feature.feature, user_feature.feature_len * sizeof(unsigned char), SQLITE_STATIC);

	char *location = UnicodeToUtf8(user_feature.location);
	ret = sqlite3_bind_text(p_insert_user_feature_stmt_, 3, location, strlen(location) + 1, NULL);

	char *url = UnicodeToUtf8(user_feature.url);
	ret = sqlite3_bind_text(p_insert_user_feature_stmt_, 4, url, strlen(url) + 1, NULL);

	ret = sqlite3_step(p_insert_user_feature_stmt_);

	free(location);
	free(url);
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::AddUserAccount(UserAccount &user_account)
{
	int ret = sqlite3_prepare(p_sql_handle_, SQL_USER_ACCOUNT_RECODE, sizeof(SQL_USER_ACCOUNT_RECODE), &p_insert_user_account_stmt_, 0);

	//ret = sqlite3_reset(p_insert_user_account_stmt_);

	char *name = UnicodeToUtf8(user_account.user_name);
	ret = sqlite3_bind_text(p_insert_user_account_stmt_, 1, name, strlen(name) + 1, NULL);

	char *account = UnicodeToUtf8(user_account.user_account);
	ret = sqlite3_bind_text(p_insert_user_account_stmt_, 2, account, strlen(account) + 1, NULL);

	char *password = UnicodeToUtf8(user_account.user_password);
	ret = sqlite3_bind_text(p_insert_user_account_stmt_, 3, password, strlen(password) + 1, NULL);

	char *email = UnicodeToUtf8(user_account.user_email);
	ret = sqlite3_bind_text(p_insert_user_account_stmt_, 4, email, strlen(email) + 1, NULL);

	char *serial_num = UnicodeToUtf8(user_account.serial_num);
	ret = sqlite3_bind_text(p_insert_user_account_stmt_, 5, serial_num, strlen(serial_num) + 1, NULL);

	ret = sqlite3_bind_int(p_insert_user_account_stmt_, 6, user_account.account_type);

	ret = sqlite3_step(p_insert_user_account_stmt_);

	free(name);
	free(account);
	free(password);
	free(email);
	free(serial_num);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::UpdateFeature2Memery(FeatureOperation operation)
{
	int ret = sqlite3_prepare_v2(p_sql_handle_, SQL_QUERY_USER_ID_FEATURE, sizeof(SQL_QUERY_USER_ID_FEATURE), &pQuery_stmt, 0);

	while (SQLITE_ROW == sqlite3_step(pQuery_stmt))
	{
		/*int id = sqlite3_column_int(pQuery_stmt, 0);
		unsigned char *feature_data = (unsigned char *)sqlite3_column_blob(pQuery_stmt, 1);
		int len = sqlite3_column_bytes(pQuery_stmt, 1);

		memeryFeature[id].feature = (unsigned char *)malloc(len);
		memeryFeature[id].featureLen = len;
		memcpy(memeryFeature[id].feature, feature_data, len);*/

		int id = sqlite3_column_int(pQuery_stmt, 0);
		unsigned char *feature_data = (unsigned char *)sqlite3_column_blob(pQuery_stmt, 1);
		int len = sqlite3_column_bytes(pQuery_stmt, 1);

		memery_feature_[id].feature = (unsigned char *)malloc(len);
		memery_feature_[id].featureLen = len;
		memcpy(memery_feature_[id].feature, feature_data, len);
	}
	sqlite3_finalize(pQuery_stmt);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::GetFaceFeatureToMemery(void)
{
	int ret = sqlite3_prepare_v2(p_sql_handle_, SQL_QUERY_USER_ID_FEATURE, sizeof(SQL_QUERY_USER_ID_FEATURE), &p_query_face_feature_stmt_, 0);

	while (SQLITE_ROW == sqlite3_step(p_query_face_feature_stmt_))
	{
		FacesFeatureUrl face_feature_url;

		face_feature_url.user_id = sqlite3_column_int(p_query_face_feature_stmt_, 0);

		unsigned char *feature_data = (unsigned char *)sqlite3_column_blob(p_query_face_feature_stmt_, 1);
		int len = sqlite3_column_bytes(p_query_face_feature_stmt_, 1);

		face_feature_url.feature = (unsigned char *)malloc(len);
		memcpy(face_feature_url.feature, feature_data, len);
		face_feature_url.feature_size = len;

		unsigned char *url = (unsigned char *)sqlite3_column_text(p_query_face_feature_stmt_, 2);
		strcpy(face_feature_url.url, Utf8ToUnicode((char *)url));

		memery_user_feature_.push_back(face_feature_url);
		//free(face_feature_url.face_feature.feature);
	}
	sqlite3_finalize(p_query_face_feature_stmt_);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::GetUserAppToMemery(void)
{
	char sql[100] = { 0 };
	sprintf(sql, "SELECT app_id,app_name,create_time,threshold,remark FROM [user_app];");

	char **result;
	int cols;
	int rows;
	char *errMsg;
	int ret = sqlite3_get_table(p_sql_handle_, sql, &result, &rows, &cols, &errMsg);
	if (ret != SQLITE_OK)
	{
		printf("Query database failed:%s", errMsg);
	}
	if (rows == 0)
		return SQL_SUCCESS;


	for (int i = 0; i < rows; ++i)
	{
		UserApp user_app;
		int index = (i + 1) * cols ;

		user_app.app_id = CharToint(result[index]);

		index++;
		char *pname = Utf8ToUnicode(result[index]);
		strcpy(user_app.app_name, pname);

		index++;
		char *time = Utf8ToUnicode(result[index]);
		strcpy(user_app.create_time, time);

		index++;
		user_app.threshold = CharToint(result[index]);

		index++;
		char *remark = Utf8ToUnicode(result[index]);
		strcpy(user_app.remark, remark);

		memery_user_app_.push_back(user_app);

		free(pname);
		free(time);
		free(remark);
	}

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::UpdateMemeryUserApp(void)
{
	this->memery_user_app_.clear();
	this->GetUserAppToMemery();
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::GetUserMainInfoToMemery(void)
{
	const char sql_query_user_info[] = "SELECT max(user_info.app_id) as app_id, user_info.name, user_info.sex,     "
		"user_info.age, user_info.phone, user_info.card_id, user_info.address, user_info.type, user_info.remark,   "
		"user_info.user_id, user_feature.url, user_app.app_name FROM [user_info] inner join [user_feature] inner   "
		"join [user_app] on user_info.user_id = user_feature.user_id  and user_app.app_id = user_info.app_id group "
		" by user_info.user_id;";

	int ret = sqlite3_prepare_v2(p_sql_handle_, sql_query_user_info, sizeof(sql_query_user_info), &p_query_user_info_stmt_, 0);

	while (SQLITE_ROW == sqlite3_step(p_query_user_info_stmt_))
	{
		UserMainInfo user_main_info;

		user_main_info.user_info.app_id = sqlite3_column_int(p_query_user_info_stmt_, 0);

		unsigned char *name = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 1);
		strcpy(user_main_info.user_info.user_name, Utf8ToUnicode((char *)name));

		unsigned char *sex = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 2);
		strcpy(user_main_info.user_info.user_sex, Utf8ToUnicode((char *)sex));

		user_main_info.user_info.user_age = sqlite3_column_int(p_query_user_info_stmt_, 3);

		unsigned char *phone = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 4);
		strcpy(user_main_info.user_info.phone_num, Utf8ToUnicode((char *)phone));

		unsigned char *card = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 5);
		strcpy(user_main_info.user_info.user_card_id, Utf8ToUnicode((char *)card));

		unsigned char *address = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 6);
		strcpy(user_main_info.user_info.user_address, Utf8ToUnicode((char *)address));

		user_main_info.user_info.user_type = sqlite3_column_int(p_query_user_info_stmt_, 7);

		unsigned char *remark = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 8);
		strcpy(user_main_info.user_info.remark, Utf8ToUnicode((char *)remark));

		user_main_info.user_info.user_id = sqlite3_column_int(p_query_user_info_stmt_, 9);

		unsigned char *url = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 10);
		strcpy(user_main_info.url, Utf8ToUnicode((char *)url));

		unsigned char *app_name = (unsigned char *)sqlite3_column_text(p_query_user_info_stmt_, 11);
		strcpy(user_main_info.app_name, Utf8ToUnicode((char *)app_name));

		memery_user_main_info_.push_back(user_main_info);

	}
	sqlite3_finalize(p_query_user_info_stmt_);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::UpdateMemeryUserMainInfo(void)
{
	this->memery_user_main_info_.clear();
	this->GetUserMainInfoToMemery();
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::DeleteMemeryUserMainInfo(int user_id)
{
	std::vector<UserMainInfo>::iterator it = memery_user_main_info_.begin();

	while (it != memery_user_main_info_.end())
	{
		if (user_id == it->user_info.user_id)
		{
			memery_user_main_info_.erase(it);
			break;
		}
		it++;
	}

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::QueryUserInfoByid(int userid, UserInfo &userInfor)
{
	char sql[128] = { 0 };
	sprintf(sql, "SElECT * FROM [user_info] WHERE id = %d;", userid);

	char **result;
	int cols;
	int rows;
	char *errMsg;
	int ret = sqlite3_get_table(pUserHandle, sql, &result, &rows, &cols, &errMsg);
	if (ret != SQLITE_OK)
	{
		printf("Query database failed:%s", errMsg);
	}
	if (rows > 0)
	{
		int index = cols + 1;

		userInfor.app_id = CharToint(result[index]);

		index++;
		userInfor.user_id = CharToint(result[index]);

		index++;
		char *pname = Utf8ToUnicode(result[index]);
		strcpy(userInfor.user_name, pname);

		index++;
		char *sex = Utf8ToUnicode(result[index]);
		strcpy(userInfor.user_sex, sex);

		index++;
		userInfor.user_age = CharToint(result[index]);

		index++;
		char *phone = Utf8ToUnicode(result[index]);
		strcpy(userInfor.phone_num, phone);

		index++;
		char *card_id = Utf8ToUnicode(result[index]);
		strcpy(userInfor.user_card_id, card_id);

		index++;
		char *address = Utf8ToUnicode(result[index]);
		strcpy(userInfor.user_address, address);

		index++;
		userInfor.user_type = CharToint(result[index]);

		index++;
		char *creat_time = Utf8ToUnicode(result[index]);
		strcpy(userInfor.create_time, creat_time);

		index++;
		char *remark = Utf8ToUnicode(result[index]);
		strcpy(userInfor.remark, remark);

		free(pname);
		free(sex);
		free(phone);
		free(card_id);
		free(address);
		free(creat_time);
		free(remark);
	}
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::QueryUserAccountInfo(UserAccount &user_account, int &result)
{
	int ret = sqlite3_prepare_v2(p_sql_handle_, SQL_QUERY_USER_ACCOUNT, sizeof(SQL_QUERY_USER_ACCOUNT), &p_query_user_account_stmt_, 0);

	result = 0;
	while (SQLITE_ROW == sqlite3_step(p_query_user_account_stmt_))
	{
		unsigned char *account = (unsigned char *)sqlite3_column_text(p_query_user_account_stmt_, 0);
		int account_len = sqlite3_column_bytes(p_query_user_account_stmt_, 0);
		int i = 0;
		for (i = 0; i < account_len; ++i)
		{
			if (user_account.user_account[i] != account[i])
				break;
		}
		if (i == account_len)
		{
			if (account_len == strlen(user_account.user_account) + 1)
			{
				result = 1;
			}
			if (result == 1)
				break;
		}


		unsigned char *email = (unsigned char *)sqlite3_column_text(p_query_user_account_stmt_, 1);
		int email_len = sqlite3_column_bytes(p_query_user_account_stmt_, 1);
		for (i = 0; i < email_len; ++i)
		{
			if (user_account.user_email[i] != email[i])
				break;
		}
		if (i == email_len)
		{
			if (email_len == strlen(user_account.user_email) + 1)
			{
				result = 2;
			}
			if (result == 2)
				break;
		}

	}
	sqlite3_finalize(p_query_user_account_stmt_);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::VerifyUserAccount(UserAccount *user_account, int &result)
{
	int ret = sqlite3_prepare_v2(p_sql_handle_, SQL_VERIFY_USER_ACCOUNT, sizeof(SQL_VERIFY_USER_ACCOUNT), &p_verify_user_account_stmt_, 0);

	int all_account = 0;
	int account_recode = 0;
	result = 0;
	/*result返回值位定义
	 *个位：1、账户存在  2、账户不存在
	 *十位：1、密码正确  2、密码错误
	 *百位：账户属性
	*/
	while (SQLITE_ROW == sqlite3_step(p_verify_user_account_stmt_))
	{
		all_account++;
		unsigned char *account = (unsigned char *)sqlite3_column_text(p_verify_user_account_stmt_, 1);
		int account_len = sqlite3_column_bytes(p_verify_user_account_stmt_, 1);
		bool account_is_exist = CompareChar(account, account_len, (unsigned char*)user_account->user_account, strlen(user_account->user_account) + 1);
		if (!account_is_exist)   //帐号不匹配
		{
			account_recode++;
		}
		else                      //帐号匹配，验证密码
		{
			unsigned char *password = (unsigned char *)sqlite3_column_text(p_verify_user_account_stmt_, 2);
			int password_len = sqlite3_column_bytes(p_verify_user_account_stmt_, 2);
			bool password_is_right = CompareChar(password, password_len, (unsigned char*)user_account->user_password, strlen(user_account->user_password) + 1);
			if (password_is_right)
				result += 10;
			else
				result += 20;

			int account_type = sqlite3_column_int(p_verify_user_account_stmt_, 3);
			result += (account_type * 100);

			unsigned char *user_name = (unsigned char *)sqlite3_column_text(p_verify_user_account_stmt_, 0);
			int name_len = sqlite3_column_bytes(p_verify_user_account_stmt_, 0);
			strcpy(user_account->user_name, (char *)user_name);
		}
	}
	sqlite3_finalize(p_verify_user_account_stmt_);

	if (all_account == account_recode) //账户不存在
		result += 2;
	else
		result += 1;

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::QueryUserAppByid(int app_id, UserApp &user_app)
{
	char sql[100] = { 0 };
	sprintf(sql, "SELECT app_id,app_name,create_time,threshold,remark FROM [user_app] WHERE app_id = %d;", app_id);

	char **result;
	int cols;
	int rows;
	char *errMsg;
	int ret = sqlite3_get_table(p_sql_handle_, sql, &result, &rows, &cols, &errMsg);
	if (ret != SQLITE_OK)
	{
		printf("Query database failed:%s", errMsg);
	}
	if (rows > 0)
	{
		int index = cols + 1;

		user_app.app_id = CharToint(result[index]);

		index++;
		char *pname = Utf8ToUnicode(result[index]);
		strcpy(user_app.app_name, pname);

		index++;
		char *time = Utf8ToUnicode(result[index]);
		strcpy(user_app.create_time, time);

		index++;
		user_app.threshold = CharToint(result[index]);

		index++;
		char *remark = Utf8ToUnicode(result[index]);
		strcpy(user_app.remark, remark);

		free(pname);
		free(time);
		free(remark);
	}

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::ModifyUserAppByid(int app_id, UserApp &user_app)
{
	char sql[256] = { 0 };
	char *app_name = UnicodeToUtf8(user_app.app_name);
	char *remark = UnicodeToUtf8(user_app.remark);
	sprintf(sql, "UPDATE [user_app] SET app_name='%s',threshold='%d',remark='%s' WHERE app_id = %d;",
		app_name, user_app.threshold, remark, app_id);

	char *errMsg;
	int ret = sqlite3_exec(p_sql_handle_, sql, NULL, NULL, &errMsg);
	if (ret != SQLITE_OK)
	{
		printf("Modify database failed:%s", errMsg);
	}
	
	free(app_name);
	free(remark);
	return SQL_SUCCESS;
}

int SQLITE_OPERATION::DeleteUserAppByid(int app_id)
{
	char sql[100] = { 0 };
	sprintf(sql, "DELETE FROM [user_app] WHERE app_id = %d;", app_id);

	char *errMsg;
	int ret = sqlite3_exec(p_sql_handle_, sql, NULL, NULL, &errMsg);
	if (ret != SQLITE_OK)
	{
		printf("Delete database failed:%s", errMsg);
	}

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::ModifyUserInfoByid(const UserInfo &user_info)
{
	char sql[]="UPDATE [user_info] SET app_id=?,name=?,sex=?,age=?,phone=?,card_id=?,address=?,"
		"type=?,create_time=?,remark=? WHERE user_id=? ;";

	int ret = sqlite3_prepare(p_sql_handle_, sql, sizeof(sql), &p_update_user_info_stmt_, 0);

	ret = sqlite3_bind_int(p_update_user_info_stmt_, 1, user_info.app_id);

	char *name = UnicodeToUtf8(user_info.user_name);
	ret = sqlite3_bind_text(p_update_user_info_stmt_, 2, name, strlen(name) + 1, NULL);

	char *sex = UnicodeToUtf8(user_info.user_sex);
	ret = sqlite3_bind_text(p_update_user_info_stmt_, 3, sex, strlen(sex) + 1, NULL);

	ret = sqlite3_bind_int(p_update_user_info_stmt_, 4, user_info.user_age);

	char *phone = UnicodeToUtf8(user_info.phone_num);
	ret = sqlite3_bind_text(p_update_user_info_stmt_, 5, phone, strlen(phone) + 1, NULL);

	char *cardid = UnicodeToUtf8(user_info.user_card_id);
	ret = sqlite3_bind_text(p_update_user_info_stmt_, 6, cardid, strlen(cardid) + 1, NULL);

	char *address = UnicodeToUtf8(user_info.user_address);
	ret = sqlite3_bind_text(p_update_user_info_stmt_, 7, address, strlen(address) + 1, NULL);

	ret = sqlite3_bind_int(p_update_user_info_stmt_, 8, user_info.user_type);

	char *creat_time = UnicodeToUtf8(user_info.create_time);
	ret = sqlite3_bind_text(p_update_user_info_stmt_, 9, creat_time, strlen(creat_time) + 1, NULL);

	char *remark = UnicodeToUtf8(user_info.remark);
	ret = sqlite3_bind_text(p_update_user_info_stmt_, 10, remark, strlen(remark) + 1, NULL);

	ret = sqlite3_bind_int(p_update_user_info_stmt_, 11, user_info.user_id);

	ret = sqlite3_step(p_update_user_info_stmt_);

	free(name);
	free(sex);
	free(phone);
	free(cardid);
	free(address);
	free(creat_time);
	free(remark);

	return SQL_SUCCESS;
}

int SQLITE_OPERATION::DeleteUserInfoByid(const int user_id)
{
	char sql[100] = { 0 };
	sprintf(sql, "DELETE FROM [user_info] WHERE user_id = %d;", user_id);

	char *errMsg;
	int ret = sqlite3_exec(p_sql_handle_, sql, NULL, NULL, &errMsg);
	if (ret != SQLITE_OK)
	{
		printf("Delete user_info failed:%s", errMsg);
	}

	return SQL_SUCCESS;
}


int SQLITE_OPERATION::OutputResult(int userid, PERSON_INFORMATIONS &userInfor)
{
	char sql[128] = { 0 };
	sprintf(sql, "SElECT * FROM [user_information] WHERE id = %d;", userid);

	char **result;
	int cols;
	int rows;
	char *errMsg;
	int ret = sqlite3_get_table(pUserHandle, sql, &result, &rows, &cols, &errMsg);
	if (ret != SQLITE_OK)
	{
		printf("Query database failed:%s", errMsg);
	}
	if (rows > 0)
	{
		int index = cols + 1;

		char *pname = Utf8ToUnicode(result[index]);
		strcpy(userInfor.personName, pname);

		index++;
		char *sex = Utf8ToUnicode(result[index]);
		strcpy(userInfor.personSex, sex);

		index++;
		userInfor.personAge = CharToint(result[index]);

		index++;
		char *phone = Utf8ToUnicode(result[index]);
		strcpy(userInfor.phoneNum, phone);

		index++;
		char *card_id = Utf8ToUnicode(result[index]);
		strcpy(userInfor.personCardId, card_id);

		index++;
		char *address = Utf8ToUnicode(result[index]);
		strcpy(userInfor.personAddress, address);

		free(pname);
		free(sex);
		free(phone);
		free(card_id);
		free(address);
	}

	return SQL_SUCCESS;
}

SQLITE_OPERATION::~SQLITE_OPERATION()
{
	sqlite3_close(pUserHandle);
	std::map<int, PERSON_FEATURE >::iterator it = memeryFeature.begin();
	while (it != memeryFeature.end())
	{
		free(it->second.feature);
		++it;
	}	
}





