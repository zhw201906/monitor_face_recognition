#pragma once

#include <QWidget>
#include "ui_CreateAccount.h"
#include "code/database/sqlite_operation.h"

class CreateAccount : public QWidget
{
	Q_OBJECT

public:
	CreateAccount(QWidget *parent = Q_NULLPTR);
	~CreateAccount();

	void paintEvent(QPaintEvent *event);
	



signals:
	void BackLogin();
	void AccountDataSignals(UserAccount user_account);


public slots:
	void ChangeUserType(int index);
	void ClickRegister(void);


private:
	Ui::CreateAccount ui;
};
