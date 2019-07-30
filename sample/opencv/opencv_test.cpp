#include <iostream>
#include <fstream>
#include "nvt_type.h"
#include "nvtinfo.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#define OPENCVTEST_GAUSSIAN_IMG_H			1080
#define OPENCVTEST_GAUSSIAN_IMG_W			1920
#define OPENCVTEST_GAUSSIAN_SRC_IMG_FILE	"/mnt/sd/Tests/data/OY0_1080Hx1920W.bin"
#define OPENCVTEST_GAUSSIAN_DST_IMG_FILE	"/mnt/sd/Tests/results/OY0_1080Hx1920W_opencv_gaussian3x3.bin"

#define OPENCVTEST_HOG_IMG_H				128
#define OPENCVTEST_HOG_IMG_W				64
#define OPENCVTEST_HOG_SRC_IMG_FILE			"/mnt/sd/Tests/data/ml_128Hx64W.bin"
#define OPENCVTEST_HOG_WIN_H				40
#define OPENCVTEST_HOG_WIN_W				40
#define OPENCVTEST_HOG_BLK_H				16
#define OPENCVTEST_HOG_BLK_W				16
#define OPENCVTEST_HOG_BLK_STRIDE_H			8
#define OPENCVTEST_HOG_BLK_STRIDE_W			8
#define OPENCVTEST_HOG_CELL_H				8
#define OPENCVTEST_HOG_CELL_W				8
#define OPENCVTEST_HOG_NBINS				9
#define OPENCVTEST_HOG_WIN_STRIDE_H			8
#define OPENCVTEST_HOG_WIN_STRIDE_W			8
#define OPENCVTEST_HOG_PAD_H				0
#define OPENCVTEST_HOG_PAD_W				0

#define OPENCVTEST_AFFINE_IMG_H				128
#define OPENCVTEST_AFFINE_IMG_W				64
#define OPENCVTEST_AFFINE_SRC_IMG_FILE		"/mnt/sd/Tests/data/ml_128Hx64W.bin"
#define OPENCVTEST_AFFINE_DST_IMG_FILE		"/mnt/sd/Tests/results/ml_128Hx64W_opencv_affine.bin"

#define OPENCVTEST_PROFILING				NVT_TRUE

/**
    Structure for rectangle size

    Structure for rectangle size.
*/
typedef struct
{
    UINT32  w;      ///< Rectangle width
    UINT32  h;      ///< Rectangle height
} USIZE, *PUSIZE;

/**
    Structure for point

    Structure for point.
*/
typedef struct
{
    INT32  x;       ///< X coordinate of the point
    INT32  y;       ///< Y coordinate of the point
} IPOINT, *PIPOINT;

#ifndef MIN
#define MIN(a, b)               ((a)>(b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a, b)               ((a)>(b) ? (a) : (b))
#endif

using namespace std;
using namespace cv;

static void opencv_test_smooth_gaussian()
{
	Mat src(OPENCVTEST_GAUSSIAN_IMG_H, OPENCVTEST_GAUSSIAN_IMG_W, CV_8UC1);
	Mat dst(OPENCVTEST_GAUSSIAN_IMG_H, OPENCVTEST_GAUSSIAN_IMG_W, CV_8UC1);
	CvMat cv_src = src;
	CvMat cv_dst = dst;
	ifstream fin;
	ofstream fout;
	INT32 i;

	cout << "Test Gaussian filter ----------" << endl;

	//randu(src, Scalar::all(0), Scalar::all(255));

	try {
		fin.open(OPENCVTEST_GAUSSIAN_SRC_IMG_FILE, ios::binary);
	} catch (ios_base::failure &e) {
		cerr << e.what() << endl;
	}
	for (i = 0; i < OPENCVTEST_GAUSSIAN_IMG_H; i++) {
		fin.read((CHAR*)src.ptr(i), OPENCVTEST_GAUSSIAN_IMG_W);
	}
	fin.close();

#if OPENCVTEST_PROFILING
	int start = nvt_timer_tm0_get();
#endif

	cvSmooth(&cv_src, &cv_dst, CV_GAUSSIAN, 3, 0);

#if OPENCVTEST_PROFILING
	int end = nvt_timer_tm0_get();

	cout << "Start: " << start << endl;
	cout << "End: " << end << endl;
	cout << "Execution Time: " << (end - start) << " (us)" << endl;
#endif

	try {
		fout.open(OPENCVTEST_GAUSSIAN_DST_IMG_FILE, ios::binary);
	} catch (ios_base::failure &e) {
		cerr << e.what() << endl;
	}
	for (i = 0; i < OPENCVTEST_GAUSSIAN_IMG_H; i++) {
		fout.write((CHAR*)dst.ptr(i), OPENCVTEST_GAUSSIAN_IMG_W);
	}
	fout.close();
}

static void opencv_test_hog()
{
	Mat src(OPENCVTEST_HOG_IMG_H, OPENCVTEST_HOG_IMG_W, CV_8UC1);
	ifstream fin;
	HOGDescriptor hog(cvSize(OPENCVTEST_HOG_WIN_H       , OPENCVTEST_HOG_WIN_W),
					  cvSize(OPENCVTEST_HOG_BLK_H       , OPENCVTEST_HOG_BLK_W),
					  cvSize(OPENCVTEST_HOG_BLK_STRIDE_H, OPENCVTEST_HOG_BLK_STRIDE_W),
					  cvSize(OPENCVTEST_HOG_CELL_H      , OPENCVTEST_HOG_CELL_W),
					  OPENCVTEST_HOG_NBINS);
	vector<float> desc;
	UINT32 i;

	cout << "Test HOG ----------" << endl;

	try {
		fin.open(OPENCVTEST_HOG_SRC_IMG_FILE, ios::binary);
	} catch (ios_base::failure &e) {
		cerr << e.what() << endl;
	}
	for (i = 0; i < OPENCVTEST_HOG_IMG_H; i++) {
		fin.read((CHAR*)src.ptr(i), OPENCVTEST_HOG_IMG_W);
	}
	fin.close();

#if OPENCVTEST_PROFILING
	int start = nvt_timer_tm0_get();
#endif

	hog.compute(src, desc,
			cvSize(OPENCVTEST_HOG_WIN_STRIDE_H, OPENCVTEST_HOG_WIN_STRIDE_W),
			cvSize(OPENCVTEST_HOG_PAD_H, OPENCVTEST_HOG_PAD_W));

#if OPENCVTEST_PROFILING
	int end = nvt_timer_tm0_get();

	cout << "Start: " << start << endl;
	cout << "End: " << end << endl;
	cout << "Execution Time: " << (end - start) << " (us)" << endl;
#endif

	/*cout << "Descriptors (" << desc.size() << "):" << endl;
	for (i = 0; i < desc.size(); i++) {
		cout << desc[i] << endl;
	}*/
}

static void opencv_affine_get_map(Mat& map, FLOAT degree, FLOAT scale, UINT32 height, UINT32 width)
{
	UINT32 cx, cy;
	FLOAT rad, alpha, beta;
	FLOAT pi = 3.141592f;

	cx = width >> 1;
	cy = height >> 1;
	rad = degree * pi / 180.0f;
	alpha = scale * cos(rad);
	beta = scale * sin(rad);

	map.at<FLOAT>(0, 0) = alpha;
	map.at<FLOAT>(0, 1) = beta;
	map.at<FLOAT>(0, 2) = cx * (1.0f - alpha) - cy * beta;
	map.at<FLOAT>(1, 0) = -map.at<FLOAT>(0, 1);
	map.at<FLOAT>(1, 1) = map.at<FLOAT>(0, 0);
	map.at<FLOAT>(1, 2) = cx * beta + cy * (1.0f - alpha);
}

static IPOINT opencv_affine_warp_point(INT32 x, INT32 y, const Mat& map)
{
	IPOINT res;
	res.x = (INT32)(map.at<FLOAT>(0, 0) * x + map.at<FLOAT>(0, 1) * y + map.at<FLOAT>(0, 2) + 0.5f);
	res.y = (INT32)(map.at<FLOAT>(1, 0) * x + map.at<FLOAT>(1, 1) * y + map.at<FLOAT>(1, 2) + 0.5f);
	return res;
}

static Size opencv_affine_get_dst_size(const Mat& map, UINT32 height, UINT32 width, UINT32 canvas)
{
	INT32 xmin, ymin, xmax, ymax;
	IPOINT tl, tr, dl, dr;
	Size dst_size;

	if (0 == canvas) {
		xmin = 0;
		ymin = 0;
		xmax = width - 1;
		ymax = height - 1;
	} else {
		tl = opencv_affine_warp_point(0, 0, map);
		tr = opencv_affine_warp_point(width-1, 0, map);
		dl = opencv_affine_warp_point(0, height-1, map);
		dr = opencv_affine_warp_point(width-1, height-1, map);
		xmin = MIN(MIN(tl.x,tr.x), MIN(dl.x,dr.x));
		ymin = MIN(MIN(tl.y,tr.y), MIN(dl.y,dr.y));
		xmax = MAX(MAX(tl.x,tr.x), MAX(dl.x,dr.x));
		ymax = MAX(MAX(tl.y,tr.y), MAX(dl.y,dr.y));
	}
	dst_size.height = ymax - ymin + 1;
	dst_size.width  = xmax - xmin + 1;
	return dst_size;
}

static void opencv_test_affine()
{
	Mat src(OPENCVTEST_AFFINE_IMG_H, OPENCVTEST_AFFINE_IMG_W, CV_8UC1);
	Mat dst(OPENCVTEST_AFFINE_IMG_H, OPENCVTEST_AFFINE_IMG_W, CV_8UC1);
	Mat map(2, 3, CV_32F);
	FLOAT degree = -50.0f, scale = 0.6f;
	UINT32 src_h = OPENCVTEST_AFFINE_IMG_H, src_w = OPENCVTEST_AFFINE_IMG_W;
	UINT32 canvas = 0;
	Size dst_size;
	ifstream fin;
	ofstream fout;
	UINT32 i;

	cout << "Test affine transform ----------" << endl;

	try {
		fin.open(OPENCVTEST_AFFINE_SRC_IMG_FILE, ios::binary);
	} catch (ios_base::failure &e) {
		cerr << e.what() << endl;
	}
	for (i = 0; i < OPENCVTEST_AFFINE_IMG_H; i++) {
		fin.read((CHAR*)src.ptr(i), OPENCVTEST_AFFINE_IMG_W);
	}
	fin.close();

#if OPENCVTEST_PROFILING
	int start = nvt_timer_tm0_get();
#endif

	opencv_affine_get_map(map, degree, scale, src_h, src_w);
	dst_size = opencv_affine_get_dst_size(map, src_h, src_w, canvas);
	warpAffine(src, dst, map, dst_size);

#if OPENCVTEST_PROFILING
	int end = nvt_timer_tm0_get();

	cout << "Start: " << start << endl;
	cout << "End: " << end << endl;
	cout << "Execution Time: " << (end - start) << " (us)" << endl;
#endif

	try {
		fout.open(OPENCVTEST_AFFINE_DST_IMG_FILE, ios::binary);
	} catch (ios_base::failure &e) {
		cerr << e.what() << endl;
	}
	for (i = 0; i < OPENCVTEST_AFFINE_IMG_H; i++) {
		fout.write((CHAR*)dst.ptr(i), OPENCVTEST_AFFINE_IMG_W);
	}
	fout.close();
}

int main()
{
	cout << "Built with OpenCV " << CV_VERSION << endl;
	opencv_test_smooth_gaussian();
	opencv_test_hog();
	opencv_test_affine();
	return 0;
}
