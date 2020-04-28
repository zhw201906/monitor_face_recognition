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

		if (ratio_w >= ratio_h)  //图片按照长轴缩放，保持长宽比
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

	//连接添加人脸按钮
	connect(ui.button_add, &QPushButton::clicked, [=]() {
		emit AddUserFaceImage(current_user_id_);
	});

	//连接删除用户按钮
	connect(ui.button_del, &QPushButton::clicked, [=]() {
		QMessageBox msg_box;

		int index = QMessageBox::question(this, QString::fromLocal8Bit("提示"), 
			                                    QString::fromLocal8Bit("确定删除用户'%1'及全部人脸图片?\n此操作不可恢复！").arg(QString::fromLocal8Bit(user_info.user_name)),
			                                    QString::fromLocal8Bit("确定"),
			                                    QString::fromLocal8Bit("取消"), "", 0, 0);

		if (index == 0)
			emit DelUserAllInfo(current_user_id_);
		else
			return;
	});

	//连接按键选择框
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
