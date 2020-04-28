#include "FaceAppLibrary.h"
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QDateTime>
#include <QDebug>

/**************************人脸库管理类******************************/
//构造函数
//*参数1：    0：添加新用户组       1：修改当前用户组
//*参数2：    添加新用户组时为空，修改用户组时，传入当前参数

FaceAppLibrary::FaceAppLibrary(int operation, const UserApp &user_app, QWidget *parent)
	: QWidget(parent), current_operation_(operation)
{
	ui.setupUi(this);

	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));

	setWindowModality(Qt::ApplicationModal);     //设置为模态窗口

	if (operation == ADD_LIBRARY)
	{
		setWindowTitle(QString::fromLocal8Bit("新建库"));
		ui.label_4->setText(QString::fromLocal8Bit("新建用户组"));
	}		
	else  if(operation == MODIFY_LIBRARY)
	{
		setWindowTitle(QString::fromLocal8Bit("修改库"));
		ui.label_4->setText(QString::fromLocal8Bit("修改用户组信息"));
		ui.edit_app_name->setText(QString::fromLocal8Bit(user_app.app_name));
		ui.edit_app_mark->setText(QString::fromLocal8Bit(user_app.remark));
	}
	
	//设置阈值框的值范围
	ui.spin_threshold->setMinimum(0);
	ui.spin_threshold->setMaximum(100);

	//连接文本框与滑块
	void(QSpinBox:: *spin_box)(int) = &QSpinBox::valueChanged;
	connect(ui.spin_threshold, spin_box, ui.slider_threshold, &QSlider::setValue);

	//连接滑块与文本框
	connect(ui.slider_threshold, &QSlider::valueChanged, ui.spin_threshold, &QSpinBox::setValue);

	ui.spin_threshold->setValue(user_app.threshold);

	//连接退出按钮
	connect(ui.button_exit, &QPushButton::clicked, [=]() {
		//this->close();
		emit BackMainInterface();
	});

	//连接确定按钮     --数据写入数据库
	connect(ui.button_ensure, &QPushButton::clicked, [=]() {
		UserApp user_app;

		if (!ui.edit_app_name->text().isEmpty())
		{
			QString qstr = ui.edit_app_name->text();
			strcpy(user_app.app_name, qstr.toLocal8Bit().data());
		}
		else
		{
			msg_box_.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("库名称为空!"));
			return;
		}

		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString(Qt::ISODate);
		strcpy(user_app.create_time, current_date.toLocal8Bit().data());

		QString mark = ui.edit_app_mark->text();
		strcpy(user_app.remark, mark.toLocal8Bit().data());


		user_app.threshold = ui.spin_threshold->value();
		//this->close();
		emit AppLibraryInfo(current_operation_, user_app);//
		//emit BackMainInterface();
		
	});
}

void FaceAppLibrary::closeEvent(QCloseEvent * event)
{
	emit BackMainInterface();
}

//设置背景
void FaceAppLibrary::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap pix;
	pix.load("./Resources/icon/login.jpg");
	//指定长宽
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
}

FaceAppLibrary::~FaceAppLibrary()
{

}
