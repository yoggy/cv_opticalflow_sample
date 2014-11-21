cv_opticalflow_sample.cpp
====
OpenCVを使ったオプティカルフローの簡単なサンプル

<pre>
#define CAPTURE() {                                             \
    cv::Size size(320, 240);                                    \
    old_frame = now_frame;                                      \
    capture >> now_frame;                                       \
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

    optical_flow = cv::superres::createOptFlow_Farneback();

    CAPTURE();

    while(true) {
        CAPTURE();
        optical_flow->calc(old_frame, now_frame, flow);

        cv::resize(now_frame, canvas, canvas.size());
        int scale = canvas.cols / now_frame.cols;

        for (int y = 0; y < flow.rows; y += 10) {
            for (int x = 0; x < flow.cols; x += 10) {
                cv::Point2f v = flow.at<cv::Point2f>(y, x);
                cv::Point p0(x * scale, y * scale);
                cv::Point p1((x + (int)v.x) * scale, (y + (int)v.y) * scale);
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
</pre>
