#ifndef _SQLITE_OPERATION_H
#define _SQLITE_OPERATION_H

#include <stdio.h>
#include <iostream>
#include <map>
#include "sqlite3.h"
#include <vector>

#pragma  warning(disable:4996)
#pragma  comment(lib,"sqlite3.lib")

#define  ERR_             0x100
#define  SQL_SUCCESS      (ERR_+0x01)
#define  SQL_FAILED       (ERR_+0x02)

#define  MEMERY_FEATURE_NUM   (1032*10000)

const char SQL_CREAT_USER_APP_TABLE[] = "CREATE TABLE IF NOT EXISTS [user_app]("
	"[app_id] INTEGER PRIMARY KEY,"     //������ID��ϵͳ�Զ����䣬Ψһ
	"[app_name] VARCHAR(32),"           //����������
	"[create_time] VARCHAR(32),"        //�����鴴��ʱ��
	"[threshold] INTEGER,"              //ʶ����ֵ
	"[remark] VARCHAR(255)"             //��ע��Ϣ	
	");";

const char SQL_CREAT_USER_INFO_TABLE[] = "CREATE TABLE IF NOT EXISTS [user_info] ("
	"[app_id] INTEGER,"               //������ID�����������
	"[user_id] INTEGER PRIMARY KEY,"  //�û�ID��ϵͳ�Զ����䣬Ψһ
	"[name] VARCHAR(32),"             //�û�����
	"[sex] VARCHAR(20),"              //�û��Ա�
	"[age] INTEGER,"                  //�û�����
	"[phone] VARCHAR(32),"            //�û��绰
	"[card_id] VARCHAR(32),"          //�û����֤��
	"[address] VARCHAR(64),"          //�û�סַ
	"[type] INTEGER,"                 //�û����ͣ�0 ��ͨ�û���1 �������û���2 �������û���3 ץ���û�
	"[create_time] VARCHAR(32),"      //�û�����ʱ��
	"[remark] VARCHAR(255),"          //��ע��Ϣ
	"FOREIGN KEY (app_id) REFERENCES user_app(app_id) ON DELETE CASCADE ON UPDATE CASCADE"
	"); ";

const char SQL_CREATE_USER_FEATURE_TABLE[] = "CREATE TABLE IF NOT EXISTS [user_feature]("
	"[user_id] INTEGER,"              //�û�ID�����������
	"[feature] BLOB,"                 //�û���������
	"[location] VARCHAR(64),"         //�������꣬��ͼƬ�е�λ��
	"[url] VARCHAR(255),"              //ץ����Ա��Ϣ��¼�ļ�·��
	"FOREIGN KEY (user_id) REFERENCES user_info(user_id) ON DELETE CASCADE ON UPDATE CASCADE"
	");";
/*�û��˻��洢�����ݿ�
 *�û��ǳ������ظ�
 *�˺���ҪΨһ�����ڵ�¼��Ӧ�˻�
 *������������
 *����Ψһ�������һ�����
 *���кţ��������ṩ
 *�û������������ֹ���
*/
const char SQL_CREATE_USER_ACCOUNT_TABLE[] = "CREATE TABLE IF NOT EXISTS [user_account]("
	"[user_name] VARCHAR(32) NOT NULL,"              //�˻��ǳ�
	"[user_account] VARCHAR(32) NOT NULL,"           //�˻��ʺ�
	"[user_password] VARCHAR(32) NOT NULL,"          //��¼����
	"[user_email] VARCHAR(32) NOT NULL,"             //���䣨�����һ����룩
	"[serial_num] VARCHAR(32),"             //���кţ��ɿ������ṩ������ע�����Ա
	"[account_type] INTEGER NOT NULL"               //�˻����ԣ�1����ͨ�û�  2������Ա
	");";

const char SQL_USER_APP_RECODE[] = "INSERT INTO [user_app] (app_name,create_time,threshold,remark) VALUES (?,?,?,?);";
const char SQL_USER_INFO_RECODE[] = "INSERT INTO [user_info] (app_id,name,sex,age,phone,card_id,address,"
									"type,create_time,remark) VALUES (?,?,?,?,?,?,?,?,?,?);";
const char SQL_USER_FEATURE_RECODE[] = "INSERT INTO [user_feature] (user_id,feature,location,url) VALUES (?,?,?,?);";
const char SQL_USER_ACCOUNT_RECODE[] = "INSERT INTO [user_account] (user_name,user_account,user_password,user_email,serial_num,account_type) VALUES (?,?,?,?,?,?);";

const char SQL_QUERY_USER_ID_FEATURE[]  = "SELECT user_id, feature, url FROM [user_feature];";
const char SQL_QUERY_USER_ACCOUNT[] = "SELECT user_account,user_email FROM [user_account];";
const char SQL_VERIFY_USER_ACCOUNT[] = "SELECT user_name,user_account,user_password,account_type FROM [user_account];";

typedef struct PERSON_INFO
{
	int    appId;
	int    personId;
	char   personName[32];
	char   personSex[32];
	int    personAge;
	char   personCardId[32];
	char   phoneNum[32];
	char   personAddress[32];
	int    personType;
	char   createTime[32];
	char   remark[256];
	unsigned char *feature;
	int    featureLen;
	PERSON_INFO()
	{
		appId = 0;
		personId = 0;
		memset(personName, ' ', sizeof(personName));
		memset(personSex, ' ', sizeof(personSex));
		personAge = 25;
		memset(personCardId, ' ', sizeof(personCardId));
		memset(phoneNum, ' ', sizeof(phoneNum));
		memset(personAddress, ' ', sizeof(personAddress));
		personType = 0;
		memset(createTime, ' ', sizeof(createTime));
		memset(remark, ' ', sizeof(remark));
		//personTimes = 0;
		featureLen = 0;
	}
}PERSON_INFORMATIONS;

typedef struct UserInfo_
{
	int    app_id;
	int    user_id;
	char   user_name[32];
	char   user_sex[32];
	int    user_age;
	char   user_card_id[32];
	char   phone_num[32];
	char   user_address[32];
	int    user_type;
	char   create_time[32];
	char   remark[256];
	//unsigned char *feature;
	//int    featureLen;
	UserInfo_()
	{
		app_id = 0;
		user_id = 0;
		memset(user_name, ' ', sizeof(user_name));
		memset(user_sex, ' ', sizeof(user_sex));
		user_age = 18;
		memset(user_card_id, ' ', sizeof(user_card_id));
		memset(phone_num, ' ', sizeof(phone_num));
		memset(user_address, ' ', sizeof(user_address));
		user_type = 0;
		memset(create_time, ' ', sizeof(create_time));
		memset(remark, ' ', sizeof(remark));
		//personTimes = 0;
		//featureLen = 0;
	}
}UserInfo;

typedef struct UserApp_
{
	int    app_id;
	char   app_name[32];
	char   create_time[32];
	int    threshold;
	char   remark[256];
	UserApp_()
	{
		app_id = 0;
		memset(app_name, ' ', sizeof(app_name));
		memset(create_time, ' ', sizeof(create_time));
		memset(remark, ' ', sizeof(remark));
		threshold = 60;
	}
}UserApp;

typedef struct UserFeature_
{
	int    user_id;
	unsigned char   *feature;
	int    feature_len;
	char   location[256];
	char   url[256];
	UserFeature_()
	{
		user_id = 0;
		feature_len = 0;
		memset(location, ' ', sizeof(location));
		memset(url, ' ', sizeof(url));
	}
}UserFeature;

typedef struct UserAccount_
{
	char user_name[32];
	char user_account[32];
	char user_password[32];
	char user_email[32];
	char serial_num[32];
	int  account_type;
	UserAccount_()
	{
		memset(user_name, ' ', sizeof(user_name));
		memset(user_account, ' ', sizeof(user_account));
		memset(user_password, ' ', sizeof(user_password));
		memset(user_email, ' ', sizeof(user_email));
		memset(serial_num, ' ', sizeof(serial_num));
		account_type = 0;
	}

}UserAccount;

typedef struct UserMainInfo_
{
	char       app_name[32];
	UserInfo   user_info;
	char       url[256];
	int        face_num;
	UserMainInfo_()
	{
		memset(app_name, ' ', sizeof(app_name));
		memset(url, ' ', sizeof(url));
		face_num = 0;
	}

}UserMainInfo;

typedef struct
{
	unsigned char *feature;
	int featureLen;
}PERSON_FEATURE;

typedef struct UserFaceFeature_
{
	unsigned char *feature;
	int featureLen;
}UserFaceFeature;

typedef struct FacesFeature_
{
	int             user_id;
	char            url[256];
	unsigned char  *feature;
	int             feature_size;
	//UserFaceFeature face_feature;
	FacesFeature_()
	{
		//memset(feature, ' ', sizeof(feature));
		memset(url, ' ', sizeof(url));
		user_id = 0;
		feature_size = 0;
	}
}FacesFeatureUrl;

typedef sqlite3  SqlUserHandle;

#define  SqlDataBaseName  "userinformation.IDX"

enum FeatureOperation { ADD_FEATURE, DEL_FEATURE };
class SQLITE_OPERATION
{
public:
	int SqliteInit(void);

	int MemeryInit(void);
	int GetFeature2Memery(void);
	int UpdateFeature2Memery(FeatureOperation operation = ADD_FEATURE);

	int GetFaceFeatureToMemery(void);  //��ȡȫ�������������ڴ���

	int GetUserAppToMemery(void);    //��ȡuser_app��Ϣ���ڴ���
	int UpdateMemeryUserApp(void);   //�����ڴ���user_app��Ϣ
	//int QueryUserAppByAppid(int app_id,)

	int GetUserMainInfoToMemery(void);          //��ȡ�û���Ҫ��Ϣ���ڴ���
	int UpdateMemeryUserMainInfo(void);         //�����ڴ����û���Ҫ��Ϣ
	int DeleteMemeryUserMainInfo(int user_id);  //ɾ���ڴ���user_id��Ϣ
	
	//¼���û���ز���
	int CreateUserApp(UserApp &user_app);          //�������û���
	int CreatUserInfo(const UserInfo &user_info, int &user_id);        //�������û�,���õ����û�id
	int AddUserFeature(UserFeature &user_feature); //����û�����
	int AddUserAccount(UserAccount &user_account);

	int CreatUser(PERSON_INFORMATIONS *userInfor);

	int QueryUserInfoByid(int userid, UserInfo &userInfor);
	int QueryUserAccountInfo(UserAccount &user_account,int &result);
	int VerifyUserAccount(UserAccount *user_account, int &result);

	int QueryUserAppByid(int app_id, UserApp &user_app);  //ͨ���û���id,��ѯ�û�����Ϣ
	int ModifyUserAppByid(int app_id, UserApp &user_app); //ͨ���û���id,�޸��û�����Ϣ
	int DeleteUserAppByid(int app_id);                    //ͨ���û���id,ɾ���û�����Ϣ,�����û���Ϣ���û���������

	int ModifyUserInfoByid(const UserInfo &userInfor);  //ͨ���û�id���޸��û���Ϣ
	int DeleteUserInfoByid(const int user_id);          //ͨ��user_id��ɾ���û���Ϣ�������û���������


	int OutputResult(int userid, PERSON_INFORMATIONS &userInfor);
	SqlUserHandle *getSqlHandle() { return pUserHandle; };

	~SQLITE_OPERATION();


private:
	SqlUserHandle *pUserHandle;
	sqlite3_stmt  *pInsert_stmt;
	sqlite3_stmt  *pQuery_stmt;

	SqlUserHandle *p_sql_handle_;
	sqlite3_stmt  *p_insert_user_app_stmt_;
	sqlite3_stmt  *p_insert_user_info_stmt_;
	sqlite3_stmt  *p_update_user_info_stmt_;
	sqlite3_stmt  *p_insert_user_feature_stmt_;
	sqlite3_stmt  *p_insert_user_account_stmt_;

	sqlite3_stmt  *p_query_user_feature_stmt_;
	sqlite3_stmt  *p_query_user_account_stmt_;
	sqlite3_stmt  *p_verify_user_account_stmt_;
	sqlite3_stmt  *p_query_user_app_stmt_;
	sqlite3_stmt  *p_query_user_info_stmt_;
	sqlite3_stmt  *p_query_face_feature_stmt_;

public:
	std::map<int, PERSON_FEATURE> memeryFeature;
	std::map<int, UserFaceFeature> memery_feature_;

	std::vector<UserApp>         memery_user_app_;         //�洢user_app��Ϣ
	std::vector<UserMainInfo>    memery_user_main_info_;   //�洢user_main_info,��������ʶ��Ϳ����
	std::vector<FacesFeatureUrl> memery_user_feature_;     //�洢��������ȫ��������������������ʶ��
};

typedef   SQLITE_OPERATION    SqlDatabaseClass;


#endif

