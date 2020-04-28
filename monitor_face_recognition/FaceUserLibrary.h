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
	void BackMainInterface();  //�˳��ź�
	void AddUserInfo(UserInfo &user_info, QString image_path);  //����û�ȷ�ϰ�ť�ź�
	void ModifyUser(UserInfo &user_info);   //�޸��û�ȷ�ϰ�ť
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
