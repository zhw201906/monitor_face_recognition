#include "DisplayDetectResult.h"
#include <QLabel>
#include <QPainter>

////等比缩放图像
////参数1：图像路径（IN）
////参数2：空间宽度（IN）
////参数3：控件高度（IN）
////参数4：缩放后结果图（OUT）
//void Geometric_Scaling_Image(const QString path, const int control_width, const int control_height, QPixmap &dst_pix)
//{
//	QPixmap pix;
//	pix.load(path);
//
//	double ratio_w = pix.width() * 1.0 / control_width;
//	double ratio_h = pix.height() * 1.0 / control_height;
//
//	if (ratio_w >= ratio_h)
//		pix = pix.scaled(pix.width() / ratio_w, pix.height() / ratio_w);
//	else
//		pix = pix.scaled(pix.width() / ratio_h, pix.height() / ratio_h);
//
//	dst_pix = pix;
//}

void LabelDisplayMat(QLabel *label, cv::Mat &mat)
{
	cv::Mat Rgb;
	QImage Img;
	if (mat.channels() == 3)//RGB Img
	{
		cv::cvtColor(mat, Rgb, CV_BGR2RGB);//颜色空间转换
		Img = QImage((const uchar*)(Rgb.data), Rgb.cols, Rgb.rows, Rgb.cols * Rgb.channels(), QImage::Format_RGB888);
	}
	else//Gray Img
	{
		Img = QImage((const uchar*)(mat.data), mat.cols, mat.rows, mat.cols*mat.channels(), QImage::Format_Indexed8);
	}
	label->setPixmap(QPixmap::fromImage(Img));
}

DisplayDetectResult::DisplayDetectResult(cv::Mat &img, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));

	LabelDisplayMat(ui.label, img);
}

void DisplayDetectResult::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap pix;
	pix.load("./Resources/icon/background2.jpg");
	//指定长宽
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
	//在背景上加图标背景
	//pix.load("./Resources/icon/logo.png");
	//////缩放
	//pix = pix.scaled(pix.width()*0.18, pix.height()*0.18);
	//painter.drawPixmap(5, 5, pix);
}

DisplayDetectResult::~DisplayDetectResult()
{
}
