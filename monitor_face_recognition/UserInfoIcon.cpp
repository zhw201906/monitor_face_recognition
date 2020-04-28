#include "UserInfoIcon.h"
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>

UserInfoIcon::UserInfoIcon(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//connect(ui)


}

UserInfoIcon::UserInfoIcon(UserInfo &user_info,QString image_path)
{
	ui.setupUi(this);

	QPixmap pix;
	pix.load(image_path);

	if (pix.width() > ui.label_image->width() || pix.height() > ui.label_image->height())
	{
		float ratio_w = pix.width()*1.0 / ui.label_image->width();
		float ratio_h = pix.height()*1.0 / ui.label_image->height();

		if (ratio_w >= ratio_h)  //ͼƬ���ճ������ţ����ֳ����
		{
			pix = pix.scaledToWidth(ui.label_image->width());
		}
		else
		{
			pix = pix.scaledToHeight(ui.label_image->height());
		}
	}
	
	ui.label_image->setPixmap(pix);
	ui.box_name->setText(QString::fromLocal8Bit(user_info.user_name));

	current_user_id_ = user_info.user_id;

	//�������������ť
	connect(ui.button_add, &QPushButton::clicked, [=]() {
		emit AddUserFaceImage(current_user_id_);
	});

	//����ɾ���û���ť
	connect(ui.button_del, &QPushButton::clicked, [=]() {
		QMessageBox msg_box;

		int index = QMessageBox::question(this, QString::fromLocal8Bit("��ʾ"), 
			                                    QString::fromLocal8Bit("ȷ��ɾ���û�'%1'��ȫ������ͼƬ?\n�˲������ɻָ���").arg(QString::fromLocal8Bit(user_info.user_name)),
			                                    QString::fromLocal8Bit("ȷ��"),
			                                    QString::fromLocal8Bit("ȡ��"), "", 0, 0);

		if (index == 0)
			emit DelUserAllInfo(current_user_id_);
		else
			return;
	});

	//���Ӱ���ѡ���
	connect(ui.box_name, &QCheckBox::stateChanged, [=](int state) {
		emit SelectUser(state, current_user_id_);
	});
}

UserInfoIcon::~UserInfoIcon()
{

}

//void UserInfoIcon::mousePressEvent(QMouseEvent * event)
//{
//	//ui.box_name->setFocus(Qt::MouseFocusReason);
//	//emit PressUser(current_user_id_);
//}
