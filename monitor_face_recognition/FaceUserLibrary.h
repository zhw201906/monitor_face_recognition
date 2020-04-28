#pragma once

#include <QWidget>
#include "ui_FaceUserLibrary.h"
#include <QMessageBox>
#include "sqlite_operation.h"
#include "face_operation.h"

#define  OPEN_IMAGE_PATH  "D:\\test_image"


enum operation_type{ADD_USER,MODIFY_USER,ADD_USER_FACE};

class FaceUserLibrary : public QWidget
{
	Q_OBJECT

public:
	FaceUserLibrary(int operation,const UserInfo &user_info, QWidget *parent = Q_NULLPTR);
	~FaceUserLibrary();

	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);

signals:
	void BackMainInterface();  //退出信号
	void AddUserInfo(UserInfo &user_info, QString image_path);  //添加用户确认按钮信号
	void ModifyUser(UserInfo &user_info);   //修改用户确认按钮
	void AddUserFace(QString image_path);


private:
	Ui::FaceUserLibrary ui;

	int current_operation_;
	QString image_path_;
	UserInfo user_info_pro_;

	QMessageBox msg_box_;

public:
	int click_count_ = 0;

};
