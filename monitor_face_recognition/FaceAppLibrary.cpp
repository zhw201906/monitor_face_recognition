#include "FaceAppLibrary.h"
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QDateTime>
#include <QDebug>

/**************************�����������******************************/
//���캯��
//*����1��    0��������û���       1���޸ĵ�ǰ�û���
//*����2��    ������û���ʱΪ�գ��޸��û���ʱ�����뵱ǰ����

FaceAppLibrary::FaceAppLibrary(int operation, const UserApp &user_app, QWidget *parent)
	: QWidget(parent), current_operation_(operation)
{
	ui.setupUi(this);

	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));

	setWindowModality(Qt::ApplicationModal);     //����Ϊģ̬����

	if (operation == ADD_LIBRARY)
	{
		setWindowTitle(QString::fromLocal8Bit("�½���"));
		ui.label_4->setText(QString::fromLocal8Bit("�½��û���"));
	}		
	else  if(operation == MODIFY_LIBRARY)
	{
		setWindowTitle(QString::fromLocal8Bit("�޸Ŀ�"));
		ui.label_4->setText(QString::fromLocal8Bit("�޸��û�����Ϣ"));
		ui.edit_app_name->setText(QString::fromLocal8Bit(user_app.app_name));
		ui.edit_app_mark->setText(QString::fromLocal8Bit(user_app.remark));
	}
	
	//������ֵ���ֵ��Χ
	ui.spin_threshold->setMinimum(0);
	ui.spin_threshold->setMaximum(100);

	//�����ı����뻬��
	void(QSpinBox:: *spin_box)(int) = &QSpinBox::valueChanged;
	connect(ui.spin_threshold, spin_box, ui.slider_threshold, &QSlider::setValue);

	//���ӻ������ı���
	connect(ui.slider_threshold, &QSlider::valueChanged, ui.spin_threshold, &QSpinBox::setValue);

	ui.spin_threshold->setValue(user_app.threshold);

	//�����˳���ť
	connect(ui.button_exit, &QPushButton::clicked, [=]() {
		//this->close();
		emit BackMainInterface();
	});

	//����ȷ����ť     --����д�����ݿ�
	connect(ui.button_ensure, &QPushButton::clicked, [=]() {
		UserApp user_app;

		if (!ui.edit_app_name->text().isEmpty())
		{
			QString qstr = ui.edit_app_name->text();
			strcpy(user_app.app_name, qstr.toLocal8Bit().data());
		}
		else
		{
			msg_box_.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("������Ϊ��!"));
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

//���ñ���
void FaceAppLibrary::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap pix;
	pix.load("./Resources/icon/login.jpg");
	//ָ������
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
}

FaceAppLibrary::~FaceAppLibrary()
{

}
