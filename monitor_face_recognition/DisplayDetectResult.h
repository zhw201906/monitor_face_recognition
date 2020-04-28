#pragma once

#include <QWidget>
#include "ui_DisplayDetectResult.h"
#include "opencv2/opencv.hpp"

class DisplayDetectResult : public QWidget
{
	Q_OBJECT

public:
	//DisplayDetectResult(QWidget *parent = Q_NULLPTR);
	DisplayDetectResult(cv::Mat &img,QWidget *parent = Q_NULLPTR);
	~DisplayDetectResult();

	void paintEvent(QPaintEvent *event);

private:
	Ui::DisplayDetectResult ui;
};
