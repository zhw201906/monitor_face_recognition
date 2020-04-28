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

	//��ȡ�����ļ�����ȡ�û��˻���Ϣ
	QSettings config(CONFIG_FILE_NAME, QSettings::IniFormat);
	QString user_account_ = config.value("ACCOUNT_INFO/user_account", "").toString();
	QString user_password_ = config.value("ACCOUNT_INFO/user_password", "").toString();
	remeber_password_ = config.value("ACCOUNT_INFO/is_remeber_password", "0").toInt();
	auto_login_ = config.value("ACCOUNT_INFO/is_auto_login", "0").toInt();

	//�˻�������Ϊ��ʱ����ʾ���ʺź�������ʾ��
	if (!user_account_.isEmpty())
		ui.edit_account->setText(user_account_);
	else
		ui.edit_account->setPlaceholderText(QString::fromLocal8Bit("�������ʺ�"));

	if (!user_password_.isEmpty())
		ui.edit_password->setText(user_password_);
	else
		ui.edit_password->setPlaceholderText(QString::fromLocal8Bit("����������"));

	if (auto_login_ == true)
		ui.box_auto_login->setCheckState(Qt::Checked);
	else
		ui.box_auto_login->setCheckState(Qt::Unchecked);

	if (remeber_password_ == true)
		ui.box_remeber_password->setCheckState(Qt::Checked);
	else
		ui.box_remeber_password->setCheckState(Qt::Unchecked);

	//�����ݿ�
	if (sqlite_operation_ == NULL)
		sqlite_operation_ = new SQLITE_OPERATION;
	sqlite_operation_->SqliteInit();
	sqlite_operation_->MemeryInit();

	//������ǰ�˻���Ϣ
	if (current_account_info_ == NULL)
		current_account_info_ = new UserAccount;


/*��������¼�����и����ź���ۺ���*/

	//�˳�ϵͳ��ť
	connect(ui.button_exit, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�˳�ϵͳ");
		this->close();
	});

	//ע���ʺŰ�ť
	connect(ui.button_create_account, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("ע���˻�");
		if (creat_account_ == NULL)
		{
			creat_account_ = new CreateAccount;
		}

		//����--��ע���û����淵�ص�¼����
		connect(creat_account_, &CreateAccount::BackLogin, this, &Login::BackLoginFromCreateAccount);

		//ע���û�
		connect(creat_account_, &CreateAccount::AccountDataSignals, this, &Login::DealAddAccount);
		QTimer::singleShot(500, this, [=]() {
			this->hide();
			creat_account_->show();
		});	
	});

	//�������밴ť
	connect(ui.button_forget_password, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("��������");
		if (forget_password_ == NULL)
		{
			forget_password_ = new ForgetPassword;
		}

		//����--������������淵�ص�¼����
		connect(forget_password_, &ForgetPassword::BackLogin, this, &Login::BackLoginFromForgetPassword);
		QTimer::singleShot(500, this, [=]() {
			this->hide();
			forget_password_->show();
		});
	});

	//���ӵ�¼��ť��ۺ���
	connect(ui.button_login, SIGNAL(clicked(bool)), this, SLOT(ClickLogin()));

	//ѡ���ס����ѡ���
	connect(ui.box_remeber_password, SIGNAL(stateChanged(int)), this, SLOT(CheckRemeberPassword(int)));

	//ѡ���Զ���¼ѡ���
	connect(ui.box_auto_login, SIGNAL(stateChanged(int)), this, SLOT(CheckAutoLogin(int)));

	////�ʺ������Ϊ��ʱ�ۺ���
	//connect(ui.edit_account, &QLineEdit::textChanged, [=]() {
	//	if(ui.edit_account->text().isEmpty())
	//		ui.edit_account->setPlaceholderText(QString::fromLocal8Bit("�������ʺ�"));
	//});
}

//���ñ���
void Login::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap pix;
	pix.load("./Resources/icon/login.jpg");
	//ָ������
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
	//�ڱ����ϼ�ͼ�걳��
	//pix.load(":/image/1.jpg");
	////����
	//pix = pix.scaled(pix.width()*0.5, pix.width()*0.5);
	//painter.drawPixmap(10, 30, pix);
}

//ע�����û����ص�¼����ۺ���
void Login::BackLoginFromCreateAccount()
{
	qDebug() << QString::fromLocal8Bit("���ص�¼����");
	QTimer::singleShot(500, this, [=]() {
		creat_account_->hide();
		creat_account_->close();
		delete creat_account_;
		creat_account_ = NULL;
		this->show();
	});	
}

//�������뷵�ص�¼����ۺ���
void Login::BackLoginFromForgetPassword()
{
	qDebug() << QString::fromLocal8Bit("���ص�¼����");
	QTimer::singleShot(500, this, [=]() {
		forget_password_->hide();
		forget_password_->close();
		delete forget_password_;
		forget_password_ = NULL;
		this->show();
	});	
}

//�����淵�ص�¼����ۺ���
void Login::BackLoginFromMainInterface()
{
	qDebug() << QString::fromLocal8Bit("���ص�¼����");
	QTimer::singleShot(500, this, [=]() {
		main_interface_->hide();
		main_interface_->close();
		delete main_interface_;
		main_interface_ = NULL;
		this->show();
	});	
}

//ע���û�У��ע����Ϣ�ۺ���
void Login::DealAddAccount(UserAccount user_account)
{
	QMessageBox msg_box;
	int result = 0;
	sqlite_operation_->QueryUserAccountInfo(user_account, result);
	if (result == 0)
	{
		sqlite_operation_->AddUserAccount(user_account);
		msg_box.information(creat_account_, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("ע��ɹ���"));
	}
	else if (result == 1)
	{
		msg_box.critical(creat_account_, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�˻��ѱ�ע�ᣡ"));
	}
	else if (result == 2)
	{
		msg_box.critical(creat_account_, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����ѱ�ע�ᣡ"));
	}
}

//��¼ϵͳ�ۺ���
void Login::ClickLogin(void)
{
	QMessageBox msg_box;

	//��ȡ�ı����ʺ�
	if (!ui.edit_account->text().isEmpty())      
	{
		std::string str = ui.edit_account->text().toStdString();
		strcpy(current_account_info_->user_account, str.c_str());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�������ʺ�!"));
		return;
	}

	//��ȡ�ı����û�����
	if (!ui.edit_password->text().isEmpty())      
	{
		std::string str = ui.edit_password->text().toStdString();
		strcpy(current_account_info_->user_password, str.c_str());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("����������!"));
		return;
	}

	//���û�����������Ϣʱ�������˻���֤�������˳�����ʾ������Ϣ
	if (!ui.edit_account->text().isEmpty() || !ui.edit_password->text().isEmpty())
	{
		bool verify_success = false;
		int result = 0;

		sqlite_operation_->VerifyUserAccount(current_account_info_, result);  //��֤��¼�û���Ϣ�Ƿ���ȷ

		//��֤��Ϣ������result������
		//*��λ��1���˻�����  2���˻�������
		//*ʮλ��1��������ȷ  2���������
		//*��λ���˻�����
		qDebug() << "user_name" << current_account_info_->user_name << endl;
		if (result == 0)
		{
			msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�˻�������!"));
		}
		else
		{
			int account_type = result / 100 % 10;
			int password_result = result / 10 % 10;
			int account_result = result % 10;

			if (account_result == 2)
			{
				msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�˻�������!"));
			}
			else
			{
				if (password_result == 2)
				{
					msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�������!"));
				}
				else
				{
					verify_success = true;
					current_account_info_->account_type = account_type;
				}
			}
		}

		//��֤��¼��Ϣ��ȷ
		if (verify_success)
		{
			//д�����ļ�
			QSettings config(CONFIG_FILE_NAME, QSettings::IniFormat);
			QString user_account_ = ui.edit_account->text();
			config.setValue("ACCOUNT_INFO/user_account", user_account_);
			if (remeber_password_ == true)  //ѡ���ס����
			{	
				QString user_password_ = ui.edit_password->text();  //д���뵽�����ļ�
				config.setValue("ACCOUNT_INFO/user_password", user_password_);
			}
			else
			{
				config.setValue("ACCOUNT_INFO/user_password", " ");
			}
			config.setValue("ACCOUNT_INFO/is_remeber_password", remeber_password_);
			config.setValue("ACCOUNT_INFO/is_auto_login", auto_login_);

			//���������ڶ���
			if (main_interface_ == NULL)
			{
				//main_interface_ = new MainInterface(current_account_info_);
				main_interface_ = new MainInterface(sqlite_operation_, current_account_info_);
			}

			//�������л��û��ۺ���
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

