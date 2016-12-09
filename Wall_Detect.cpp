#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/line_descriptor.hpp"

#include <iostream>

#define HIST_SIZE_X 500
#define HIST_SIZE_Y 600


using namespace cv;
using namespace std;
using namespace line_descriptor;

Ptr<BinaryDescriptor> bd = BinaryDescriptor::createBinaryDescriptor();

bool
isError(Point point)
{
    if ((point.x >= 600) || (point.y <= 0) || (point.y >= 500) || (point.x <= 0) )
        return true;

    return false;
}

void
cloud2map(Mat src)
{
    Point px(0, 0), prePx(0.0);
    Mat hist(HIST_SIZE_X, HIST_SIZE_Y, CV_8UC3, Scalar(255, 255, 255));
    Mat mask    = Mat::ones(hist.size(), CV_8UC1);
    Vec3f point = src.at<Vec3f>(src.cols / 2, src.rows / 2);

    vector<KeyLine> lines;
    cout << point[0] << endl;
    for (int i = 1; i < 640; i++) {
        prePx = px;
        px    = Point(300 + src.at<Vec3f>(src.rows / 2, i)[0] * 50, 500 - src.at<Vec3f>(src.rows / 2, i)[2] * 50);
        if (!(isError(px) || (i == 1)))
            circle(hist, px, 2, Scalar(0, 0, 0), 10, 8, 0);
    }
    bd->detect(hist, lines, mask);
    Mat output = hist.clone();
    /* draw lines extracted from octave 0 */
    if (output.channels() == 1)
        cvtColor(output, output, COLOR_GRAY2BGR);
    for (size_t i = 0; i < lines.size(); i++) {
        KeyLine kl = lines[i];
        if (kl.octave == 0) {
            /* get a random color */
            int R = ( rand() % (int) ( 255 + 1 ) );
            int G = ( rand() % (int) ( 255 + 1 ) );
            int B = ( rand() % (int) ( 255 + 1 ) );

            /* get extremes of line */
            Point pt1 = Point(kl.startPointX, kl.startPointY);
            Point pt2 = Point(kl.endPointX, kl.endPointY);

            /* draw line */
            line(output, pt1, pt2, Scalar(B, G, R), 5);
        }
    }
    imshow("hist", hist);
    imshow("Lines", output);
} // cloud2map

int
main(/*int argc, char* argv[]*/)
{
    Mat world;
    VideoCapture capture;

    capture.open(CV_CAP_OPENNI);
    capture.set(CV_CAP_PROP_OPENNI_REGISTRATION, 0);
    unsigned t0 = clock();
    char inKey;
    if (!capture.isOpened() ) {
        cout << "Can not open a capture object." << endl;
        return -1;
    }
    unsigned elapsed = clock() - t0;
    cout << "initialized in " << elapsed << " s. ready!" << endl;
    namedWindow("depth", 1);
    while (true) {
        if (!capture.grab() ) {
            cout << "Can not grab images." << endl;
            return -1;
        } else {
            capture.retrieve(world, CV_CAP_OPENNI_POINT_CLOUD_MAP);
            cloud2map(world);
            imshow("depth", world);
        }
        inKey = waitKey(34);
        if (inKey == 'q') break;
    }
}
