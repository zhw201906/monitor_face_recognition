#pragma once

#include <QtWidgets/QWidget>
#include "ui_Login.h"
#include "CreateAccount.h"
#include "MainInterface.h"
#include "ForgetPassword.h"
#include "code/database/sqlite_operation.h"

#define  CONFIG_FILE_NAME  "Login_Account_Config.ini"

class Login : public QWidget
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();

	void paintEvent(QPaintEvent *event);

public slots:
	void BackLoginFromCreateAccount();
	void BackLoginFromForgetPassword();
	void BackLoginFromMainInterface();

	void DealAddAccount(UserAccount user_account);
	void ClickLogin(void);

	void CheckRemeberPassword(int state);
	void CheckAutoLogin(int state);

private:
	Ui::LoginClass ui;

	CreateAccount *creat_account_ = NULL;
	MainInterface *main_interface_ = NULL;
	ForgetPassword *forget_password_ = NULL;

	SqlDatabaseClass *sqlite_operation_ = NULL;
	UserAccount *current_account_info_ = NULL;

	int remeber_password_ = 0;
	int auto_login_ = 0;

};
