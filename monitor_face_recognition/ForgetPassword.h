#pragma once

#include <QWidget>
#include "ui_ForgetPassword.h"

class ForgetPassword : public QWidget
{
	Q_OBJECT

public:
	ForgetPassword(QWidget *parent = Q_NULLPTR);
	~ForgetPassword();

	void paintEvent(QPaintEvent *event);

signals:
	void BackLogin();
private:
	Ui::ForgetPassword ui;
};
