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
void connect_img(float arr1[][16],float arr2[][16], float coord[][4]) {//(need: 2 imgs, label coord, 2 arr(histogram))
// cal histogram(need: 2arr(histogram))
    int match[4] = { -1,-1,-1,-1 };
    for (int i = 0; i < 4; i++) {
        float min = 99999.0;
        for (int j = 0; j < 4; j++) {
            float tot = 0.0;
            for (int k = 0; k < 16; k++) {// 16번 오차의 제곱의 합
                tot += (arr1[i][k] - arr2[j][k]) * (arr1[i][k] - arr2[j][k]);   //제곱을 더하는 과정
            }
            if (min > tot) {
                // 오차가 최소가 된다면 match된다고 볼 수 있음
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
    Mat img1 = imread("1st.jpg", IMREAD_GRAYSCALE);
    Mat img2 = imread("2nd.jpg", IMREAD_GRAYSCALE);
    if (!img1.empty() && !img2.empty()) {
        cout << "img1 channel : " << img1.channels() <<" Width: "<< img1.cols << " Height: " << img1.rows << endl;
        cout << "img2 channel : " << img2.channels() << " Width: " << img2.cols << " Height: " << img2.rows << endl;
        Mat img_result;
        hconcat(img1,img2, img_result);
        for (int i = 0; i < 4; i++) {
            rectangle(img_result, Point(coord[i][0], coord[i][1]), Point(coord[i][2], coord[i][3]), Scalar(0, 0, 255), 8);
            putText(img_result, to_string(i), Point(coord[i][0], coord[i][1]), 1, 22, Scalar(255, 255, 0), 11, 8);
            rectangle(img_result, Point(coord[i+4][0] + img1.cols, coord[i+4][1]), Point(coord[i+4][2] + img1.cols, coord[i+4][3]), Scalar(0, 0, 255), 8);
            putText(img_result, to_string(i), Point(coord[i + 4][0] + img1.cols, coord[i + 4][1]), 1, 22, Scalar(255, 255, 0), 11, 8);
        }
        // connenct matching point
        for (int i = 0; i < 4; i++) {
            line(img_result, Point(coord[i][0], coord[i][1]), Point(coord[match[i]][0]+img1.cols, coord[match[i]][1]), Scalar(0, 0, 255), 10, 8);
        }
        //resize(img_result, img_result, Size(600, 300), 0, 0, INTER_CUBIC); // 사진으로 저장할 때는 크기를 downsizing해서 저장한다
        imshow("img2", img_result);
        imwrite("img_result.jpg", img_result);        // img를 파일로 저장한다.
        waitKey(0);
    }
    else{
        cout << "img1 or img2 image read fail";
    }
}
int main(int, char**){
    cout << "OpenCV Version : " << CV_VERSION << endl;
    // 예시로 받아온 전의 함수들의 결과
    float coord[8][4] = { {1153,1040,1132,1114},{52,36,17,85},{2191,2010,2041,2062},{1673,1644,1725,1766},{1761,1782,1753,1714},{1032,1016,1267,1138},{29,80,21,46},{23,54,75,26} };
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
    connect_img(histo1,histo2, coord);
    return 0;
}
