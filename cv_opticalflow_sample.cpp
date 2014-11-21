//
// cv_opticalflow_sample.cpp - OpenCV���g�����I�v�e�B�J���t���[�̊ȒP�ȃT���v��
//
// �Q�l : 
//     OpenCV2.4.10/sources/samples/cpp/fback.cpp
//     OpenCV2.4.10/sources/samples/cpp/simpleflow_demo.cpp
//     OpenCV2.4.10/sources/samples/cpp/tvl1_optical_flow.cpp
//     OpenCV2.4.10/sources/samples/gpu/optical_flow.cpp
//     OpenCV2.4.10/sources/samples/gpu/farneback_optical_flow.cpp
//     OpenCV2.4.10/sources/samples/gpu/pyrlk_optical_flow.cpp
//     OpenCV2.4.10/sources/samples/ocl/pyrlk_optical_flow.cpp
//     OpenCV2.4.10/sources/samples/ocl/tvl1_optical_flow.cpp
//
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/superres/optical_flow.hpp>

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "opencv_core2410d.lib")
#pragma comment(lib, "opencv_imgproc2410d.lib")
#pragma comment(lib, "opencv_highgui2410d.lib")
#pragma comment(lib, "opencv_superres2410d.lib")
#else
#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_imgproc2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#pragma comment(lib, "opencv_superres2410.lib")
#endif
#endif

#define CAPTURE() {                                             \
	cv::Size size(320, 240);                                    \
	old_frame = now_frame;                                      \
	capture >> now_frame;                                       \
	/* �I�v�e�B�J���t���[�̌v�Z�͏d���̂ŉ摜�����������Čv�Z*/ \
	cv::resize(now_frame, now_frame, size);                     \
}

int main(int argc, char* argv[])
{	
	cv::Mat old_frame, now_frame;
	cv::Mat canvas(cv::Size(640, 480), CV_8UC3);

	cv::VideoCapture capture;	
	if (capture.open(0) == false) {
		printf("error : capture.open() failed...\n");
		return -1;
	}

	cv::Mat_<cv::Point2f> flow;
	cv::Ptr<cv::superres::DenseOpticalFlowExt> optical_flow;

	// createOptFlow_???�֐���Simple, Farneback, DualTVL1�Ȃǉ���ނ�����̂ŁA
	// �g����֐��ɂ��Ă�optical_flow.hpp���Q�Ƃ̂��ƁB
	// cv::Ptr<DenseOpticalFlowExt>�Ŏ󂯂�K�v������̂Œ��ӁB
	optical_flow = cv::superres::createOptFlow_Farneback();

	// 1�t���[���O�̉摜���K�v�Ȃ̂Œ���
	CAPTURE();

	while(true) {
		// �L���v�`��
		CAPTURE();

		// �I�v�e�B�J���t���[�̌v�Z
		optical_flow->calc(old_frame, now_frame, flow);

		// ���ʂ̕\��
		cv::resize(now_frame, canvas, canvas.size());

		int scale = canvas.cols / now_frame.cols;

		for (int y = 0; y < flow.rows; y += 10) {
			for (int x = 0; x < flow.cols; x += 10) {
				// �t���[�̕������擾
				cv::Point2f v = flow.at<cv::Point2f>(y, x); // at()���g���Ƃ���x,y�t�Ȃ̂Œ���

				// canvas�̃T�C�Y�ɂ��킹�ă��T�C�Y���Ă���
				cv::Point p0(x * scale, y * scale);
				cv::Point p1((x + (int)v.x) * scale, (y + (int)v.y) * scale);

				// �t���[�̕�����canvas�ɕ`��
				cv::line(canvas, p0, p1, CV_RGB(0,255,0), 1);
				cv::circle(canvas, p0, 1, CV_RGB(255,255,0), CV_FILLED);
			}
		}

		cv::resize(canvas, canvas, cv::Size(640, 480));

		cv::imshow("optical_flow", canvas);

		int c = cv::waitKey(1);
		if (c == 27) break;
	}

	capture.release();
	cv::destroyAllWindows();

	return 0;
}

