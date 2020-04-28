#include "Login.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <QMessageBox>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>

Login::Login(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));

	//读取配置文件，获取用户账户信息
	QSettings config(CONFIG_FILE_NAME, QSettings::IniFormat);
	QString user_account_ = config.value("ACCOUNT_INFO/user_account", "").toString();
	QString user_password_ = config.value("ACCOUNT_INFO/user_password", "").toString();
	remeber_password_ = config.value("ACCOUNT_INFO/is_remeber_password", "0").toInt();
	auto_login_ = config.value("ACCOUNT_INFO/is_auto_login", "0").toInt();

	//账户和密码为空时，显示“帐号和密码提示”
	if (!user_account_.isEmpty())
		ui.edit_account->setText(user_account_);
	else
		ui.edit_account->setPlaceholderText(QString::fromLocal8Bit("请输入帐号"));

	if (!user_password_.isEmpty())
		ui.edit_password->setText(user_password_);
	else
		ui.edit_password->setPlaceholderText(QString::fromLocal8Bit("请输入密码"));

	if (auto_login_ == true)
		ui.box_auto_login->setCheckState(Qt::Checked);
	else
		ui.box_auto_login->setCheckState(Qt::Unchecked);

	if (remeber_password_ == true)
		ui.box_remeber_password->setCheckState(Qt::Checked);
	else
		ui.box_remeber_password->setCheckState(Qt::Unchecked);

	//打开数据库
	if (sqlite_operation_ == NULL)
		sqlite_operation_ = new SQLITE_OPERATION;
	sqlite_operation_->SqliteInit();
	sqlite_operation_->MemeryInit();

	//创建当前账户信息
	if (current_account_info_ == NULL)
		current_account_info_ = new UserAccount;


/*连接主登录界面中各种信号与槽函数*/

	//退出系统按钮
	connect(ui.button_exit, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("退出系统");
		this->close();
	});

	//注册帐号按钮
	connect(ui.button_create_account, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("注册账户");
		if (creat_account_ == NULL)
		{
			creat_account_ = new CreateAccount;
		}

		//连接--从注册用户界面返回登录界面
		connect(creat_account_, &CreateAccount::BackLogin, this, &Login::BackLoginFromCreateAccount);

		//注册用户
		connect(creat_account_, &CreateAccount::AccountDataSignals, this, &Login::DealAddAccount);
		QTimer::singleShot(500, this, [=]() {
			this->hide();
			creat_account_->show();
		});	
	});

	//忘记密码按钮
	connect(ui.button_forget_password, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("忘记密码");
		if (forget_password_ == NULL)
		{
			forget_password_ = new ForgetPassword;
		}

		//连接--从忘记密码界面返回登录界面
		connect(forget_password_, &ForgetPassword::BackLogin, this, &Login::BackLoginFromForgetPassword);
		QTimer::singleShot(500, this, [=]() {
			this->hide();
			forget_password_->show();
		});
	});

	//连接登录按钮与槽函数
	connect(ui.button_login, SIGNAL(clicked(bool)), this, SLOT(ClickLogin()));

	//选择记住密码选择框
	connect(ui.box_remeber_password, SIGNAL(stateChanged(int)), this, SLOT(CheckRemeberPassword(int)));

	//选择自动登录选择框
	connect(ui.box_auto_login, SIGNAL(stateChanged(int)), this, SLOT(CheckAutoLogin(int)));

	////帐号输入框为空时槽函数
	//connect(ui.edit_account, &QLineEdit::textChanged, [=]() {
	//	if(ui.edit_account->text().isEmpty())
	//		ui.edit_account->setPlaceholderText(QString::fromLocal8Bit("请输入帐号"));
	//});
}

//设置背景
void Login::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap pix;
	pix.load("./Resources/icon/login.jpg");
	//指定长宽
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
	//在背景上加图标背景
	//pix.load(":/image/1.jpg");
	////缩放
	//pix = pix.scaled(pix.width()*0.5, pix.width()*0.5);
	//painter.drawPixmap(10, 30, pix);
}

//注册新用户返回登录界面槽函数
void Login::BackLoginFromCreateAccount()
{
	qDebug() << QString::fromLocal8Bit("返回登录界面");
	QTimer::singleShot(500, this, [=]() {
		creat_account_->hide();
		creat_account_->close();
		delete creat_account_;
		creat_account_ = NULL;
		this->show();
	});	
}

//忘记密码返回登录界面槽函数
void Login::BackLoginFromForgetPassword()
{
	qDebug() << QString::fromLocal8Bit("返回登录界面");
	QTimer::singleShot(500, this, [=]() {
		forget_password_->hide();
		forget_password_->close();
		delete forget_password_;
		forget_password_ = NULL;
		this->show();
	});	
}

//主界面返回登录界面槽函数
void Login::BackLoginFromMainInterface()
{
	qDebug() << QString::fromLocal8Bit("返回登录界面");
	QTimer::singleShot(500, this, [=]() {
		main_interface_->hide();
		main_interface_->close();
		delete main_interface_;
		main_interface_ = NULL;
		this->show();
	});	
}

//注册用户校验注册信息槽函数
void Login::DealAddAccount(UserAccount user_account)
{
	QMessageBox msg_box;
	int result = 0;
	sqlite_operation_->QueryUserAccountInfo(user_account, result);
	if (result == 0)
	{
		sqlite_operation_->AddUserAccount(user_account);
		msg_box.information(creat_account_, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("注册成功！"));
	}
	else if (result == 1)
	{
		msg_box.critical(creat_account_, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("账户已被注册！"));
	}
	else if (result == 2)
	{
		msg_box.critical(creat_account_, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("邮箱已被注册！"));
	}
}

//登录系统槽函数
void Login::ClickLogin(void)
{
	QMessageBox msg_box;

	//读取文本框帐号
	if (!ui.edit_account->text().isEmpty())      
	{
		std::string str = ui.edit_account->text().toStdString();
		strcpy(current_account_info_->user_account, str.c_str());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请输入帐号!"));
		return;
	}

	//读取文本框用户密码
	if (!ui.edit_password->text().isEmpty())      
	{
		std::string str = ui.edit_password->text().toStdString();
		strcpy(current_account_info_->user_password, str.c_str());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("请输入密码!"));
		return;
	}

	//当用户输入完整信息时，进行账户验证，否则退出并提示输入信息
	if (!ui.edit_account->text().isEmpty() || !ui.edit_password->text().isEmpty())
	{
		bool verify_success = false;
		int result = 0;

		sqlite_operation_->VerifyUserAccount(current_account_info_, result);  //验证登录用户信息是否正确

		//验证信息包含在result变量中
		//*个位：1、账户存在  2、账户不存在
		//*十位：1、密码正确  2、密码错误
		//*百位：账户属性
		qDebug() << "user_name" << current_account_info_->user_name << endl;
		if (result == 0)
		{
			msg_box.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("账户不存在!"));
		}
		else
		{
			int account_type = result / 100 % 10;
			int password_result = result / 10 % 10;
			int account_result = result % 10;

			if (account_result == 2)
			{
				msg_box.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("账户不存在!"));
			}
			else
			{
				if (password_result == 2)
				{
					msg_box.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("密码错误!"));
				}
				else
				{
					verify_success = true;
					current_account_info_->account_type = account_type;
				}
			}
		}

		//验证登录信息正确
		if (verify_success)
		{
			//写配置文件
			QSettings config(CONFIG_FILE_NAME, QSettings::IniFormat);
			QString user_account_ = ui.edit_account->text();
			config.setValue("ACCOUNT_INFO/user_account", user_account_);
			if (remeber_password_ == true)  //选择记住密码
			{	
				QString user_password_ = ui.edit_password->text();  //写密码到配置文件
				config.setValue("ACCOUNT_INFO/user_password", user_password_);
			}
			else
			{
				config.setValue("ACCOUNT_INFO/user_password", " ");
			}
			config.setValue("ACCOUNT_INFO/is_remeber_password", remeber_password_);
			config.setValue("ACCOUNT_INFO/is_auto_login", auto_login_);

			//创建主窗口对象
			if (main_interface_ == NULL)
			{
				//main_interface_ = new MainInterface(current_account_info_);
				main_interface_ = new MainInterface(sqlite_operation_, current_account_info_);
			}

			//主窗口切换用户槽函数
			connect(main_interface_, &MainInterface::BackLogin, this, &Login::BackLoginFromMainInterface);
			this->hide();
			main_interface_->show();
		}
	}
}

void Login::CheckRemeberPassword(int state)
{
	if (state == Qt::Unchecked)
	{
		this->remeber_password_ = 0;
	}
	else if (state == Qt::Checked)
	{
		this->remeber_password_ = 1;
	}
}

void Login::CheckAutoLogin(int state)
{
	if (state == Qt::Unchecked)
	{
		this->auto_login_ = 0;
	}
	else if (state == Qt::Checked)
	{
		this->auto_login_ = 1;
	}
}

Login::~Login()
{
	if (main_interface_ != NULL)
	{
		delete main_interface_;
		main_interface_ = NULL;
	}
	if (creat_account_ != NULL)
	{
		delete creat_account_;
		creat_account_ = NULL;
	}
	if (forget_password_ != NULL)
	{
		delete forget_password_;
		forget_password_ = NULL;
	}
}

