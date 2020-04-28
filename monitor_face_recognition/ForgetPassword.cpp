#include "ForgetPassword.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>

ForgetPassword::ForgetPassword(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	setFixedSize(480, 400);
	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));
	setWindowTitle(QString::fromLocal8Bit("�һ�����"));

	connect(ui.button_exit, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("���ص�½����");
		emit BackLogin();
	});
}

void ForgetPassword::paintEvent(QPaintEvent *event)
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

ForgetPassword::~ForgetPassword()
{
}
