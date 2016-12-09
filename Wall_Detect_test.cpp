#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

void depth2wall(Mat src)
{
	Mat hist(1000,src.cols,CV_8UC3,Scalar(255,255,255));
	int pix =0;
	cout << (uint)(src.at<char>(src.cols/2,src.rows/2)) << endl;
	for (int i = 0; i < src.cols;  i++) {
			pix = src.at<uint>(src.rows/2,i);
			line(hist,Point(i,pix),Point(i,pix),Scalar(0,0,0),1,8,0);
	}
	// imshow("hist",hist);
	// imshow("src",src);
}
int main()
{
	Mat depth,color,filterd;
	VideoCapture capture;
	namedWindow("Depth", WINDOW_AUTOSIZE);
	if(!capture.open(CV_CAP_OPENNI)){
		printf("ERROR: check connection to kinect");
		return -1;
	}
	while(1)
	{
		capture >> depth;
		depth2wall(depth);
		imshow("Color",depth);
		if(waitKey(1) == 'q') break;
	}
	return 0;
}
