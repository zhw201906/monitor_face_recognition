#pragma once

#include <QWidget>
#include "ui_FaceAppLibrary.h"
#include "sqlite_operation.h"
#include <QMessageBox>
#include <QCloseEvent> 

enum app_operation_type { ADD_LIBRARY, MODIFY_LIBRARY};

class FaceAppLibrary : public QWidget
{
	Q_OBJECT

public:
	FaceAppLibrary(int operation, const UserApp &user_app, QWidget *parent = Q_NULLPTR);
	~FaceAppLibrary();

	void closeEvent(QCloseEvent *event);
	void paintEvent(QPaintEvent *event);


signals:
	//添加库或者修改库信息信号
	void AppLibraryInfo(int operation, UserApp &user_app);
	//退出，返回主界面信号
	void BackMainInterface();


private:
	Ui::FaceAppLibrary ui;

	QMessageBox msg_box_;
	int current_operation_;
};
