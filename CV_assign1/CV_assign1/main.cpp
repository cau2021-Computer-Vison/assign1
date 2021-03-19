/**
  @file videocapture_basic.cpp
  @brief A very basic sample for using VideoCapture and VideoWriter
  @author PkLab.net
  @date Aug 24, 2016
*/

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
void connect_img(int arr1[][4],int arr2[][4], int coord[][4]) {//(need: 2 imgs, label coord, 2 arr(histogram))
// put labeling into each img(need: label coord)
    // read 2 imgs
    Mat img1 = imread("1st.jpg", IMREAD_GRAYSCALE);
    Mat img2 = imread("2nd.jpg", IMREAD_GRAYSCALE);
    if (!img1.empty() && !img2.empty()) {
        cout << "img1 channel : " << img1.channels() << endl;
        namedWindow("img1");
        imshow("img1", img1);
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
    int coord[8][4] = { {1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16},{31,32,33,34},{35,36,37,38},{39,40,41,42},{43,44,45,46} };
    int histo1[4][4]= { {1, 2, 3, 4}, { 6, 7, 8, 9}, { 11, 12, 13, 14}, { 11, 12, 13, 14} };
    int histo2[4][4]= { {1, 2, 3, 4}, { 6, 7, 8, 9}, { 11, 12, 13, 14}, { 11, 12, 13, 14} };
    connect_img(histo1,histo2, coord);
    return 0;
}
