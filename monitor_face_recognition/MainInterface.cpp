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

//�ȱ�����ͼ��
//����1��ͼ��·����IN��
//����2���ռ��ȣ�IN��
//����3���ؼ��߶ȣ�IN��
//����4�����ź���ͼ��OUT��
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
	//this->setAttribute(Qt::WA_DeleteOnClose, true);   //�رմ���ʱ�Զ�����
	setWindowIcon(QIcon("./Resources/icon/icon.jpg"));

	ui.list_users_info->setViewMode(QListView::IconMode);
	//ui.list_users_info->setGridSize(QSize(230,225));
	ui.list_users_info->setResizeMode(QListWidget::Adjust);
	ui.list_users_info->setMovement(QListWidget::Static);

	ui.button_modify_user->setEnabled(false);   //δѡ���û�ʱ������ɾ���û����޸��û���ť������
	ui.button_delete_user->setEnabled(false);

	ui.button_library_add->setIcon(QIcon("./Resources/icon/add_lib.jpg"));
	ui.button_library_del->setIcon(QIcon("./Resources/icon/del_lib.jpg"));
	ui.button_library_edit->setIcon(QIcon("./Resources/icon/edit_lib.jpg"));
	ui.button_library_refresh->setIcon(QIcon("./Resources/icon/refresh_lib.jpg"));

	ui.label_detect_result->setText(QString::fromLocal8Bit("���������"));
	ui.label_recognize_result->setText(QString::fromLocal8Bit("����ʶ����"));
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

	//�����л��û���ť
	connect(ui.button_change_account, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�л��û�");
		//ui.list_library_app->clear();
		if (display_detect_result_ != NULL)
			display_detect_result_->close();
		emit BackLogin();
	});

	//���ӹر�ϵͳ��ť
	connect(ui.button_close_system, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�ر�ϵͳ");
		if (display_detect_result_ != NULL)
			display_detect_result_->close();
		this->close();
	});

	//�����������л���ť
	connect(ui.button_online_monitoring, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("���߼��");
		ui.button_online_monitoring->setStyleSheet("background-color:white");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(3);
	});

	connect(ui.button_camera_management, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�������");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:white");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(0);
	});

	connect(ui.button_library_management, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�����");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:white");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(2);
	});

	connect(ui.button_snap_query, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("ץ�Ĳ�ѯ");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:white");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(4);
	});

	connect(ui.button_track, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("���ٹ켣");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:white");
		ui.button_intelligence_test->setStyleSheet("background-color:blue");

		ui.stacked_main_funcation->setCurrentIndex(5);
	});

	connect(ui.button_intelligence_test, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("���ܲ���");
		ui.button_online_monitoring->setStyleSheet("background-color:blue");
		ui.button_camera_management->setStyleSheet("background-color:blue");
		ui.button_library_management->setStyleSheet("background-color:blue");
		ui.button_snap_query->setStyleSheet("background-color:blue");
		ui.button_track->setStyleSheet("background-color:blue");
		ui.button_intelligence_test->setStyleSheet("background-color:white");

		ui.stacked_main_funcation->setCurrentIndex(1);
	});

/*************************************************************************************************/
/*�����ԱȲ��ֹ���ʵ�֣��ź����Ӳۺ���*/
	//�ϴ���һ�ŶԱ�ͼƬ
	connect(ui.button_upload_image1, &QPushButton::clicked, [=]() {
		compare_image1_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͼƬ"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));

		QPixmap pix;
		Geometric_Scaling_Image(compare_image1_path_, ui.label_compare_image1->width(), ui.label_compare_image1->height(), pix);
		ui.label_compare_image1->setPixmap(pix);
		ui.button_compare_result->setText(" ");
	});

	//�ϴ��ڶ��ŶԱ�ͼƬ
	connect(ui.button_upload_image2, &QPushButton::clicked, [=]() {
		compare_image2_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͼƬ"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));

		QPixmap pix;
		Geometric_Scaling_Image(compare_image2_path_, ui.label_compare_image2->width(), ui.label_compare_image2->height(), pix);
		ui.label_compare_image2->setPixmap(pix);
		ui.button_compare_result->setText(" ");
	});

	//�����ԱȰ�ť
	connect(ui.button_compare_task, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�����ȶ�");
		IplImage* img1 = cvLoadImage(compare_image1_path_.toLocal8Bit());
		if (img1 == NULL)
		{
			msg_box_.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ƬһΪ�գ�"));
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
			msg_box_.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��Ƭ��Ϊ�գ�"));
			return;
		}
		ASF_MultiFaceInfo faceDetectResult2;
		res = face_opreation_->FacesDetectTask(img2, &faceDetectResult2);
		res = face_opreation_->FaceFeatureExtract(img2, faceDetectResult2, &feature);

		MFloat confidenceValue;
		res = face_opreation_->FaceCompareTask(feature, feature_copy, &confidenceValue);

		QString str = QString("%1:%2%").arg(QString::fromLocal8Bit("���ƶ�")).arg(QString::number(confidenceValue * 100, 'f', 2));
		
		ui.button_compare_result->setStyleSheet("background-color:white;color:red");
		ui.button_compare_result->setText(str);
		free(feature_copy.feature);
		cvReleaseImage(&img1);
		cvReleaseImage(&img2);
	});
/**************************************************************************************************/


/*==========================================================================================================================*/
/*������ⲿ�ֹ���ʵ�֣��ź����Ӳۺ���*/
	//�ϴ��������ͼƬ
	connect(ui.button_upload_detect_image, &QPushButton::clicked, [=]() {
		detect_image_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͼƬ"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));
		QPixmap pix;
		Geometric_Scaling_Image(detect_image_path_, ui.label_detect_image->width(), ui.label_detect_image->height(), pix);
		//pix.load(detect_image_path_);
		ui.label_detect_image->setPixmap(pix);

		ui.tree_detect_result->clear();
		ui.button_display_result->setEnabled(false);
	});

	//������ⰴť
	connect(ui.button_detect_task, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�������");

		ui.tree_detect_result->clear();

		if (display_detect_result_ != NULL)
		{
			delete display_detect_result_;
			display_detect_result_ = NULL;
		}

		IplImage* img = cvLoadImage(detect_image_path_.toLocal8Bit());
		if (img == NULL)
		{
			msg_box_.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ƬΪ�գ�"));
			return;
		}

		FaceDetectResult face_detect_result;
		int res = face_opreation_->FacesDetectTask(img, &face_detect_result, face_detect_result_image_);

		ui.tree_detect_result->setHeaderLabels(QStringList()<< QString::fromLocal8Bit("����")<< QString::fromLocal8Bit("��ϸ��Ϣ"));

		QTreeWidgetItem *num_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("��������") 
			                                         << QString::number(face_detect_result.detectInfo.faceNum));
		ui.tree_detect_result->addTopLevelItem(num_item);

		for (int i = 0; i < face_detect_result.detectInfo.faceNum; i++)
		{
			//������Ϣ���ڵ�
			QString str = QString("%1%2%3").arg(QString::fromLocal8Bit("����")).arg(QString::number(i + 1)).arg(QString::fromLocal8Bit("��Ϣ"));
			QTreeWidgetItem *info_item = new QTreeWidgetItem(QStringList() << str);
			ui.tree_detect_result->addTopLevelItem(info_item);

			//�Ա�
			QString sex_str;
			if (face_detect_result.genderInfo.genderArray[i] == 0)
			{
				sex_str = QString("%1").arg(QString::fromLocal8Bit("��"));
			}
			else if (face_detect_result.genderInfo.genderArray[i] == 1)
			{
				sex_str = QString("%1").arg(QString::fromLocal8Bit("Ů"));
			}
			else
			{
				sex_str = QString("%1").arg(QString::fromLocal8Bit("δ֪"));
			}
			QTreeWidgetItem *sex_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("�Ա�") << sex_str);
			info_item->addChild(sex_item);

			//������Ϣ
			QTreeWidgetItem *age_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("����") 
				                                         << QString::number(face_detect_result.ageInfo.ageArray[i]));
			info_item->addChild(age_item);

			//����λ����Ϣ
			QTreeWidgetItem *location_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("����λ��"));
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

			//����3D��Ϣ
			QTreeWidgetItem *angle_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("3D��Ϣ"));
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

	//�鿴���������
	connect(ui.button_display_result, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�鿴���");

		if (display_detect_result_ == NULL)
		{
			display_detect_result_ = new DisplayDetectResult(face_detect_result_image_);
		}
		display_detect_result_->show();
	});
/*==========================================================================================================================*/


	//�ϴ�����ʶ��ͼƬ
	connect(ui.button_upload_recognize_image, &QPushButton::clicked, [=]() {
		recognize_image_path_ = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("ѡ��ͼƬ"), OPEN_IMAGE_PATH,
			tr("Images (*.png *.jpg);; All files (*.*)"));

		QPixmap pix;
		Geometric_Scaling_Image(recognize_image_path_, ui.label_reconige_image->width(), ui.label_reconige_image->height(), pix);
		ui.label_reconige_image->setPixmap(pix);
	});

	//����ʶ��ť
	connect(ui.button_recognize_task, &QPushButton::clicked, [=]() {
		IplImage* img = cvLoadImage(recognize_image_path_.toLocal8Bit());

		face_opreation_->faceRecognitionResult.clear();

		cv::Mat dst;
		AllFacesFeature all_face_feature;
		//face_opreation_->FaceFeatureExtract(img, dst, all_face_feature);
		face_opreation_->FaceCompareTask(&all_face_feature, &sqlite_operation_->memery_user_feature_);


		ui.tree_recognize_result->clear();
		ui.tree_recognize_result->setHeaderLabels(QStringList() << QString::fromLocal8Bit("����") << QString::fromLocal8Bit("��ϸ��Ϣ"));

		QTreeWidgetItem *num_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("��������")
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
			//������Ϣ���ڵ�
			QString str = QString("%1%2%3").arg(QString::fromLocal8Bit("����")).arg(QString::number(i + 1)).arg(QString::fromLocal8Bit("��Ϣ"));
			QTreeWidgetItem *info_item = new QTreeWidgetItem(QStringList() << str);
			ui.tree_recognize_result->addTopLevelItem(info_item);

			//����
			QTreeWidgetItem *name_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("����") 
				<< QString::fromLocal8Bit(it->user_info.user_name));
			info_item->addChild(name_item);

			//�Ա�
			QTreeWidgetItem *sex_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("�Ա�") 
				<< QString::fromLocal8Bit(it->user_info.user_sex));
			info_item->addChild(sex_item);

			//����
			QTreeWidgetItem *age_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("����")
				<< QString::number(it->user_info.user_age));
			info_item->addChild(age_item);

			//��ϵ��ʽ
			QTreeWidgetItem *phone_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("��ϵ��ʽ")
				<< QString::fromLocal8Bit(it->user_info.phone_num));
			info_item->addChild(phone_item);

			//��ַ
			QTreeWidgetItem *address_item = new QTreeWidgetItem(QStringList() << QString::fromLocal8Bit("��ַ")
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

	//��ʼ������ʶ������
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

	if (current_account_info_->account_type == 1)   //��ͨ�û�
	{
		QIcon icon("./Resources/icon/user.jpg");
		ui.button_user_account_name->setIconSize(QSize(35, 35));
		ui.button_user_account_name->setIcon(icon);
	}
	else if (current_account_info_->account_type == 2)  //����Ա
	{
		QIcon icon("./Resources/icon/admin.jpg");
		ui.button_user_account_name->setIconSize(QSize(35, 35));
		ui.button_user_account_name->setIcon(icon);
	}
	

/************************************************************************************************************************/
/*�������ݿ⣬�û��������ز���*/

	//��������������ݿⰴť
	connect(ui.button_library_add, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("���������");

		if (face_app_library_ == NULL)
		{
			UserApp user_app;
			face_app_library_ = new FaceAppLibrary(ADD_LIBRARY, user_app);
		}
		face_app_library_->show();

		//����������������ź�
		connect(face_app_library_, &FaceAppLibrary::AppLibraryInfo, [=](int operation, UserApp user_app) {
			if (operation == ADD_LIBRARY)  //�����µ��û���
			{
				msg_box_.information(face_app_library_, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����û���ɹ���"));
				sqlite_operation_->CreateUserApp(user_app); //д���ݿ�
				sqlite_operation_->UpdateMemeryUserApp();   //�����ڴ�����������Ϣ
				RecodeUserAppMap();                         //����ListWidget����ʾ��������Ϣ
				RefreshListWidget();                        //ˢ��ListWidget��������Ϣ
			}
		});

		//���ӷ����������ź�
		connect(face_app_library_, &FaceAppLibrary::BackMainInterface, [=]() {
			if (face_app_library_ != NULL)
			{
				delete face_app_library_;
				face_app_library_ = NULL;
			}
		});
	});

	//���ӱ༭�������ݿⰴť
	connect(ui.button_library_edit, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�༭������");

		int row = ui.list_library_app->currentRow();

		std::map<int, UserApp>::iterator it = map_user_app_recode_.find(row);

		if (face_app_library_ == NULL)
		{
			UserApp user_app;
			face_app_library_ = new FaceAppLibrary(MODIFY_LIBRARY, it->second);
		}
		face_app_library_->show();

		//�༭�������ź�
		connect(face_app_library_, &FaceAppLibrary::AppLibraryInfo, [=](int operation, UserApp user_app) {
			if (operation == MODIFY_LIBRARY)
			{
				msg_box_.information(face_app_library_, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�޸��û���ɹ���"));
				sqlite_operation_->ModifyUserAppByid(it->second.app_id, user_app);
				sqlite_operation_->UpdateMemeryUserApp();
				RecodeUserAppMap();
				RefreshListWidget();
			}

		});

		//���ӷ����������ź�
		connect(face_app_library_, &FaceAppLibrary::BackMainInterface, [=]() {
			if (face_app_library_ != NULL)
			{
				delete face_app_library_;
				face_app_library_ = NULL;
			}
		});		
	});

	//����ɾ�������ⰴť
	connect(ui.button_library_del, &QPushButton::clicked, [=]() {
		int row = ui.list_library_app->currentRow();

		if (row == -1 || current_app_id_ == 0)
		{
			msg_box_.critical(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("δѡ���û��飡"));
			return;
		}

		int index = QMessageBox::question(this, QString::fromLocal8Bit("��ʾ"),
			QString::fromLocal8Bit("ȷ��ɾ���û���'%1'��ȫ���û���Ϣ?\n�˲������ɻָ���").arg(QString::fromLocal8Bit(map_user_app_recode_[row].app_name)),
			QString::fromLocal8Bit("ȷ��"),
			QString::fromLocal8Bit("ȡ��"), "", 0, 0);

		if (index != 0)
			return;

		sqlite_operation_->DeleteUserAppByid(map_user_app_recode_[row].app_id);
		sqlite_operation_->UpdateMemeryUserApp();
		RecodeUserAppMap();
		RefreshListWidget();
	});

	//����ˢ�¿��б�ť
	connect(ui.button_library_refresh, &QPushButton::clicked, [=]() {
		sqlite_operation_->UpdateMemeryUserApp();
		//RecodeUserAppMap();
		RefreshListWidget();
		ClearUserListWidget();
		DisplayCurrentAppUserInfo(0);
	});

	//���ӵ���û����ź�
	connect(ui.list_library_app, &QListWidget::itemClicked, [=]() {
		qDebug() << QString::fromLocal8Bit("���ListWidget����ǰ������") << ui.list_library_app->currentRow();

		ClearUserListWidget();
		current_app_id_ = map_user_app_recode_[ui.list_library_app->currentRow()].app_id;
		DisplayCurrentAppUserInfo(current_app_id_);
	});

/************************************************************************************************************************/


/*=======================================================================================================================*/
/*�������ݿ⣬�û�������ز���*/

	//����������û���ť
	connect(ui.button_add_user, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("������û�");
		AddNewUser();
	});

	//���ӱ༭�û���Ϣ��ť
	connect(ui.button_modify_user, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("�༭�û���Ϣ");
		if (current_user_id_ == 0)
		{
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("δѡ���û���"));
			return;
		}
		if (vector_delete_select_user.size() > 1)
		{
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ѡ���û�����û���"));
			return;
		}
		ModifyUserInfo(current_user_id_);
		ui.button_modify_user->setEnabled(false);
		ui.button_delete_user->setEnabled(false);
		vector_delete_select_user.clear();

	});

	//����ˢ���û���Ϣ��ť
	connect(ui.button_update_users, &QPushButton::clicked, [=]() {
		ClearUserListWidget();
		DisplayCurrentAppUserInfo(current_app_id_);
	});

	//��������ɾ����ť
	connect(ui.button_delete_user, &QPushButton::clicked, [=]() {
		qDebug() << QString::fromLocal8Bit("����ɾ��");

		DeleteMultiUserInfo();
		ui.button_modify_user->setEnabled(false);
		ui.button_delete_user->setEnabled(false);
		vector_delete_select_user.clear();
	});

/*
	QString dir_path = QFileDialog::getExistingDirectory(this, QString::fromLocal8Bit("ѡ��Ŀ¼"), "D:\\test_image", QFileDialog::ShowDirsOnly);

		QDir dir(dir_path);

		QStringList ImageList;

		ImageList << "*.bmp" << "*.jpg" << "*.png";//���ַ����б����ͼƬ����

		dir.setNameFilters(ImageList);//����ļ�����ͼƬ������

		int ImageCount = dir.count();//���dir�����ֵĸ�����Ҳ��ʾ�ļ�����ͼƬ�ĸ���

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

			if (ratio_w >= ratio_h)  //ͼƬ���ճ������ţ����ֳ����
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
	//���ӵ���û��ź�
	connect(ui.list_users_info, &QListWidget::itemClicked, [=]() {
		qDebug() << QString::fromLocal8Bit("���ListWidget����ǰ������") << ui.list_users_info->currentRow();

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
	//ָ������
	painter.drawPixmap(0, 0, this->width(), this->height(), pix);
	//�ڱ����ϼ�ͼ�걳��
	pix.load("./Resources/icon/logo.png");
	////����
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

	//�����������
	connect(user_info_icon, &UserInfoIcon::AddUserFaceImage, [=](int user_id) {
		qDebug() << QString::fromLocal8Bit("����������û�id:") << user_id;
		AddFaceToUser(user_id);
	});

	//����ɾ���û�
	connect(user_info_icon, &UserInfoIcon::DelUserAllInfo, [=](int user_id) {
		qDebug() << QString::fromLocal8Bit("ɾ���û����û�id") << user_id;
		DeleteUserInfo(user_id);
	});

	//����--ѡ��״̬��ť
	connect(user_info_icon, &UserInfoIcon::SelectUser, [=](int state, int user_id) {
		if (state == Qt::Checked)
		{
			qDebug() << QString::fromLocal8Bit("ѡ���û�") << user_id << "    row:" << ui.list_users_info->currentRow();
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

	//���ѡ���û�
	connect(user_info_icon, &UserInfoIcon::PressUser, [=](int user_id) {
		qDebug() << QString::fromLocal8Bit("�ؼ��¼�,��ǰѡ���û���") << user_id;
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

	//�����½��û��ź�
	connect(face_user_library_, &FaceUserLibrary::ModifyUser, [=](UserInfo user_info) {
		qDebug() << QString::fromLocal8Bit("ȷ���޸��û���Ϣ");

		
		sqlite_operation_->ModifyUserInfoByid(user_info);  //д���ݿ�

		msg_box_.information(face_user_library_, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("�޸��û���Ϣ�ɹ���"));
		
		//����û�ԭ����Ϣ
		//sqlite_operation_->memery_user_main_info_.erase(itor);
		itor->user_info = user_info;

		ClearUserListWidget();
		DisplayCurrentAppUserInfo(current_app_id_);

		//��ʾ������û����������ڴ�������
		//DisplayUserListWidget(user_info, image_save_path);

		//UserMainInfo new_user_main_ifo;
		//new_user_main_ifo.user_info = user_info;
		//new_user_main_ifo.user_info.user_id = current_user_id_;

		//strcpy(new_user_main_ifo.app_name, map_user_app_recode_[ui.list_library_app->currentRow()].app_name);
		//strcpy(new_user_main_ifo.url, image_save_path);
		//sqlite_operation_->memery_user_main_info_.push_back(new_user_main_ifo);

	});

	//���ӷ����������ź�
	connect(face_user_library_, &FaceUserLibrary::BackMainInterface, [=]() {
		qDebug() << QString::fromLocal8Bit("�˳�������û�");

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
		msg_box_.critical(face_user_library_, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("δѡ���û��飡"));
		return;
	}

	UserInfo user_info;
	if (face_user_library_ == NULL)
	{
		face_user_library_ = new FaceUserLibrary(ADD_USER, user_info);
	}
	face_user_library_->show();

	//�����½��û��ź�
	connect(face_user_library_, &FaceUserLibrary::AddUserInfo, [=](UserInfo user_info, QString image_path) {
		qDebug() << QString::fromLocal8Bit("ȷ��������û�");

		IplImage* img = cvLoadImage(image_path.toLocal8Bit());
		if (img == NULL)
		{
			face_user_library_->click_count_--;
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ȡͼƬʧ�ܣ�"));
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
			msg_box_.information(face_user_library_, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("������û��ɹ���"));
		}
		else
		{
			msg_box_.information(face_user_library_, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("���û�����ӣ�"));
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

		//��ʾ������û����������ڴ�������
		DisplayUserListWidget(user_info, image_save_path);

		UserMainInfo new_user_main_ifo;
		new_user_main_ifo.user_info = user_info;
		new_user_main_ifo.user_info.user_id = current_user_id_;

		strcpy(new_user_main_ifo.app_name, map_user_app_recode_[ui.list_library_app->currentRow()].app_name);
		strcpy(new_user_main_ifo.url, image_save_path);
		sqlite_operation_->memery_user_main_info_.push_back(new_user_main_ifo);

	});

	//���ӷ����������ź�
	connect(face_user_library_, &FaceUserLibrary::BackMainInterface, [=]() {
		qDebug() << QString::fromLocal8Bit("�˳�������û�");

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

	//�����½��û��ź�
	connect(face_user_library_, &FaceUserLibrary::AddUserFace, [=](QString image_path) {
		qDebug() << QString::fromLocal8Bit("ȷ������û�����");

		IplImage* img = cvLoadImage(image_path.toLocal8Bit());
		if (img == NULL)
		{
			face_user_library_->click_count_--;
			msg_box_.critical(face_user_library_, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��ȡͼƬʧ�ܣ�"));
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
		msg_box_.information(face_user_library_, QString::fromLocal8Bit("��ʾ"), QString::fromLocal8Bit("����û������ɹ���"));

	});

	//���ӷ����������ź�
	connect(face_user_library_, &FaceUserLibrary::BackMainInterface, [=]() {
		qDebug() << QString::fromLocal8Bit("�˳�����û�����");

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
