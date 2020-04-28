#include "CreateAccount.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>

CreateAccount::CreateAccount(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setFixedSize(460, 540);
	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));
	setWindowTitle(QString::fromLocal8Bit("�������û�"));
	
	ui.edit_user_name->setPlaceholderText(QString::fromLocal8Bit("����"));
	ui.edit_account->setPlaceholderText("zhongguo123");
	ui.edit_password->setPlaceholderText("zg123!");
	ui.edit_ensure_password->setPlaceholderText("zg123!");
	ui.edit_email->setPlaceholderText("131452021@qq.com");
	ui.edit_serial_num->setEnabled(false);
	ui.edit_serial_num->setReadOnly(true);

	//���巵�ص�¼����İ�ť
	connect(ui.button_exit, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�˳�ע��");
		emit BackLogin();
	});

	connect(ui.box_user_type, SIGNAL(currentIndexChanged(int)), this, SLOT(ChangeUserType(int)));
	connect(ui.button_register, SIGNAL(clicked(bool)), this, SLOT(ClickRegister()));
}

void CreateAccount::paintEvent(QPaintEvent *event)
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

void CreateAccount::ChangeUserType(int index)
{
	if (index == 0)
	{
		//
		ui.edit_serial_num->setEnabled(false);
		ui.edit_serial_num->setReadOnly(true);
	}
	else if (index == 1)
	{
		//
		ui.edit_serial_num->setEnabled(true);
		ui.edit_serial_num->setReadOnly(false);
	}

}

void CreateAccount::ClickRegister(void)
{
	UserAccount user_account;
	QMessageBox msg_box;
	bool send_write_database_flag = false;

	if (!ui.edit_user_name->text().isEmpty())     //�û���
	{
		QString qstr = ui.edit_user_name->text();
		strcpy(user_account.user_name, qstr.toLocal8Bit().data());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�û�������Ϊ��!"));
		send_write_database_flag = true;
		return;
	}

	if (!ui.edit_account->text().isEmpty())      //�û��˺�
	{
		std::string str = ui.edit_account->text().toStdString();
		strcpy(user_account.user_account, str.c_str());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�˺Ų���Ϊ��!"));
		send_write_database_flag = true;
		return;
	}
	std::string password;
	if (!ui.edit_password->text().isEmpty())     //�û�����
	{
		std::string str = ui.edit_password->text().toStdString();
		password = str;
		strcpy(user_account.user_password, str.c_str());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���벻��Ϊ��!"));
		send_write_database_flag = true;
		return;
	}

	if (!ui.edit_ensure_password->text().isEmpty())  //ȷ������
	{
		std::string str = ui.edit_ensure_password->text().toStdString();
		if (str != password)
		{
			msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�����������벻һ��!"));
			send_write_database_flag = true;
		}
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("ȷ�����벻��Ϊ��!"));
		send_write_database_flag = true;
		return;
	}

	if (!ui.edit_email->text().isEmpty())      //�û�����
	{
		std::string str = ui.edit_email->text().toStdString();
		strcpy(user_account.user_email, str.c_str());
	}
	else
	{
		msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���䲻��Ϊ��!"));
		send_write_database_flag = true;
		return;
	}

	user_account.account_type = ui.box_user_type->currentIndex() + 1;
	if (user_account.account_type == 2)
	{
		if (ui.edit_serial_num->text().isEmpty())
		{	
			msg_box.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("���������к�!"));
			send_write_database_flag = true;
			return;
		}
	}
	std::string str = ui.edit_serial_num->text().toStdString();
	strcpy(user_account.serial_num, str.c_str());

	if (!send_write_database_flag)
	{
		emit AccountDataSignals(user_account);
	}
}

CreateAccount::~CreateAccount()
{
}
