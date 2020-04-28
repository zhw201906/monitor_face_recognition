#include "FaceUserLibrary.h"
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QSlider>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QString>


//operation:���ܶ���
FaceUserLibrary::FaceUserLibrary(int operation, const UserInfo &user_info, QWidget *parent)
	: QWidget(parent), current_operation_(operation), user_info_pro_(user_info)
{
	ui.setupUi(this);

	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));

	setWindowModality(Qt::ApplicationModal);     //����Ϊģ̬����

	ui.edit_name->setPlaceholderText(QString::fromLocal8Bit("����"));
	ui.edit_age->setPlaceholderText(QString::fromLocal8Bit("18"));
	ui.edit_phone->setPlaceholderText(QString::fromLocal8Bit("15525896547"));
	ui.edit_card_id->setPlaceholderText(QString::fromLocal8Bit("421035199909151028"));
	ui.edit_address->setPlaceholderText(QString::fromLocal8Bit("�Ĵ��ɶ������3��"));
	ui.edit_remark->setPlaceholderText(QString::fromLocal8Bit("С������"));

	ui.label_display_image->setStyleSheet("QLabel{border:1px solid rgb(0, 0, 100);background-color: rgb(255, 255, 255);}");

	if (current_operation_ == ADD_USER)   //������û�
	{
		setWindowTitle(QString::fromLocal8Bit("������û�"));
		ui.label_title->setText(QString::fromLocal8Bit("�½��û�"));
		ui.button_load_image->setEnabled(true);
	}
	else if (current_operation_ == MODIFY_USER)  //�޸��û���Ϣ
	{
		setWindowTitle(QString::fromLocal8Bit("�༭�û���Ϣ"));
		ui.label_title->setText(QString::fromLocal8Bit("�༭�û�"));

		ui.edit_name->setText(QString::fromLocal8Bit(user_info.user_name));
		ui.edit_age->setText(QString("%1").arg(user_info.user_age));
		ui.edit_phone->setText(QString::fromLocal8Bit(user_info.phone_num));
		ui.edit_card_id->setText(QString::fromLocal8Bit(user_info.user_card_id));
		ui.edit_address->setText(QString::fromLocal8Bit(user_info.user_address));
		ui.edit_remark->setText(QString::fromLocal8Bit(user_info.remark));

		ui.box_sex->setCurrentText(QString::fromLocal8Bit(user_info.user_sex));
		ui.box_user_type->setCurrentIndex(user_info.user_type);

		ui.button_load_image->setEnabled(false);
	}
	else if (current_operation_ == ADD_USER_FACE)  //����û�����
	{
		setWindowTitle(QString::fromLocal8Bit("����û�����"));
		ui.label_title->setText(QString::fromLocal8Bit("�������"));

		ui.edit_name->setText(QString::fromLocal8Bit(user_info.user_name));
		ui.edit_age->setText(QString("%1").arg(user_info.user_age));
		ui.edit_phone->setText(QString::fromLocal8Bit(user_info.phone_num));
		ui.edit_card_id->setText(QString::fromLocal8Bit(user_info.user_card_id));
		ui.edit_address->setText(QString::fromLocal8Bit(user_info.user_address));
		ui.edit_remark->setText(QString::fromLocal8Bit(user_info.remark));
		ui.box_sex->setCurrentText(QString::fromLocal8Bit(user_info.user_sex));
		ui.box_user_type->setCurrentIndex(user_info.user_type);

		ui.edit_name->setEnabled(false);
		ui.edit_age->setEnabled(false);
		ui.edit_phone->setEnabled(false);
		ui.edit_card_id->setEnabled(false);
		ui.edit_address->setEnabled(false);
		ui.edit_remark->setEnabled(false);
		ui.box_sex->setEnabled(false);
		ui.box_user_type->setEnabled(false);

		ui.button_load_image->setEnabled(true);
	}


	//�����ϴ�ͼƬ��ť
	connect(ui.button_load_image, &QPushButton::clicked, [=]() {
		image_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͼƬ"), OPEN_IMAGE_PATH,
			                                       tr("Images (*.png *.jpg);; All files (*.*)"));
		QPixmap pix;
		pix.load(image_path_);

		float ratio_w = pix.width()*1.0 / ui.label_display_image->width();
		float ratio_h = pix.height()*1.0 / ui.label_display_image->height();

		if (ratio_w >= ratio_h)  //ͼƬ���ճ������ţ����ֳ����
		{
			pix = pix.scaledToWidth(ui.label_display_image->width());
		}
		else
		{
			pix = pix.scaledToHeight(ui.label_display_image->height());
		}

		ui.label_display_image->setPixmap(pix);
	});

	//����ȷ����ť
	connect(ui.button_sure, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("ȷ����ť");

		if (current_operation_ == ADD_USER)   //������û�
		{
			UserInfo user_info;

			QString qstr = ui.edit_name->text();
			strcpy(user_info.user_name, qstr.toLocal8Bit().data());

			QString age = ui.edit_age->text();
			user_info.user_age = age.toInt();

			QString phone = ui.edit_phone->text();
			strcpy(user_info.phone_num, phone.toLocal8Bit().data());

			QString card = ui.edit_card_id->text();
			strcpy(user_info.user_card_id, card.toLocal8Bit().data());

			QString address = ui.edit_address->text();
			strcpy(user_info.user_address, address.toLocal8Bit().data());

			QString remark = ui.edit_remark->text();
			strcpy(user_info.remark, remark.toLocal8Bit().data());

			user_info.user_type = ui.box_user_type->currentIndex();

			QString sex = ui.box_sex->currentText();
			strcpy(user_info.user_sex, sex.toLocal8Bit().data());

			QDateTime current_date_time = QDateTime::currentDateTime();
			QString current_date = current_date_time.toString(Qt::ISODate);
			strcpy(user_info.create_time, current_date.toLocal8Bit().data());

			emit AddUserInfo(user_info, image_path_);
		}
		else if (current_operation_ == MODIFY_USER)  //�޸��û���Ϣ
		{
			UserInfo user_info(user_info_pro_);

			QString qstr = ui.edit_name->text();
			strcpy(user_info.user_name, qstr.toLocal8Bit().data());

			QString age = ui.edit_age->text();
			user_info.user_age = age.toInt();

			QString phone = ui.edit_phone->text();
			strcpy(user_info.phone_num, phone.toLocal8Bit().data());

			QString card = ui.edit_card_id->text();
			strcpy(user_info.user_card_id, card.toLocal8Bit().data());

			QString address = ui.edit_address->text();
			strcpy(user_info.user_address, address.toLocal8Bit().data());

			QString remark = ui.edit_remark->text();
			strcpy(user_info.remark, remark.toLocal8Bit().data());

			user_info.user_type = ui.box_user_type->currentIndex();

			QString sex = ui.box_sex->currentText();
			strcpy(user_info.user_sex, sex.toLocal8Bit().data());

			emit ModifyUser(user_info);
		}
		else if (current_operation_ == ADD_USER_FACE)  //����û�����
		{
			emit AddUserFace(image_path_);
		}




		//UserInfo user_info;
		click_count_++;

		//if (!ui.edit_name->text().isEmpty())     //�û���
		//{
		//	/*QString qstr = ui.edit_name->text();
		//	strcpy(user_info.user_name, qstr.toLocal8Bit().data());*/
		//}
		//else
		//{
		//	//msg_box_.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("�û�������Ϊ��!"));

		//}
		
	});

	//�����˳���ť
	connect(ui.button_exit, &QPushButton::clicked, [=]() {		
		//this->close();
		emit BackMainInterface();
	});
}

void FaceUserLibrary::closeEvent(QCloseEvent * event)
{
	emit BackMainInterface();
}

void FaceUserLibrary::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	QPixmap pix;
	pix.load("./Resources/icon/login.jpg");
	//ָ������
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
}


FaceUserLibrary::~FaceUserLibrary()
{
}

