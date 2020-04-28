#pragma once

#include <QWidget>
#include "ui_MainInterface.h"
#include "sqlite_operation.h"
#include "face_operation.h"
#include <QMessageBox>
#include "DisplayDetectResult.h"
#include "FaceAppLibrary.h"
#include "FaceUserLibrary.h"
#include "UserInfoIcon.h"
#include <map>
#include <vector>


class MainInterface : public QWidget
{
	Q_OBJECT

public:
	//MainInterface(QWidget *parent = Q_NULLPTR);
	MainInterface(SqlDatabaseClass *sql_handle, UserAccount *account_info, QWidget *parent = Q_NULLPTR);
	~MainInterface();

	void paintEvent(QPaintEvent *event);   //��ͼ
	void DisplayListWidget();              //��ʾ�û����������
	void ClearListWidget();                //����û����������
	void RefreshListWidget();              //ˢ���û����������


										   //��ʾ�û���Ϣ���е�����
	void DisplayUserListWidget(UserInfo &user_info, QString image_path); 									  
	void ClearUserListWidget();            //����û���Ϣ���е�����
	void RefreshUserListWidget();          //ˢ���û���Ϣ���е�����
	 
	void RecodeUserAppMap();         //ά���û�������map
	void DisplayCurrentAppUserInfo(int app_id);  //��ʾapp_id�е��û���Ϣ

	void ModifyUserInfo(int user_id);     //�޸��û���Ϣ
	void AddNewUser();                    //������û�
	void AddFaceToUser(int user_id);      //����û�����
	void DeleteUserInfo(int user_id);     //ɾ���û���Ϣ
	void DeleteMultiUserInfo(void);       //����ɾ���û���Ϣ

signals:
	void BackLogin();

private:
	Ui::MainInterface ui;

	SqlDatabaseClass *sqlite_operation_ = NULL;
	UserAccount      *current_account_info_ = NULL;

	QString compare_image1_path_;
	QString compare_image2_path_;
	QString detect_image_path_;
	QString recognize_image_path_;

	FaceEngineClass *face_opreation_ = NULL;

	cv::Mat face_detect_result_image_;

	//�Ӵ��ڳ�Ա����
	DisplayDetectResult *display_detect_result_ = NULL;
	FaceAppLibrary      *face_app_library_ = NULL;
	FaceUserLibrary     *face_user_library_ = NULL;

	QMessageBox msg_box_;

	int current_app_id_ = 0;
	int current_user_id_ = 0;
	std::map<int, UserApp> map_user_app_recode_;   //
	std::map<float, int>   map_recognize_result_;
	std::vector<int>       vector_delete_select_user;

};

