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

	void paintEvent(QPaintEvent *event);   //绘图
	void DisplayListWidget();              //显示用户组框中数据
	void ClearListWidget();                //清空用户组框中数据
	void RefreshListWidget();              //刷新用户组框中数据


										   //显示用户信息框中的数据
	void DisplayUserListWidget(UserInfo &user_info, QString image_path); 									  
	void ClearUserListWidget();            //清除用户信息框中的数据
	void RefreshUserListWidget();          //刷新用户信息框中的数据
	 
	void RecodeUserAppMap();         //维护用户组数据map
	void DisplayCurrentAppUserInfo(int app_id);  //显示app_id中的用户信息

	void ModifyUserInfo(int user_id);     //修改用户信息
	void AddNewUser();                    //添加新用户
	void AddFaceToUser(int user_id);      //添加用户人脸
	void DeleteUserInfo(int user_id);     //删除用户信息
	void DeleteMultiUserInfo(void);       //批量删除用户信息

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

	//子窗口成员变量
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

