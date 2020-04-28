#include "MainInterface.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QLabel>
#include <QPalette>
#include <QIcon>
#include <QGroupBox>
#include <QFileDialog>
#include <QImage>
#include <QTreeWidget>
#include <QStringList>
#include <QListWidget>
#include <QToolButton>

//等比缩放图像
//参数1：图像路径（IN）
//参数2：空间宽度（IN）
//参数3：控件高度（IN）
//参数4：缩放后结果图（OUT）
void Geometric_Scaling_Image(const QString path, const int control_width, const int control_height, QPixmap &dst_pix)
{
	QPixmap pix;
	pix.load(path);

	double ratio_w = pix.width() * 1.0 / control_width;
	double ratio_h = pix.height() * 1.0 / control_height;

	if (ratio_w >= ratio_h)
		pix = pix.scaled(pix.width() / ratio_w, pix.height() / ratio_w);
	else
		pix = pix.scaled(pix.width() / ratio_h, pix.height() / ratio_h);

	dst_pix = pix;
}

//MainInterface::MainInterface(QWidget *parent)
MainInterface::MainInterface(SqlDatabaseClass *sql_handle, UserAccount *account_info, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	//this->setAttribute(Qt::WA_DeleteOnClose, true);   //关闭窗口时自动销毁
	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));

	ui.list_users_info->setViewMode(QListView::IconMode);
	//ui.list_users_info->setGridSize(QSize(230,225));
	ui.list_users_info->setResizeMode(QListWidget::Adjust);
	ui.list_users_info->setMovement(QListWidget::Static);

	ui.button_modify_user->setEnabled(false);   //未选中用户时，批量删除用户和修改用户按钮不可用
	ui.button_delete_user->setEnabled(false);

	ui.button_library_add->setIcon(QIcon("./Resources/icon/add_lib.jpg"));
	ui.button_library_del->setIcon(QIcon("./Resources/icon/del_lib.jpg"));
	ui.button_library_edit->setIcon(QIcon("./Resources/icon/edit_lib.jpg"));
	ui.button_library_refresh->setIcon(QIcon("./Resources/icon/refresh_lib.jpg"));

	ui.label_detect_result->setText(QString::fromLocal8Bit("人脸检测结果"));
	ui.label_recognize_result->setText(QString::fromLocal8Bit("人脸识别结果"));
	ui.button_online_monitoring->setStyleSheet("background-color:white");
	ui.button_camera_management->setStyleSheet("background-color:blue");
	ui.button_library_management->setStyleSheet("background-color:blue");
	ui.button_snap_query->setStyleSheet("background-color:blue");
	ui.button_track->setStyleSheet("background-color:blue");
	ui.button_intelligence_test->setStyleSheet("background-color:blue");

	ui.button_detect_task->setStyleSheet("color:red");
	ui.button_compare_task->setStyleSheet("color:red");
	ui.button_recognize_task->setStyleSheet("color:red");
	ui.button_display_result->setEnabled(false);
	ui.stacked_main_funcation->setCurrentIndex(3);

	ui.label_compare_image1->setStyleSheet("QLabel{border:1px solid rgb(0, 0, 100);background-color: rgb(255, 255, 255);}");
	ui.label_compare_image2->setStyleSheet("QLabel{border:1px solid rgb(0, 0, 100);background-color: rgb(255, 255, 255);}");
	//ui.label_detect_image->setStyleSheet("QLabel{background-color: rgb(255, 255, 255);}");
	ui.label_detect_image->setStyleSheet("QLabel{border:1px solid rgb(0, 0, 100);background-color: rgb(255, 255, 255);}");
	ui.label_reconige_image->setStyleSheet("QLabel{border:1px solid rgb(0, 0, 100);background-color: rgb(255, 255, 255);}");

	ui.groupBox->setStyleSheet("QGroupBox{border:1px solid rgb(0, 0, 0);}");
	ui.groupBox_2->setStyleSheet("QGroupBox{border:1px solid rgb(0, 0, 0);}");
	ui.groupBox_3->setStyleSheet("QGroupBox{border:1px solid rgb(0, 0, 0);}");

	//连接切换用户按钮
	connect(ui.button_change_account, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("切换用户");
		//ui.list_library_app->clear();
		if (display_detect_result_ != NULL)
			display_detect_result_->close();
		emit BackLogin();
	});

	//连接关闭系统按钮
	connect(ui.button_close_system, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("关闭系统");
		if (display_detect_result_ != NULL)
			display_detect_result_->close();
		this->close();
	});

	//连接主功能切换按钮
	connect(ui.button_online_monitoring, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("在线监控");
		ui.button_online_monitoring->setStyleSheet("background-color:white");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(3);
	});

	connect(ui.button_camera_management, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("相机管理");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:white");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(0);
	});

	connect(ui.button_library_management, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("库管理");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:white");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(2);
	});

	connect(ui.button_snap_query, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("抓拍查询");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:white");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(4);
	});

	connect(ui.button_track, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("行踪轨迹");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:white");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(5);
	});

	connect(ui.button_intelligence_test, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("智能测试");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:white");

		ui.stacked_main_funcation->setCurrentIndex(1);
	});

/*************************************************************************************************/
/*人脸对比部分功能实现，信号连接槽函数*/
	//上传第一张对比图片
	connect(ui.button_upload_image1, &QPushButton::clicked, [=]() {
		compare_image1_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择图片"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));

		QPixmap pix;
		Geometric_Scaling_Image(compare_image1_path_, ui.label_compare_image1->width(), ui.label_compare_image1->height(), pix);
		ui.label_compare_image1->setPixmap(pix);
		ui.button_compare_result->setText(" ");
	});

	//上传第二张对比图片
	connect(ui.button_upload_image2, &QPushButton::clicked, [=]() {
		compare_image2_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择图片"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));

		QPixmap pix;
		Geometric_Scaling_Image(compare_image2_path_, ui.label_compare_image2->width(), ui.label_compare_image2->height(), pix);
		ui.label_compare_image2->setPixmap(pix);
		ui.button_compare_result->setText(" ");
	});

	//人脸对比按钮
	connect(ui.button_compare_task, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("人脸比对");
		IplImage* img1 = cvLoadImage(compare_image1_path_.toLocal8Bit());
		if (img1 == NULL)
		{
			msg_box_.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("照片一为空！"));
			return;
		}
		ASF_MultiFaceInfo faceDetectResult;
		int res = face_opreation_->FacesDetectTask(img1, &faceDetectResult);
		ASF_FaceFeature feature;
		res = face_opreation_->FaceFeatureExtract(img1, faceDetectResult, &feature);

		ASF_FaceFeature feature_copy;
		feature_copy.feature = (MByte*)malloc(feature.featureSize);
		memcpy(feature_copy.feature, feature.feature, feature.featureSize);
		feature_copy.featureSize = feature.featureSize;


		IplImage* img2 = cvLoadImage(compare_image2_path_.toLocal8Bit());
		if (img2 == NULL)
		{
			msg_box_.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("照片二为空！"));
			return;
		}
		ASF_MultiFaceInfo faceDetectResult2;
		res = face_opreation_->FacesDetectTask(img2, &faceDetectResult2);
		res = face_opreation_->FaceFeatureExtract(img2, faceDetectResult2, &feature);

		MFloat confidenceValue;
		res = face_opreation_->FaceCompareTask(feature, feature_copy, &confidenceValue);

		QString str = QString("%1:%2%").arg(QString::fromLocal8Bit("相似度")).arg(QString::number(confidenceValue * 100, 'f', 2));
		
		ui.button_compare_result->setStyleSheet("background-color:white;color:red");
		ui.button_compare_result->setText(str);
		free(feature_copy.feature);
		cvReleaseImage(&img1);
		cvReleaseImage(&img2);
	});
/**************************************************************************************************/


/*==========================================================================================================================*/
/*人脸检测部分功能实现，信号连接槽函数*/
	//上传人脸检测图片
	connect(ui.button_upload_detect_image, &QPushButton::clicked, [=]() {
		detect_image_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择图片"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));
		QPixmap pix;
		Geometric_Scaling_Image(detect_image_path_, ui.label_detect_image->width(), ui.label_detect_image->height(), pix);
		//pix.load(detect_image_path_);
		ui.label_detect_image->setPixmap(pix);

		ui.tree_detect_result->clear();
		ui.button_display_result->setEnabled(false);
	});

	//人脸检测按钮
	connect(ui.button_detect_task, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("人脸检测");

		ui.tree_detect_result->clear();

		if (display_detect_result_ != NULL)
		{
			delete display_detect_result_;
			display_detect_result_ = NULL;
		}

		IplImage* img = cvLoadImage(detect_image_path_.toLocal8Bit());
		if (img == NULL)
		{
			msg_box_.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("照片为空！"));
			return;
		}

		FaceDetectResult face_detect_result;
		int res = face_opreation_->FacesDetectTask(img, &face_detect_result, face_detect_result_image_);

		ui.tree_detect_result->setHeaderLabels(QStringList()<< QString::fromLocal8Bit("属性")<< QString::fromLocal8Bit("详细信息"));

		QTreeWidgetItem *num_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("人脸个数") 
			                                         << QString::number(face_detect_result.detectInfo.faceNum));
		ui.tree_detect_result->addTopLevelItem(num_item);

		for (int i = 0; i < face_detect_result.detectInfo.faceNum; i++)
		{
			//人脸信息根节点
			QString str = QString("%1%2%3").arg(QString::fromLocal8Bit("人脸")).arg(QString::number(i + 1)).arg(QString::fromLocal8Bit("信息"));
			QTreeWidgetItem *info_item = new QTreeWidgetItem(QStringList() << str);
			ui.tree_detect_result->addTopLevelItem(info_item);

			//性别
			QString sex_str;
			if (face_detect_result.genderInfo.genderArray[i] == 0)
			{
				sex_str = QString("%1").arg(QString::fromLocal8Bit("男"));
			}
			else if (face_detect_result.genderInfo.genderArray[i] == 1)
			{
				sex_str = QString("%1").arg(QString::fromLocal8Bit("女"));
			}
			else
			{
				sex_str = QString("%1").arg(QString::fromLocal8Bit("未知"));
			}
			QTreeWidgetItem *sex_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("性别") << sex_str);
			info_item->addChild(sex_item);

			//年龄信息
			QTreeWidgetItem *age_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("年龄") 
				                                         << QString::number(face_detect_result.ageInfo.ageArray[i]));
			info_item->addChild(age_item);

			//人脸位置信息
			QTreeWidgetItem *location_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("人脸位置"));
			info_item->addChild(location_item);

			QTreeWidgetItem *left_location_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("left") 
				                                   << QString::number(face_detect_result.detectInfo.faceRect[i].left));
			location_item->addChild(left_location_item);

			QTreeWidgetItem *right_location_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("right")
				<< QString::number(face_detect_result.detectInfo.faceRect[i].right));
			location_item->addChild(right_location_item);

			QTreeWidgetItem *top_location_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("top")
				<< QString::number(face_detect_result.detectInfo.faceRect[i].top));
			location_item->addChild(top_location_item);

			QTreeWidgetItem *bottom_location_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("bottom")
				<< QString::number(face_detect_result.detectInfo.faceRect[i].bottom));
			location_item->addChild(bottom_location_item);

			//人脸3D信息
			QTreeWidgetItem *angle_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("3D信息"));
			info_item->addChild(angle_item);

			QTreeWidgetItem *pitch_angle_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("pitch")
				<< QString::number(face_detect_result.angleInfo.pitch[i],'f',2));
			angle_item->addChild(pitch_angle_item);

			QTreeWidgetItem *roll_angle_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("roll")
				<< QString::number(face_detect_result.angleInfo.roll[i], 'f', 2));
			angle_item->addChild(roll_angle_item);

			QTreeWidgetItem *yaw_angle_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("yaw")
				<< QString::number(face_detect_result.angleInfo.yaw[i], 'f', 2));
			angle_item->addChild(yaw_angle_item);
		}
		ui.button_display_result->setEnabled(true);
		cvReleaseImage(&img);
	});

	//查看人脸检测结果
	connect(ui.button_display_result, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("查看结果");

		if (display_detect_result_ == NULL)
		{
			display_detect_result_ = new DisplayDetectResult(face_detect_result_image_);
		}
		display_detect_result_->show();
	});
/*==========================================================================================================================*/


	//上传人脸识别图片
	connect(ui.button_upload_recognize_image, &QPushButton::clicked, [=]() {
		recognize_image_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("选择图片"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));

		QPixmap pix;
		Geometric_Scaling_Image(recognize_image_path_, ui.label_reconige_image->width(), ui.label_reconige_image->height(), pix);
		ui.label_reconige_image->setPixmap(pix);
	});

	//人脸识别按钮
	connect(ui.button_recognize_task, &QPushButton::clicked, [=]() {
		IplImage* img = cvLoadImage(recognize_image_path_.toLocal8Bit());

		face_opreation_->faceRecognitionResult.clear();

		cv::Mat dst;
		AllFacesFeature all_face_feature;
		//face_opreation_->FaceFeatureExtract(img, dst, all_face_feature);
		face_opreation_->FaceCompareTask(&all_face_feature, &sqlite_operation_->memery_user_feature_);


		ui.tree_recognize_result->clear();
		ui.tree_recognize_result->setHeaderLabels(QStringList() << QString::fromLocal8Bit("属性") << QString::fromLocal8Bit("详细信息"));

		QTreeWidgetItem *num_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("人脸个数")
			<< QString::number(face_opreation_->faceRecognitionResult.size()));
		ui.tree_recognize_result->addTopLevelItem(num_item);

		int i = 0;
		std::map<MFloat, int>::iterator itor = face_opreation_->faceRecognitionResult.begin();
		while (itor != face_opreation_->faceRecognitionResult.end())
		{			
			std::vector<UserMainInfo>::iterator it = sqlite_operation_->memery_user_main_info_.begin();
			while (it != sqlite_operation_->memery_user_main_info_.end())
			{
				if (it->user_info.user_id == itor->second)
					break;
				it++;
			}
			//人脸信息根节点
			QString str = QString("%1%2%3").arg(QString::fromLocal8Bit("人脸")).arg(QString::number(i + 1)).arg(QString::fromLocal8Bit("信息"));
			QTreeWidgetItem *info_item = new QTreeWidgetItem(QStringList() << str);
			ui.tree_recognize_result->addTopLevelItem(info_item);

			//姓名
			QTreeWidgetItem *name_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("姓名") 
				<< QString::fromLocal8Bit(it->user_info.user_name));
			info_item->addChild(name_item);

			//性别
			QTreeWidgetItem *sex_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("性别") 
				<< QString::fromLocal8Bit(it->user_info.user_sex));
			info_item->addChild(sex_item);

			//年龄
			QTreeWidgetItem *age_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("年龄")
				<< QString::number(it->user_info.user_age));
			info_item->addChild(age_item);

			//联系方式
			QTreeWidgetItem *phone_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("联系方式")
				<< QString::fromLocal8Bit(it->user_info.phone_num));
			info_item->addChild(phone_item);

			//地址
			QTreeWidgetItem *address_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("地址")
				<< QString::fromLocal8Bit(it->user_info.user_address));
			info_item->addChild(address_item);

			i++;
			itor++;

		}

		cvReleaseImage(&img);
		//for (int i = 0; i < all_face_feature.user_num; i++)
		//{
		//	float cfd_value = 0;
		//	int   userid = 0;
		//	std::vector<FacesFeatureUrl>::iterator itor = sqlite_operation_->memery_user_feature_.begin();
		//	for (; itor != sqlite_operation_->memery_user_feature_.end(); itor++)
		//	{
		//		float confidenceValue;
		//		ASF_FaceFeature singal_face_feature;

		//		singal_face_feature.feature = (MByte*)malloc(itor->feature_size);
		//		memcpy(singal_face_feature.feature, itor->feature, itor->feature_size);
		//		singal_face_feature.featureSize = itor->feature_size;

		//		face_opreation_->FaceCompareTask(all_face_feature.face_feature[i], singal_face_feature, &confidenceValue);
		//		if (confidenceValue > cfd_value) 
		//		{
		//			cfd_value = confidenceValue;
		//			userid = itor->user_id;
		//		}
		//		//free(singal_face_feature.feature);
		//	}
		//	//free(all_face_feature.face_feature[i].feature);
		//	map_recognize_result_[cfd_value] = userid;	
		//	qDebug() << "recognize userid:" << userid << endl;
		//}
		
		//free(all_face_feature.face_feature);


		//for (int i = 0; i < map_recognize_result_.size(); i++)
		//{
		//	qDebug() << "recognize userid:" << recognize_user.userId[i] << endl;
		//}

	});




	if (sqlite_operation_ == NULL)
		sqlite_operation_ = new SqlDatabaseClass;
	sqlite_operation_ = sql_handle;

	if (current_account_info_ == NULL)
		current_account_info_ = new UserAccount;
	current_account_info_ = account_info;
	
	RefreshListWidget();
	//ui.list_library_app->setCurrentRow(0);
	ClearUserListWidget();
	//current_app_id_ = map_user_app_recode_[0].app_id;
	DisplayCurrentAppUserInfo(0);

	//初始化人脸识别引擎
	FaceEngineInitParams face_engine_init_params;
	face_engine_init_params.detectMode = ASF_DETECT_MODE_IMAGE;
	face_engine_init_params.faceMaxNum = 40;
	face_engine_init_params.faceOrient = ASF_OP_0_ONLY;
	face_engine_init_params.faceScaleVal = 32;
	face_engine_init_params.functionMask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE |
		ASF_LIVENESS | ASF_IR_LIVENESS;

	if (face_opreation_ == NULL)
		face_opreation_ = new FaceEngineClass(face_engine_init_params);

	QString user_name(current_account_info_->user_name);
	ui.button_user_account_name->setText(user_name);

	if (current_account_info_->account_type == 1)   //普通用户
	{
		QIcon icon("./Resources/icon/user.jpg");
		ui.button_user_account_name->setIconSize(QSize(35, 35));
		ui.button_user_account_name->setIcon(icon);
	}
	else if (current_account_info_->account_type == 2)  //管理员
	{
		QIcon icon("./Resources/icon/admin.jpg");
		ui.button_user_account_name->setIconSize(QSize(35, 35));
		ui.button_user_account_name->setIcon(icon);
	}
	

/************************************************************************************************************************/
/*人脸数据库，用户组操作相关部分*/

	//连接添加人脸数据库按钮
	connect(ui.button_library_add, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("添加人脸库");

		if (face_app_library_ == NULL)
		{
			UserApp user_app;
			face_app_library_ = new FaceAppLibrary(ADD_LIBRARY, user_app);
		}
		face_app_library_->show();

		//连接添加新人脸库信号
		connect(face_app_library_, &FaceAppLibrary::AppLibraryInfo, [=](int operation, UserApp user_app) {
			if (operation == ADD_LIBRARY)  //创建新的用户组
			{
				msg_box_.information(face_app_library_, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加用户组成功！"));
				sqlite_operation_->CreateUserApp(user_app); //写数据库
				sqlite_operation_->UpdateMemeryUserApp();   //更新内存中人脸组信息
				RecodeUserAppMap();                         //更新ListWidget待显示人脸组信息
				RefreshListWidget();                        //刷新ListWidget人脸组信息
			}
		});

		//连接返回主界面信号
		connect(face_app_library_, &FaceAppLibrary::BackMainInterface, [=]() {
			if (face_app_library_ != NULL)
			{
				delete face_app_library_;
				face_app_library_ = NULL;
			}
		});
	});

	//连接编辑人脸数据库按钮
	connect(ui.button_library_edit, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("编辑人脸库");

		int row = ui.list_library_app->currentRow();

		std::map<int, UserApp>::iterator it = map_user_app_recode_.find(row);

		if (face_app_library_ == NULL)
		{
			UserApp user_app;
			face_app_library_ = new FaceAppLibrary(MODIFY_LIBRARY, it->second);
		}
		face_app_library_->show();

		//编辑人脸库信号
		connect(face_app_library_, &FaceAppLibrary::AppLibraryInfo, [=](int operation, UserApp user_app) {
			if (operation == MODIFY_LIBRARY)
			{
				msg_box_.information(face_app_library_, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("修改用户组成功！"));
				sqlite_operation_->ModifyUserAppByid(it->second.app_id, user_app);
				sqlite_operation_->UpdateMemeryUserApp();
				RecodeUserAppMap();
				RefreshListWidget();
			}

		});

		//连接返回主界面信号
		connect(face_app_library_, &FaceAppLibrary::BackMainInterface, [=]() {
			if (face_app_library_ != NULL)
			{
				delete face_app_library_;
				face_app_library_ = NULL;
			}
		});		
	});

	//连接删除人脸库按钮
	connect(ui.button_library_del, &QPushButton::clicked, [=]() {
		int row = ui.list_library_app->currentRow();

		if (row == -1 || current_app_id_ == 0)
		{
			msg_box_.critical(this, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未选择用户组！"));
			return;
		}

		int index = QMessageBox::question(this, QString::fromLocal8Bit("提示"),
			QString::fromLocal8Bit("确定删除用户组'%1'及全部用户信息?\n此操作不可恢复！").arg(QString::fromLocal8Bit(map_user_app_recode_[row].app_name)),
			QString::fromLocal8Bit("确定"),
			QString::fromLocal8Bit("取消"), "", 0, 0);

		if (index != 0)
			return;

		sqlite_operation_->DeleteUserAppByid(map_user_app_recode_[row].app_id);
		sqlite_operation_->UpdateMemeryUserApp();
		RecodeUserAppMap();
		RefreshListWidget();
	});

	//连接刷新库列表按钮
	connect(ui.button_library_refresh, &QPushButton::clicked, [=]() {
		sqlite_operation_->UpdateMemeryUserApp();
		//RecodeUserAppMap();
		RefreshListWidget();
		ClearUserListWidget();
		DisplayCurrentAppUserInfo(0);
	});

	//连接点击用户组信号
	connect(ui.list_library_app, &QListWidget::itemClicked, [=]() {
		qDebug() << QString::fromLocal8Bit("点击ListWidget。当前行数：") << ui.list_library_app->currentRow();

		ClearUserListWidget();
		current_app_id_ = map_user_app_recode_[ui.list_library_app->currentRow()].app_id;
		DisplayCurrentAppUserInfo(current_app_id_);
	});

/************************************************************************************************************************/


/*=======================================================================================================================*/
/*人脸数据库，用户操作相关部分*/

	//连接添加新用户按钮
	connect(ui.button_add_user, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("添加新用户");
		AddNewUser();
	});

	//连接编辑用户信息按钮
	connect(ui.button_modify_user, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("编辑用户信息");
		if (current_user_id_ == 0)
		{
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未选择用户！"));
			return;
		}
		if (vector_delete_select_user.size() > 1)
		{
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("已选择用户多个用户！"));
			return;
		}
		ModifyUserInfo(current_user_id_);
		ui.button_modify_user->setEnabled(false);
		ui.button_delete_user->setEnabled(false);
		vector_delete_select_user.clear();

	});

	//连接刷新用户信息按钮
	connect(ui.button_update_users, &QPushButton::clicked, [=]() {
		ClearUserListWidget();
		DisplayCurrentAppUserInfo(current_app_id_);
	});

	//连接批量删除按钮
	connect(ui.button_delete_user, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("批量删除");

		DeleteMultiUserInfo();
		ui.button_modify_user->setEnabled(false);
		ui.button_delete_user->setEnabled(false);
		vector_delete_select_user.clear();
	});

/*
	QString dir_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("选择目录"), "D:\\test_image", QFileDialog::ShowDirsOnly);

		QDir dir(dir_path);

		QStringList ImageList;

		ImageList << "*.bmp" << "*.jpg" << "*.png";//向字符串列表添加图片类型

		dir.setNameFilters(ImageList);//获得文件夹下图片的名字

		int ImageCount = dir.count();//获得dir里名字的个数，也表示文件夹下图片的个数

		for (int i = 0; i < ImageCount; i++)
		{
			QString image_path = dir_path + '/' + dir[i];


			QPixmap pix;
			pix.load(image_path);

			if (pix.isNull())
				return;

			UserInfoIcon *photo_view_item = new UserInfoIcon;

			float ratio_w = pix.width()*1.0 / photo_view_item->ui.label_image->width();
			float ratio_h = pix.height()*1.0 / photo_view_item->ui.label_image->height();

			if (ratio_w >= ratio_h)  //图片按照长轴缩放，保持长宽比
			{
				pix = pix.scaledToWidth(photo_view_item->ui.label_image->width());
			}
			else
			{
				pix = pix.scaledToHeight(photo_view_item->ui.label_image->height());
			}

			photo_view_item->ui.label_image->setPixmap(pix);


			QListWidgetItem *nitem = new QListWidgetItem(ui.list_users_info);
			nitem->setSizeHint(QSize(230, 225));
			ui.list_users_info->addItem(nitem);
			ui.list_users_info->setItemWidget(nitem, photo_view_item);

			photo_view_item->ui.box_name->setText(dir[i]);
		}
		ui.list_users_info->show();
*/
	//连接点击用户信号
	connect(ui.list_users_info, &QListWidget::itemClicked, [=]() {
		qDebug() << QString::fromLocal8Bit("点击ListWidget。当前行数：") << ui.list_users_info->currentRow();

		////ui.list_users_info->setCurrentIndex
		//int index = ui.list_users_info->currentRow();
		//ui.list_users_info->setCurrentRow(index);
		////ui.list_users_info->setFocus(Qt::MouseFocusReason);

	});
	

/*=======================================================================================================================*/
}

void MainInterface::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QPixmap pix;
	pix.load("./Resources/icon/MainBackGround.jpg");
	//指定长宽
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
	//在背景上加图标背景
	pix.load("./Resources/icon/logo.png");
	////缩放
	pix = pix.scaled(pix.width()*0.18, pix.height()*0.18);
	painter.drawPixmap(5, 5, pix);
}

void MainInterface::DisplayListWidget()
{
	for (int i = 0; i < sqlite_operation_->memery_user_app_.size(); i++)
	{
		map_user_app_recode_[i] = sqlite_operation_->memery_user_app_[i];
		QString str_item = QString("%1").arg(QString::fromLocal8Bit(sqlite_operation_->memery_user_app_[i].app_name));
		QListWidgetItem *widget_item = new QListWidgetItem(str_item);
		ui.list_library_app->addItem(widget_item);
	}
}

void MainInterface::ClearListWidget()
{
	int count = ui.list_library_app->count();
	for (int i = count - 1; i >= 0; i--)
	{
		QListWidgetItem *item = ui.list_library_app->item(i);
		ui.list_library_app->removeItemWidget(item);
		delete item;
	}
}

void MainInterface::RefreshListWidget()
{
	ClearListWidget();
	DisplayListWidget();
}

void MainInterface::DisplayUserListWidget(UserInfo &user_info, QString image_path)
{
	UserInfoIcon *user_info_icon = new UserInfoIcon(user_info, image_path);

	//连接添加人脸
	connect(user_info_icon, &UserInfoIcon::AddUserFaceImage, [=](int user_id) {
		qDebug() << QString::fromLocal8Bit("添加人脸，用户id:") << user_id;
		AddFaceToUser(user_id);
	});

	//连接删除用户
	connect(user_info_icon, &UserInfoIcon::DelUserAllInfo, [=](int user_id) {
		qDebug() << QString::fromLocal8Bit("删除用户，用户id") << user_id;
		DeleteUserInfo(user_id);
	});

	//连接--选择状态按钮
	connect(user_info_icon, &UserInfoIcon::SelectUser, [=](int state, int user_id) {
		if (state == Qt::Checked)
		{
			qDebug() << QString::fromLocal8Bit("选中用户") << user_id << "    row:" << ui.list_users_info->currentRow();
			current_user_id_ = user_id;
			ui.button_modify_user->setEnabled(true);
			ui.button_delete_user->setEnabled(true);
			vector_delete_select_user.push_back(user_id);
		}
		else
		{
			current_user_id_ = 0;
			
			std::vector<int>::iterator it = vector_delete_select_user.begin();
			while (it != vector_delete_select_user.end())
			{
				if (*it == user_id)
				{
					vector_delete_select_user.erase(it);
					break;
				}
				it++;
			}
			
			if (vector_delete_select_user.size() == 0)
			{
				ui.button_modify_user->setEnabled(false);
				ui.button_delete_user->setEnabled(false);
			}
		}		
	});

	//点击选中用户
	connect(user_info_icon, &UserInfoIcon::PressUser, [=](int user_id) {
		qDebug() << QString::fromLocal8Bit("控件事件,当前选中用户：") << user_id;
		//user_info_icon->setFocus(Qt::MouseFocusReason);

	});

	QListWidgetItem *diaplay_user_item = new QListWidgetItem(ui.list_users_info);
	diaplay_user_item->setSizeHint(QSize(220, 225));
	ui.list_users_info->addItem(diaplay_user_item);
	ui.list_users_info->setItemWidget(diaplay_user_item, user_info_icon);
}

void MainInterface::ClearUserListWidget()
{
	int count = ui.list_users_info->count();
	for (int i = count - 1; i >= 0; i--)
	{
		QListWidgetItem *item = ui.list_users_info->item(i);
		ui.list_users_info->removeItemWidget(item);
		delete item;
	}
}

void MainInterface::RefreshUserListWidget()
{
	ClearUserListWidget();
	DisplayCurrentAppUserInfo(current_app_id_);
}

void MainInterface::RecodeUserAppMap()
{
	map_user_app_recode_.clear();
	for (int i = 0; i < sqlite_operation_->memery_user_app_.size(); i++)
	{
		map_user_app_recode_[i] = sqlite_operation_->memery_user_app_[i];
	}
}

void MainInterface::DisplayCurrentAppUserInfo(int app_id)
{
	for (int i = 0; i < sqlite_operation_->memery_user_main_info_.size(); i++)
	{
		if (app_id != 0)
		{
			if (app_id != sqlite_operation_->memery_user_main_info_[i].user_info.app_id)
				continue;

			DisplayUserListWidget(sqlite_operation_->memery_user_main_info_[i].user_info,
				QString(sqlite_operation_->memery_user_main_info_[i].url));
		}
		else
		{
			DisplayUserListWidget(sqlite_operation_->memery_user_main_info_[i].user_info,
				QString(sqlite_operation_->memery_user_main_info_[i].url));
		}
	}
}

void MainInterface::ModifyUserInfo(int user_id)
{
	UserInfo user_info;
	
	std::vector<UserMainInfo>::iterator itor = sqlite_operation_->memery_user_main_info_.begin();
	for (; itor != sqlite_operation_->memery_user_main_info_.end(); ++itor)
	{
		if (itor->user_info.user_id == user_id)
		{
			user_info = itor->user_info;
			break;
		}
	}

	if (face_user_library_ == NULL)
	{
		face_user_library_ = new FaceUserLibrary(MODIFY_USER, user_info);
	}
	face_user_library_->show();

	//连接新建用户信号
	connect(face_user_library_, &FaceUserLibrary::ModifyUser, [=](UserInfo user_info) {
		qDebug() << QString::fromLocal8Bit("确定修改用户信息");

		
		sqlite_operation_->ModifyUserInfoByid(user_info);  //写数据库

		msg_box_.information(face_user_library_, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("修改用户信息成功！"));
		
		//清除用户原本信息
		//sqlite_operation_->memery_user_main_info_.erase(itor);
		itor->user_info = user_info;

		ClearUserListWidget();
		DisplayCurrentAppUserInfo(current_app_id_);

		//显示新添加用户，并更新内存中数据
		//DisplayUserListWidget(user_info, image_save_path);

		//UserMainInfo new_user_main_ifo;
		//new_user_main_ifo.user_info = user_info;
		//new_user_main_ifo.user_info.user_id = current_user_id_;

		//strcpy(new_user_main_ifo.app_name, map_user_app_recode_[ui.list_library_app->currentRow()].app_name);
		//strcpy(new_user_main_ifo.url, image_save_path);
		//sqlite_operation_->memery_user_main_info_.push_back(new_user_main_ifo);

	});

	//连接返回主界面信号
	connect(face_user_library_, &FaceUserLibrary::BackMainInterface, [=]() {
		qDebug() << QString::fromLocal8Bit("退出添加新用户");

		if (face_user_library_ != NULL)
		{
			delete face_user_library_;
			face_user_library_ = NULL;
		}
	});
}

void MainInterface::AddNewUser()
{
	if (current_app_id_ == 0)
	{
		msg_box_.critical(face_user_library_, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("未选择用户组！"));
		return;
	}

	UserInfo user_info;
	if (face_user_library_ == NULL)
	{
		face_user_library_ = new FaceUserLibrary(ADD_USER, user_info);
	}
	face_user_library_->show();

	//连接新建用户信号
	connect(face_user_library_, &FaceUserLibrary::AddUserInfo, [=](UserInfo user_info, QString image_path) {
		qDebug() << QString::fromLocal8Bit("确定添加新用户");

		IplImage* img = cvLoadImage(image_path.toLocal8Bit());
		if (img == NULL)
		{
			face_user_library_->click_count_--;
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("读取图片失败！"));
			return;
		}
		ASF_MultiFaceInfo faceDetectResult;
		int res = face_opreation_->FacesDetectTask(img, &faceDetectResult);
		ASF_FaceFeature feature;
		res = face_opreation_->FaceFeatureExtract(img, faceDetectResult, &feature);

		user_info.app_id = current_app_id_;

		if (face_user_library_->click_count_ <= 1)
		{
			sqlite_operation_->CreatUserInfo(user_info, current_user_id_);
			user_info.user_id = current_user_id_;
			msg_box_.information(face_user_library_, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加新用户成功！"));
		}
		else
		{
			msg_box_.information(face_user_library_, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("该用户已添加！"));
			return;
		}

		UserFeature user_feature;
		user_feature.user_id = current_user_id_;
		user_feature.feature = (unsigned char*)malloc(feature.featureSize);
		memcpy(user_feature.feature, feature.feature, feature.featureSize);
		user_feature.feature_len = feature.featureSize;

		sprintf(user_feature.location, "left:%d\nright:%d\ntop:%d\nbottom:%d\n", faceDetectResult.faceRect->left,
			faceDetectResult.faceRect->right, faceDetectResult.faceRect->top, faceDetectResult.faceRect->bottom);

		char image_save_path[100] = { 0 };
		cv::Mat dst_img(img);
		int img_w = dst_img.cols;
		int img_h = dst_img.rows;
		sprintf(image_save_path, ".\\user_image_lib\\app_%d_user_%d_%dx%d.jpg", user_info.app_id, current_user_id_, 
			faceDetectResult.faceRect->left, faceDetectResult.faceRect->top);

		if (dst_img.cols > 1080)
		{
			cv::resize(dst_img, dst_img, cv::Size(1080, 1080 * img_h / img_w ));
		}

		if (dst_img.rows > 960)
		{
			cv::resize(dst_img, dst_img, cv::Size(960 * img_w / img_h , 960));
		}
		cv::imwrite(image_save_path, dst_img);


		cvReleaseImage(&img);
		memcpy(user_feature.url, image_save_path, strlen(image_save_path) + 1);

		sqlite_operation_->AddUserFeature(user_feature);

		//显示新添加用户，并更新内存中数据
		DisplayUserListWidget(user_info, image_save_path);

		UserMainInfo new_user_main_ifo;
		new_user_main_ifo.user_info = user_info;
		new_user_main_ifo.user_info.user_id = current_user_id_;

		strcpy(new_user_main_ifo.app_name, map_user_app_recode_[ui.list_library_app->currentRow()].app_name);
		strcpy(new_user_main_ifo.url, image_save_path);
		sqlite_operation_->memery_user_main_info_.push_back(new_user_main_ifo);

	});

	//连接返回主界面信号
	connect(face_user_library_, &FaceUserLibrary::BackMainInterface, [=]() {
		qDebug() << QString::fromLocal8Bit("退出添加新用户");

		if (face_user_library_ != NULL)
		{
			delete face_user_library_;
			face_user_library_ = NULL;
		}
	});
}

void MainInterface::AddFaceToUser(int user_id)
{
	UserInfo user_info;

	std::vector<UserMainInfo>::iterator itor = sqlite_operation_->memery_user_main_info_.begin();
	for (; itor != sqlite_operation_->memery_user_main_info_.end(); ++itor)
	{
		if (itor->user_info.user_id == user_id)
		{
			user_info = itor->user_info;
			break;
		}
	}

	if (face_user_library_ == NULL)
	{
		face_user_library_ = new FaceUserLibrary(ADD_USER_FACE, user_info);
	}
	face_user_library_->show();

	//连接新建用户信号
	connect(face_user_library_, &FaceUserLibrary::AddUserFace, [=](QString image_path) {
		qDebug() << QString::fromLocal8Bit("确定添加用户人脸");

		IplImage* img = cvLoadImage(image_path.toLocal8Bit());
		if (img == NULL)
		{
			face_user_library_->click_count_--;
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("错误"), QString::fromLocal8Bit("读取图片失败！"));
			return;
		}

		ASF_MultiFaceInfo faceDetectResult;
		int res = face_opreation_->FacesDetectTask(img, &faceDetectResult);
		ASF_FaceFeature feature;
		res = face_opreation_->FaceFeatureExtract(img, faceDetectResult, &feature);

		UserFeature user_feature;
		user_feature.user_id = itor->user_info.user_id;
		user_feature.feature = (unsigned char*)malloc(feature.featureSize);
		memcpy(user_feature.feature, feature.feature, feature.featureSize);
		user_feature.feature_len = feature.featureSize;

		sprintf(user_feature.location, "left:%d\nright:%d\ntop:%d\nbottom:%d\n", faceDetectResult.faceRect->left,
			faceDetectResult.faceRect->right, faceDetectResult.faceRect->top, faceDetectResult.faceRect->bottom);

		char image_save_path[100] = { 0 };
		cv::Mat dst_img(img);
		int img_w = dst_img.cols;
		int img_h = dst_img.rows;
		sprintf(image_save_path, ".\\user_image_lib\\app_%d_user_%d_%dx%d.jpg", itor->user_info.app_id, itor->user_info.user_id, 
			  faceDetectResult.faceRect->left, faceDetectResult.faceRect->top);

		if (dst_img.cols > 1080)
		{
			cv::resize(dst_img, dst_img, cv::Size(1080, 1080 * img_h / img_w));
		}

		if (dst_img.rows > 960)
		{
			cv::resize(dst_img, dst_img, cv::Size(960 * img_w / img_h , 960));
		}
		cv::imwrite(image_save_path, dst_img);


		cvReleaseImage(&img);
		memcpy(user_feature.url, image_save_path, strlen(image_save_path) + 1);

		sqlite_operation_->AddUserFeature(user_feature);
		msg_box_.information(face_user_library_, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加用户人脸成功！"));

	});

	//连接返回主界面信号
	connect(face_user_library_, &FaceUserLibrary::BackMainInterface, [=]() {
		qDebug() << QString::fromLocal8Bit("退出添加用户人脸");

		if (face_user_library_ != NULL)
		{
			delete face_user_library_;
			face_user_library_ = NULL;
		}
	});

}

void MainInterface::DeleteUserInfo(int user_id)
{
	sqlite_operation_->DeleteUserInfoByid(user_id);
	sqlite_operation_->DeleteMemeryUserMainInfo(user_id);
	RefreshUserListWidget();
}

void MainInterface::DeleteMultiUserInfo(void)
{
	std::vector<int>::iterator it = vector_delete_select_user.begin();

	while (it != vector_delete_select_user.end())
	{
		sqlite_operation_->DeleteUserInfoByid(*it);
		sqlite_operation_->DeleteMemeryUserMainInfo(*it);
		it++;
	}
	RefreshUserListWidget();
}

MainInterface::~MainInterface()
{
	ClearListWidget();
	ClearUserListWidget();
	if (sqlite_operation_ != NULL)
	{	
		sqlite_operation_ = NULL;
		delete sqlite_operation_;
		//sqlite_operation_ = NULL;		
	}
		
	if (current_account_info_ != NULL)
	{		
		current_account_info_ = NULL;
		delete current_account_info_;
		
	}
	
	if (display_detect_result_ != NULL)
	{	
		display_detect_result_ = NULL;
		delete display_detect_result_;
		
	}

	if (face_app_library_ != NULL)
	{	
		face_app_library_ = NULL;
		delete face_app_library_;
				
	}

	if (face_user_library_ != NULL)
	{
		face_user_library_ = NULL;
		delete face_user_library_;
		
	}

	
}
