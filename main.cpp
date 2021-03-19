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
void connect_img(float arr1[][4],float arr2[][4], float coord[][4]) {//(need: 2 imgs, label coord, 2 arr(histogram))
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
        //imshow("img1", img1);
        //imshow("img2", img2);
        //resize(img_result, img_result, Size(600, 300), 0, 0, INTER_CUBIC); // 사진으로 저장할 때는 크기를 downsizing해서 저장한다
        imshow("img2", img_result);
        imwrite("img_result.jpg", img_result);        // img를 파일로 저장한다.
        waitKey(0);
    }
    else{
        cout << "img1 or img2 image read fail";
    }
    // put sqare & number
// make 2 imgs into 1 img(need label coord cal)
// cal histogram(need: 2arr(histogram))
// connect labeled indexes
// save the final img
}
int main(int, char**)
{
    cout << "OpenCV Version : " << CV_VERSION << endl;
    // 예시로 받아온 전의 함수들의 결과
    float coord[8][4] = { {1153,1040,1132,1114},{52,36,17,85},{2191,2010,2041,2062},{1673,1644,1725,1766},{1761,1782,1753,1714},{1032,1016,1267,1138},{29,80,21,46},{23,54,75,26} };
    float histo1[4][4]= { {1, 2, 3, 4}, { 6, 7, 8, 9}, { 11, 12, 13, 14}, { 11, 12, 13, 14} };
    float histo2[4][4]= { {1, 2, 3, 4}, { 6, 7, 8, 9}, { 11, 12, 13, 14}, { 11, 12, 13, 14} };
    connect_img(histo1,histo2, coord);
    return 0;
}
