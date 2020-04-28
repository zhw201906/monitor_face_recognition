#include "DisplayDetectResult.h"
#include <QLabel>
#include <QPainter>

////�ȱ�����ͼ��
////����1��ͼ��·����IN��
////����2���ռ��ȣ�IN��
////����3���ؼ��߶ȣ�IN��
////����4�����ź���ͼ��OUT��
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
		cv::cvtColor(mat, Rgb, CV_BGR2RGB);//��ɫ�ռ�ת��
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
	//ָ������
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
	//�ڱ����ϼ�ͼ�걳��
	//pix.load("./Resources/icon/logo.png");
	//////����
	//pix = pix.scaled(pix.width()*0.18, pix.height()*0.18);
	//painter.drawPixmap(5, 5, pix);
}

DisplayDetectResult::~DisplayDetectResult()
{
}
