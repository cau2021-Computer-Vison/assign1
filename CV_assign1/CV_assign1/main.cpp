/**
  @file videocapture_basic.cpp
  @brief A very basic sample for using VideoCapture and VideoWriter
  @author PkLab.net
  @date Aug 24, 2016
*/
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int first_count = 0, second_count = 0;
int img1_x[4], img1_y[4], img2_x[4], img2_y[4];

void onMouse_first(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		Mat& image = *(Mat*)(userdata);
		if (first_count < 4 && x > 18 && y > 18 && x < (image.cols - 18) && y < (image.rows - 18)) {
			rectangle(image, Point(x - 16, y - 16), Point(x + 16, y + 16), Scalar(0));
			img1_x[first_count] = x;
			img1_y[first_count] = y;
			cout << "1�� �׸��� " << first_count << "��° �Է�!" << endl;
			first_count++;
			imshow("first", image);
		}
		else if (first_count < 4) {
			cout << "������ �ʰ��߽��ϴ�!" << endl;
		}
		else {
			cout << "�Է� Ƚ���� �ʰ��߽��ϴ�!" << endl;
		}
	}
}

void onMouse_second(int event, int x, int y, int flags, void* userdata) {
	if (event == EVENT_LBUTTONDOWN) {
		Mat& image = *(Mat*)(userdata);
		if (second_count < 4 && x > 18 && y > 18 && x < (image.cols - 18) && (y < image.rows - 18)) {
			rectangle(image, Point(x - 16, y - 16), Point(x + 16, y + 16), Scalar(0));
			img2_x[second_count] = x;
			img2_y[second_count] = y;
			cout << "2�� �׸��� " << second_count << "��° �Է�!" << endl;
			second_count++;
			imshow("second", image);
		}
		else if (second_count < 4) {
			cout << "������ �ʰ��߽��ϴ�!" << endl;
		}
		else {
			cout << "�Է� Ƚ���� �ʰ��߽��ϴ�!" << endl;
		}
	}
}


Mat CalGredient(Mat ori) { //�׷����Ʈ �� ��� �Լ�
	Mat dx, dy, res, temp, ne, cmp;

	equalizeHist(ori, cmp);
	GaussianBlur(cmp, cmp, Size(3, 3), 0, 0, BORDER_REFLECT);
	Laplacian(cmp, temp, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//convertScaleAbs(temp, res);
	//Mat edge = res - 100;
	//equalizeHist(ori, ori);


	Sobel(temp, dx, CV_32FC1, 1, 0);
	Sobel(temp, dy, CV_32FC1, 0, 1);
	magnitude(dx, dy, ne);
	ne.convertTo(res, CV_8U);
	return res;

	//Canny(ori, res, 50, 255);
	//equalizeHist(res, res);
	//imshow("test", edge);
	//waitKey(0);
	//return res;


}

Mat CalHist(const Mat& image) { // ������׷� �� ��� �Լ�
	//assert(image.type() == CV_8UC1);
	//assert(image.size)

	Mat res, rst; // ����� ��¿� Mat ��ü

	int channels[] = { 0 }; //ä��, �׷��̽����� �����̶� 0
	int dims = 1;//ä�� ����, ���� �׷��̽������̶� 1
	const int histSize[] = { 16 }; //16�� bin���� ���� ����
	float graylevel[] = { 0,256 }; //ȭ���� ����  �ּ� 0, �ִ� 256
	const float* ranges[] = { graylevel };

	calcHist(&image, 1, channels, noArray(), res, dims, histSize, ranges);
	normalize(res, res, 0, 100, NORM_MINMAX);//�ּ� 0, �ִ� 100���η� ����ȭ
	return res;
}

void DrawHist(const Mat& hist, string name) { // ������׷� �׸��� �Լ�

	Mat imgHist(200, 256, CV_8UC3, Scalar(255, 255, 255));

	for (int i = 0; i < 16; i++) {
		line(imgHist, Point(i * 16, 200), Point(i * 16, 200 - cvRound(hist.at<float>(i, 0))), Scalar(255, 0, 255), 4, 8, 0);
	}
	imshow(name, imgHist);
	waitKey(100);
}

void connect_img(float arr1[][16], float arr2[][16], int coord[][4], Mat img1, Mat img2) { //(need: 2 imgs, label coord, 2 arr(histogram))
// cal histogram(need: 2arr(histogram))
	int match[4] = { -1,-1,-1,-1 };
	for (int i = 0; i < 4; i++) {
		float min = 99999.0;
		for (int j = 0; j < 4; j++) {
			float tot = 0.0;
			for (int k = 0; k < 16; k++) {// 16�� ������ ������ ��
				tot += (arr1[i][k] - arr2[j][k]) * (arr1[i][k] - arr2[j][k]);   //������ ���ϴ� ����
			}
			if (min > tot) {
				// ������ �ּҰ� �ȴٸ� match�ȴٰ� �� �� ����
				min = tot;
				match[i] = j;
			}
		}
	}
	/*
	for (int i = 0; i < 4; i++) {
		cout << match[i] << endl;
	}
	*/
	// put labeling into each img(need: label coord)
		// read 2 imgs
	if (!img1.empty() && !img2.empty()) {
		cout << "img1 channel : " << img1.channels() << " Width: " << img1.cols << " Height: " << img1.rows << endl;
		cout << "img2 channel : " << img2.channels() << " Width: " << img2.cols << " Height: " << img2.rows << endl;
		Mat img_result;
		hconcat(img1, img2, img_result);
		for (int i = 0; i < 4; i++) {
			rectangle(img_result, Point(coord[i][0], coord[i][1]), Point(coord[i][2], coord[i][3]), Scalar(0, 0, 255), 8);
			putText(img_result, to_string(i), Point(coord[i][0], coord[i][1]), 1, 8, Scalar(255, 255, 0), 3, 8);
			rectangle(img_result, Point(coord[i + 4][0] + img1.cols, coord[i + 4][1]), Point(coord[i + 4][2] + img1.cols, coord[i + 4][3]), Scalar(0, 0, 255), 8);
			putText(img_result, to_string(i), Point(coord[i + 4][0] + img1.cols, coord[i + 4][1]), 1, 8, Scalar(255, 255, 0), 3, 8);
		}
		// connenct matching point
		for (int i = 0; i < 4; i++) {
			line(img_result, Point(coord[i][0], coord[i][1]), Point(coord[match[i] + 4][0] + img1.cols, coord[match[i] + 4][1]), Scalar(0, 0, 255), 10, 8);
		}
		//resize(img_result, img_result, Size(600, 300), 0, 0, INTER_CUBIC); // �������� ������ ���� ũ�⸦ downsizing�ؼ� �����Ѵ�
		imshow("img_result", img_result);
		imwrite("img_result.jpg", img_result);        // img�� ���Ϸ� �����Ѵ�.
		waitKey(0);
	}
	else {
		cout << "img1 or img2 image read fail";
	}
}

void MatchHistogram(Mat img1, Mat img2) {
	assert(!(img1.empty() || img2.empty())); // �̹��� �˻�
	assert(img1.type() == CV_8U && img2.type() == CV_8U); //�Է¹��� �̹����� 1ä�� �׷��̽�����
	vector<Mat> images_1(4);	// ������ ȭ�� ������ Vector
	vector<Mat> images_2(4);

	namedWindow("first", WINDOW_AUTOSIZE);
	namedWindow("second", WINDOW_AUTOSIZE);

	while (true) {	//�� â���� 4���� ��ǥ �Է�, img �迭�� ����
		imshow("first", img1);
		imshow("second", img2);
		setMouseCallback("first", onMouse_first, &img1);
		setMouseCallback("second", onMouse_second, &img2);

		if (first_count == 4 && second_count == 4) {
			destroyAllWindows();
			break;
		}
		waitKey(100);
	}

	Mat re_img1 = CalGredient(img1);//�̹��� 1,2�� �׷����Ʈ �����Ͽ� ��ü ����
	Mat re_img2 = CalGredient(img2);

	for (int i = 0; i < 4; i++) { //��ǥ ������� Mat �����ؼ� ���Ϳ� ����
		images_1[i] = re_img1(Rect2i(Point(img1_x[i] - 16, img1_y[i] - 16), Point(img1_x[i] + 16, img1_y[i] + 16))).clone();
		images_2[i] = re_img2(Rect2i(Point(img2_x[i] - 16, img2_y[i] - 16), Point(img2_x[i] + 16, img2_y[i] + 16))).clone();
	}

	float arr_img1[4][16], arr_img2[4][16]; //�Լ� �Է¿� �迭
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			arr_img1[i][j] = CalHist(images_1[i]).at<float>(i);
			arr_img2[i][j] = CalHist(images_2[i]).at<float>(i);
		}
	}

	int co_arr[8][4];
	for (int i = 0; i < 4; i++)
	{
		co_arr[i][0] = img1_x[i] - 16;
		co_arr[i][1] = img1_y[i] - 16;
		co_arr[i][2] = img1_x[i] + 16;
		co_arr[i][3] = img1_y[i] + 16;

		co_arr[i + 4][0] = img2_x[i] - 16;
		co_arr[i + 4][1] = img2_y[i] - 16;
		co_arr[i + 4][2] = img2_x[i] + 16;
		co_arr[i + 4][3] = img2_y[i] + 16;
	}
	for (int i = 0; i < 4; i++) {
		DrawHist(CalHist(images_1[i]), "1-" + to_string(i + 1));
	}
	for (int j = 0; j < 4; j++) {
		DrawHist(CalHist(images_2[j]), "2-" + to_string(j + 1));
	}

	connect_img(arr_img1, arr_img2, co_arr, img1, img2);
	return;
}



int main()
{
	Mat first, second;
	first = imread("c:/1st.jpg", IMREAD_GRAYSCALE);
	second = imread("c:/2nd.jpg", IMREAD_GRAYSCALE);
	if (first.empty() || second.empty()) {
		cout << "Image Load Error!" << endl;
	}

	resize(first, first, Size(), 0.25, 0.25);
	resize(second, second, Size(), 0.25, 0.25);

	MatchHistogram(first, second);

	return 0;
}