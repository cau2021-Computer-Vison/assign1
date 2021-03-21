/**
  @file videocapture_basic.cpp
  @brief A very basic sample for using VideoCapture and VideoWriter
  @author PkLab.net
  @date Aug 24, 2016
*/
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int centcoord[16] = { 16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16 };
int clickcnt = 0;

Mat CalGredient(Mat ori) {
    Mat res, temp;
    GaussianBlur(ori, ori, Size(3, 3), 0, 0, BORDER_REFLECT);
    Laplacian(ori, temp, CV_16S, 3, 1, 0, BORDER_DEFAULT);
    convertScaleAbs(temp, res);
    return res;
}

Mat CalHist(const Mat& image) {
    //assert(image.type() == CV_8UC1);

    Mat res; // ����� ��¿� Mat ��ü
    int channels[] = { 0 }; //ä��, �׷��̽����� �����̶� 0
    int dims = 1;//ä�� ����, ���� �׷��̽������̶� 1
    const int histSize[] = { 16 };
    float graylevel[] = { 0,256 };
    const float* ranges[] = { graylevel };

    calcHist(&image, 1, channels, noArray(), res, dims, histSize, ranges);
    
    return res;
}

void CallBackFunc1(int event, int x, int y, int flags, void* userdata)
{
    
    if (event == EVENT_LBUTTONDOWN)
    {
        cout << "���� ���콺 ��ư Ŭ��.. ��ǥ = (" << x << ", " << y << ")" << " �ּ� 4�� Ŭ���ϰ� �ƹ�Ű�� ��������!(4ȸ ���� Ŭ����ư�� ��ȿȭ�˴ϴ�.)" << endl;
        if (clickcnt < 8) {
            centcoord[clickcnt] = x;
            clickcnt++;
            centcoord[clickcnt] = y;
            clickcnt++;
        }
    }
}
void CallBackFunc2(int event, int x, int y, int flags, void* userdata)
{

    if (event == EVENT_LBUTTONDOWN)
    {
        cout << "���� ���콺 ��ư Ŭ��.. ��ǥ = (" << x << ", " << y << ")" << " �ּ� 4�� Ŭ���ϰ� �ƹ�Ű�� ��������!(4ȸ ���� Ŭ����ư�� ��ȿȭ�˴ϴ�.)"<< endl;
        if (clickcnt < 16) {
            centcoord[clickcnt] = x;
            clickcnt++;
            centcoord[clickcnt] = y;
            clickcnt++;
        }
    }
}
void make_cent_coord1(Mat img) {
    namedWindow("img", WINDOW_AUTOSIZE);
    //�����쿡 ���  
    imshow("img", img);
    //�����쿡 �ݹ��Լ��� ���
    setMouseCallback("img", CallBackFunc1, NULL);
    //16�� �Է��� �ɶ����� ���  
    while (1) {
        waitKey(0);
        if (clickcnt == 8) {
            destroyWindow("img");
            break;
        }
        else {
            cout << "�ּ� 4���̻� Ŭ���� ���ּ���!" << endl;
        }
    }
}
void make_cent_coord2(Mat img) {
    namedWindow("img", WINDOW_AUTOSIZE);
    //�����쿡 ���  
    imshow("img", img);
    //�����쿡 �ݹ��Լ��� ���
    setMouseCallback("img", CallBackFunc2, NULL);
    //16�� �Է��� �ɶ����� ���  
    while (1) {
        waitKey(0);
        if (clickcnt == 16) {
            destroyWindow("img");
            break;
        }
        else {
            cout << "�ּ� 4���̻� Ŭ���� ���ּ���!" << endl;
        }
    }
}

void connect_img(float arr1[][16],float arr2[][16], int coord[][4], Mat img1,Mat img2) {//(need: 2 imgs, label coord, 2 arr(histogram))
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
    for (int i = 0; i < 4; i++) {
        cout << match[i] << endl;
    }
// put labeling into each img(need: label coord)
    // read 2 imgs
    if (!img1.empty() && !img2.empty()) {
        cout << "img1 channel : " << img1.channels() <<" Width: "<< img1.cols << " Height: " << img1.rows << endl;
        cout << "img2 channel : " << img2.channels() << " Width: " << img2.cols << " Height: " << img2.rows << endl;
        Mat img_result;
        hconcat(img1,img2, img_result);
        for (int i = 0; i < 4; i++) {
            rectangle(img_result, Point(coord[i][0], coord[i][1]), Point(coord[i][2], coord[i][3]), Scalar(0, 0, 255), 8);
            putText(img_result, to_string(i), Point(coord[i][0], coord[i][1]), 1, 8, Scalar(255, 255, 0), 3, 8);
            rectangle(img_result, Point(coord[i+4][0] + img1.cols, coord[i+4][1]), Point(coord[i+4][2] + img1.cols, coord[i+4][3]), Scalar(0, 0, 255), 8);
            putText(img_result, to_string(i), Point(coord[i + 4][0] + img1.cols, coord[i + 4][1]), 1, 8, Scalar(255, 255, 0), 3, 8);
        }
        // connenct matching point
        for (int i = 0; i < 4; i++) {
            line(img_result, Point(coord[i][0], coord[i][1]), Point(coord[match[i]+4][0]+img1.cols, coord[match[i]+4][1]), Scalar(0, 0, 255), 10, 8);
        }
        //resize(img_result, img_result, Size(600, 300), 0, 0, INTER_CUBIC); // �������� ������ ���� ũ�⸦ downsizing�ؼ� �����Ѵ�
        imshow("img_result", img_result);
        imwrite("img_result.jpg", img_result);        // img�� ���Ϸ� �����Ѵ�.
        waitKey(0);
    }
    else{
        cout << "img1 or img2 image read fail";
    }
}
int main(){
    cout << "OpenCV Version : " << CV_VERSION << endl;

    Mat first, second;
    first = imread("1st.jpg", IMREAD_GRAYSCALE);
    second = imread("2nd.jpg", IMREAD_GRAYSCALE);
    if (first.empty() || second.empty()) {
        cout << "Image Load Error!" << endl;
    }
    resize(first, first, Size(), 0.25, 0.25);
    resize(second, second, Size(), 0.25, 0.25);

    //�̹��� ���� ���콺��Ŭ���� ��ǥ����
    //(�� �̹������� 4���� �̶� 4�� ���Ϸ� Ŭ���ϸ� �ּ� 4�� Ŭ���϶�� �Ѵ�!)
    make_cent_coord1(first);
    make_cent_coord2(second);
    //�� 8��
    //Mat testmats = make_test_mat();
    int coord[8][4] = {
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0},
        {0,0,0,0} };
    for (int i = 0; i < 8; i++) {
        coord[i][0] = centcoord[2 * i] + 16;
        coord[i][1] = centcoord[(2 * i) + 1] + 16;
        coord[i][2] = centcoord[2 * i] - 16;
        coord[i][3] = centcoord[(2 * i) + 1] - 16;
    }
    // ���÷� �޾ƿ� ���� �Լ����� ���
    /*
    float coord[8][4] = {
        {200,150,240,130},
        {560,55,580,85},
        {270,540,150,570},
        {350,240,300,270},
        {510,770,480,740},
        {70,350,100,380},
        {423,830,475,800},
        {359,220,380,250} };
    */
    float histo1[4][16]= { 
        {1,2,3,4,5,6,7,85,9,10,11,82,13,14,15,16}, 
        {11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26}, 
        {31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46}, 
        {51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66} };
    float histo2[4][16]= { 
        {11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26},
        {1,2,3,4,5,6,7,85,9,10,11,82,13,14,15,16},
        {51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66},
        {31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46} };
    connect_img(histo1,histo2, coord, first, second);
    return 0;
}
