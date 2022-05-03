#include "vistool.h"
#include <ctime>
#include <map>
#include <opencv/highgui.h>

using namespace ND;

void Vistool::Init(){
	if (cfg_.BEV_CLASS_NUMS == 1) {
		cls_colors_->insert(std::make_pair(0, cfg_.DEFAULT_CLASS_COLOR));
	}
	else {
		for (int i = 0; i < cfg_.BEV_CLASS_NUMS; i++) {
			// TODO: Real Color Gen functions 
			cls_colors_->insert(std::make_pair(i, cfg_.DEFAULT_CLASS_COLOR));
		}
	}
}

void Vistool::Init(int class_nums){
	cfg_.BEV_CLASS_NUMS = class_nums;
	Init();
};

void Vistool::Init(int class_nums, std::string title){
	cfg_.BEV_TITLE = title; 
	cfg_.BEV_CLASS_NUMS = class_nums;
	Init();
};

void Vistool::Init(int class_nums, int bev_height, int bev_width, float m_scale, std::string title){
	cfg_.M_SCALE = m_scale;
	cfg_.BEV_CLASS_NUMS = class_nums;
	Init();
};


cv::Scalar Vistool::genObjectColor(int class_id) {
	std::map<int, cv::Scalar>::iterator it;
	it = cls_colors_->find(class_id);
	if (it != cls_colors_->end()) {
		return it->second;
	} else {
		return cfg_.DEFAULT_CLASS_COLOR;
	}
}

int Vistool::getGridIdNum(int idx) {
}

// ROOT Layout
void Vistool::cvGetRootLayout(cv::Mat& Im_, int frame_) {
	int img_w = Im_.cols;
	int img_h = Im_.rows;
	int ox = (int)(img_w/2);
	int oy = (int)(img_h/2);
	float x_distance = img_w * cfg_.M_SCALE;
	float y_distance = img_h * cfg_.M_SCALE;

	std::string t_frame = "Frame: ";
	t_frame += std::to_string(frame_);
	cv::putText(Im_, t_frame, cv::Point(50, 40), cfg_.DFONT, cfg_.FONT_SMALL, cfg_.TITLE_COLOR);

	//draw x and y axis 																										
	cv::line(Im_, cv::Point(ox, oy + 50), cv::Point(img_w / 2, img_h / 2 - 100), cfg_.AXIS_COLOR);
	cv::putText(Im_, "X", cv::Point(ox, oy - 100), cfg_.DFONT, cfg_.FONT_BIG, cfg_.AXIS_COLOR);
	cv::line(Im_, cv::Point(ox - 90, oy), cv::Point((img_w / 2) + 90, img_h / 2 ), cfg_.AXIS_COLOR);
	cv::putText(Im_, "Y", cv::Point((ox - 10) + 90, oy ), cfg_.DFONT, cfg_.FONT_BIG, cfg_.AXIS_COLOR);

	// draw grid
	int step = 50;
	for (int u = 1; u < ((img_w) / step); u++)
	{
		cv::line(Im_, cv::Point(u * step, 0), cv::Point(u * step, img_h), cfg_.GRID_COLOR);
		auto grid_x_id = std::to_string((int(x_distance - (u * step * cfg_.M_SCALE))));
		auto pu = cv::Point((u * step) + 5, img_h-10);
		cv::putText(Im_, grid_x_id, pu, cfg_.DFONT, cfg_.TXT_SIZE, cfg_.GRID_MESH_COLOR);
	}

	for (int v = 1; v < ((img_h) / step); v++)
	{
		cv::line(Im_, cv::Point(0, v * step), cv::Point(img_w, v * step), cfg_.GRID_COLOR);
		auto grid_y_id = std::to_string(int(y_distance - (v * step * cfg_.M_SCALE)));
		auto pv = cv::Point(5, (v * step) - 5);
		cv::putText(Im_, grid_y_id, pv, cfg_.DFONT, cfg_.TXT_SIZE, cfg_.GRID_MESH_COLOR);
	}
}

void Vistool::drawObstacle(cv::Mat& Im_, const bev_obstacle_t& obstacle_) {
	int img_w = Im_.cols;
	int img_h = Im_.rows;

	float px = obstacle_.x;
	float py = obstacle_.y;
	float w = obstacle_.w;
	float l = obstacle_.l;
	float theta = obstacle_.theta;
	float id_ = obstacle_.id_;
	int cid = obstacle_.class_id;

	float t_u = img_w / 2 + (py / cfg_.M_SCALE);
	float t_v = img_h / 2 - (px / cfg_.M_SCALE);
	float pw = w / cfg_.M_SCALE;
	float pl = l / cfg_.M_SCALE;

	cv::RotatedRect rRect = cv::RotatedRect(cv::Point2f(t_u, t_v), cv::Size2f(pw, pl), theta);
	auto objColor = genObjectColor(cid);

	// method 1 draw rect line, rect only
	cv::Point2f vertices[4];
	rRect.points(vertices);
	for (int i = 0; i< 4; i++) {
		cv::line(Im_, vertices[i], vertices[(i+1)%4], objColor);
	}

	// TODO: method 2 draw polygon option

	// draw oid
	std::string oid = std::to_string((int)id_);
	auto txt_point = cv::Point2f(t_u, t_v - 20);
	cv::putText(Im_, oid, txt_point, cfg_.DFONT, cfg_.TXT_SIZE, objColor);
}


Vistool::Vistool()
{

}

Vistool::~Vistool()
{

}

void Vistool::cvDisplay(int frame)
{
	cv::Mat bevImg(cfg_.BEV_IMG_HEIGHT, cfg_.BEV_IMG_WIDTH, CV_8UC3, cfg_.ROOT_COLOR);
	cvGetRootLayout(bevImg, frame);

	for (int i = 0; i < obstacles->size(); i++){
		drawObstacle(bevImg, obstacles->data()[i]);
	}

	cvGUIMtx_.lock();
	cv::imshow(cfg_.BEV_TITLE, bevImg);
	cv::waitKey(5);
	cvGUIMtx_.unlock();
}

void Vistool::cvSaveImage(int frame) {
	cv::Mat bevImg(cfg_.BEV_IMG_HEIGHT, cfg_.BEV_IMG_WIDTH, CV_8UC3, cfg_.ROOT_COLOR);
	cvGetRootLayout(bevImg, frame);

	for (int i = 0; i < obstacles->size(); i++){
		drawObstacle(bevImg, obstacles->data()[i]);
	}

	std::string img_tilte = "BEV_IMG_";
	img_tilte += cfg_.BEV_TITLE;
	img_tilte += "_";
	img_tilte += std::to_string(frame);
	img_tilte += ".jpeg";
	cv::imwrite(img_tilte, bevImg);
}

void Vistool::cvSaveVideo(int frame) {
	if (nullptr == videoWriter) {
		std::string avi_fp = "BEV_VEDIO_";
		avi_fp += cfg_.BEV_TITLE;
		avi_fp += ".avi";
		videoWriter = std::make_shared<cv::VideoWriter>(avi_fp, cfg_.VIDEO_CODEC, cfg_.VIDEO_FPS, cv::Size(cfg_.BEV_IMG_WIDTH, cfg_.BEV_IMG_HEIGHT));
	}

	cv::Mat bevImg(cfg_.BEV_IMG_HEIGHT, cfg_.BEV_IMG_WIDTH, CV_8UC3, cfg_.ROOT_COLOR);
	cvGetRootLayout(bevImg, frame);
	for (int i = 0; i < obstacles->size(); i++){
		drawObstacle(bevImg, obstacles->data()[i]);
	}

	videoWriter->write(bevImg);
}
