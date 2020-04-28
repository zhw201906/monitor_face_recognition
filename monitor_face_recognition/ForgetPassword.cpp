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
	setWindowTitle(QString::fromLocal8Bit("找回密码"));

	connect(ui.button_exit, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("返回登陆界面");
		emit BackLogin();
	});
}

void ForgetPassword::paintEvent(QPaintEvent *event)
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

ForgetPassword::~ForgetPassword()
{
}
