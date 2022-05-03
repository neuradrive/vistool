#pragma once

#include <iostream>
#include <stdlib.h>
#include "opencv2/opencv.hpp"
#include "pack.h"
#include <memory>
#include <vector>
#include <string>
#include <mutex>

namespace ND {

	// CV show Mutex
	std::mutex cvGUIMtx_;

	struct bev_params_t {

		// Space config
		float M_SCALE = 0.1; // Measuring Scale
		int BEV_IMG_WIDTH = 1280;
		int BEV_IMG_HEIGHT = 720;
		int BEV_CLASS_NUMS = 1;

		std::string BEV_TITLE = "ND-BEV-PLAYER";

		// font type config
		int DFONT = cv::FONT_HERSHEY_PLAIN;

		// Color config
		cv::Scalar AXIS_COLOR = cv::Scalar(0, 0, 255);
		cv::Scalar GRID_COLOR = cv::Scalar(30, 30, 30);
		cv::Scalar GRID_MESH_COLOR = cv::Scalar(200, 200, 200);
		cv::Scalar ROOT_COLOR = cv::Scalar(0, 0, 0);
		cv::Scalar TITLE_COLOR = cv::Scalar(100, 255, 255);
		cv::Scalar DEFAULT_CLASS_COLOR = cv::Scalar(0, 200, 200);

		float FONT_BIG = 0.9;
		float FONT_SMALL = 0.7;

		// vedio config
		int VIDEO_CODEC =  cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
		int VIDEO_FPS = 30;

		// font size config
		const float AXIS_LINE_SIZE = 0.4f;
		const float TXT_SIZE = 0.6f;
		const float BOX_SIZE = 0.5f;
	};

	class Vistool
	{
	public:

		Vistool();
		~Vistool();

		void Init(int class_nums, int bev_height, int bev_width, float M_SCALE, std::string title);
		void Init(int class_nums,  std::string title);
		void Init(int class_nums);

		// default
		void Init();
		void cvDisplay(int frame);
		void cvSaveVideo(int frame);
		void cvSaveImage(int frame);

	public:
		std::shared_ptr<std::vector<bev_obstacle_t>> obstacles = std::make_shared<std::vector<bev_obstacle_t>>();

	private:
		void cvGetRootLayout(cv::Mat& Im_, int frame_);
		void drawObstacle(cv::Mat& Im_, const bev_obstacle_t& obstacle_);
		cv::Scalar genObjectColor(int class_id);
		int getGridIdNum(int idx);

	private:
		std::shared_ptr<cv::VideoWriter> videoWriter = nullptr;
		std::shared_ptr<std::map<int, cv::Scalar>> cls_colors_ = std::make_shared<std::map<int, cv::Scalar>>();

		bev_params_t cfg_;
	};

}
