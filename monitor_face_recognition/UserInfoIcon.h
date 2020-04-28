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
	void AddUserFaceImage(int user_id);  //����û����������ź�
	void DelUserAllInfo(int user_id);    //ɾ���û��ź�
	void SelectUser(int state, int user_id);  //ѡ���ѡ��ȡ��״̬�ź�
	void PressUser(int user_id);
	


public:
	Ui::UserInfoIcon ui;

	int current_user_id_;


};
