#pragma once

#include <QWidget>
#include "ui_UserInfoIcon.h"
#include "sqlite_operation.h"
#include <QEvent>

class UserInfoIcon : public QWidget
{
	Q_OBJECT

public:
	UserInfoIcon(QWidget *parent = Q_NULLPTR);
	UserInfoIcon(UserInfo &user_info,QString image_path);

	~UserInfoIcon();


	//void mousePressEvent(QMouseEvent *event);


signals:
	void AddUserFaceImage(int user_id);  //添加用户人脸数据信号
	void DelUserAllInfo(int user_id);    //删除用户信号
	void SelectUser(int state, int user_id);  //选择框选择取消状态信号
	void PressUser(int user_id);
	


public:
	Ui::UserInfoIcon ui;

	int current_user_id_;


};
