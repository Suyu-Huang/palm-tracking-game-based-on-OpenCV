//
//  main.cpp
//  opencv_test001
//
//  Created by 黄苏渝 on 17/1/3.
//  Copyright © 2017年 黄苏渝. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"

using namespace cv;
using namespace std;

void initFruitRouter(int type);
void drawFruit(Point point,Mat* frame);
void on_trackbar(int, void*);
void switchKey(int key);
Point showEdge(Mat &threshedImage, Mat &originalImage);
void createTrackBars();
void morphit(Mat &img);
void blurthresh(Mat &img);

bool doMorph = false;
bool doBlur = false;
bool doShowEdge = false;
bool rotate_swtich=false;

Mat frame_cp;
Mat hsvFrame;
Mat rangeFrame;

Mat pic;

int H_MIN = 0;
int H_MAX = 255;
int S_MIN = 0;
int S_MAX = 255;
int V_MIN = 0;
int V_MAX = 255;

int kerode = 1;
int kdilate =1 ;
int kblur = 1;
int threshval = 0;
int score=0;
bool router_switch[5]={false,false,false,false,false};
Point position1[5];
void moveFruit(Point* pos,int type);

double random(double start, double end)
{
    return start+(end-start)*rand()/(RAND_MAX + 1.0);
}


Point router_calculator(int x,int y){
    x+=1;
    y=(x-640)*(x-640)/144+95;
    //Point newPoint=Point(x,y);
    Point newPoint;
    if(y<=720){
        newPoint=Point(x,y);
        return newPoint;
    }
    else{
        newPoint=Point(0,0);
        return newPoint;
    }
}

int main(){
    Mat UI_IMAGE;
    UI_IMAGE=imread("/Users/huangsuyu/Documents/C++_Practice/picture_library/ui_Image.jpg",CV_LOAD_IMAGE_COLOR);
    resize(UI_IMAGE,UI_IMAGE,Size(1280,720));
    while (true) {
        putText(UI_IMAGE, "PRESS ANY KEY TO CONTINUE", Point(100, UI_IMAGE.rows/2), FONT_HERSHEY_DUPLEX, 4,Scalar(0,0,0));
        imshow("fruit ninja", UI_IMAGE);
        if( waitKey( 10 ) == 27 ){
            break;
        }

    }
    
    createTrackBars();
    double random(double,double);
    srand(unsigned(time(0)));
    int j8=int(random(0,10))%5;
    cout<<"haha"<<j8<<endl;
    //for(int icnt = 0; icnt != 10; ++icnt)
       // cout << "No." << icnt+1 << ": " << int(random(0,10))<< endl;

    VideoCapture cap;
    cap.open(0);
    
    Mat frame;
    String file[5];
    for(int i=0;i<5;i++){
        if(i==0)
            file[i]="/Users/huangsuyu/Documents/C++_Practice/picture_library/apple.jpg";
        else if(i==1)
            file[i]="/Users/huangsuyu/Documents/C++_Practice/picture_library/pear.jpg";
        else if(i==2)
            file[i]="/Users/huangsuyu/Documents/C++_Practice/picture_library/grape.jpg";
        else if(i==3)
            file[i]="/Users/huangsuyu/Documents/C++_Practice/picture_library/banana.png";
        else
            file[i]="/Users/huangsuyu/Documents/C++_Practice/picture_library/strawberry.jpg";
                
    }
    
    for(int i=0;i<5;i++){
        Mat temp_img=imread(file[i].c_str(),CV_LOAD_IMAGE_COLOR);
        if(temp_img.empty())
            exit(-1);
       

    }
    //resize(pic,pic,Size(40,40));
    //Rect roi( Point( originX, originY ), smallImage.size() );

    
    int key;
    char buff[10];
    while(true){
        cap>>frame;
        
        if(frame.empty()){
            break;
        }
        
        switchKey(key);
        flip(frame, frame, 180);
        cvtColor(frame, hsvFrame, COLOR_BGR2HSV);
        inRange(hsvFrame, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), rangeFrame);
        
        
        if (doMorph) {
            morphit(rangeFrame);
            
        }
        if (doBlur) {
            blurthresh(rangeFrame);
        }
        /*
        if (doShowEdge) {
            showEdge(rangeFrame, frame);
        }
         */
        imshow("processed image", rangeFrame);
        if( waitKey( 10 ) == 27 ){
            break;
        }
        
    }
    while(true){
        
        // Image is obtained from the camera
        cap >> frame;
        flip(frame, frame, 180);
        cvtColor(frame, hsvFrame, COLOR_BGR2HSV);
        inRange(hsvFrame, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), rangeFrame);

        if (frame.empty())
            break;
        //int i=1;
        //itoa(i,buff,10);
        //sprintf(buff, "%d",frame.rows);
        //frame_row+=buff;
       // putText(frame, "cvPONG!", Point(0, frame.rows * 0.5 + 30),FONT_HERSHEY_DUPLEX,4,Scalar(255,255,255),10);
        
       // putText(frame,frame_row , Point(50, frame.rows * 0.5 + 30),FONT_HERSHEY_DUPLEX,4,Scalar(255,255,255),10);

        //std::cout<<"hi"<<std::endl;
        Point hand=showEdge(rangeFrame, frame);
        circle(frame, hand, 5, Scalar(0,0,255),-1);
        for(int i=0;i<5;i++){
            int o_e_tmp=  int(random(0,10))%2;
            //cout<int(random(0,10))%5<<endl;
            if(o_e_tmp==0 && router_switch[i]==false){
                router_switch[i]=true;
                initFruitRouter(i);
            }
            if(router_switch[i]==true){
                //Point p=Point(0,0);
                pic=imread(file[i].c_str(),CV_LOAD_IMAGE_COLOR);
                resize(pic,pic,Size(40,40));
                
                moveFruit(&position1[i],i);
                Point2f src_center=Point2f(pic.cols/2.0F, pic.rows/2.0F);
                rotate_swtich=!rotate_swtich;
                if(rotate_swtich){
                    Mat rot_mat = getRotationMatrix2D(src_center, 180, 1.0);
                    warpAffine(pic, pic, rot_mat, pic.size());
                }
                if(position1[i].x+40>0&&position1[i].x+40<=1280&&position1[i].y+40<720&&position1[i].y+40>0){
                    
                    Rect roi=Rect(position1[i].x,position1[i].y,pic.rows,pic.cols);
                    pic.copyTo(frame(roi ));
                }
                
                if(hand.x<=position1[i].x+40 && hand.x>=position1[i].x&& hand.y>=position1[i].y&& hand.y<=position1[i].y+40){
                    router_switch[i]=false;
                    score+=30;
                }
            }
        }
        sprintf(buff,"%d",score);
        String score_str="score :";
        score_str+=buff;
        
        putText(frame,score_str, Point(540, 60),FONT_HERSHEY_SIMPLEX,3,Scalar(255,255,255),10);
        imshow("Simple CV game", frame);
        
        if( waitKey( 10 ) == 27 ){
            break;
        }
        
        

    }
    return 0;
}

void moveFruit(Point* pos,int type){
    int x,y;
    switch (type) {
        case 0:
            
            x=position1[type].x+1;
            y=(x-640)*(x-640)/144+95;
            if(y<=720)
                position1[type]=Point(x,y);
            else{
                initFruitRouter(type);
                router_switch[type]=false;
            }
            
            break;
        case 1:
            y=position1[type].y+1;
            x=400-(y-720)*(y-720)/484;
            if(y+40<=720)
                position1[type]=Point(x,y);
            else{
                initFruitRouter(type);
                router_switch[type]=false;
            }
            break;
        case 2:
            y=position1[type].y+1;
            x=380+(y-720)*(y-720)/324;
            if(y+40<=720)
                position1[type]=Point(x,y);
            else{
                initFruitRouter(type);
                router_switch[type]=false;
            }
            break;
        case 3:
            x=400;
            y=position1[type].y+1;
            if(y+40<=720)
                position1[type]=Point(x,y);
            else{
                initFruitRouter(type);
                router_switch[type]=false;
            }
            break;
        case 4:
            x=900;
            y=position1[type].y+3;
            cout<<position1[type].y<<endl;
            if(y+40<=720)
                position1[type]=Point(x,y);
            else{
                initFruitRouter(type);
                router_switch[type]=false;
            }
            //cout<<position1[type].y<<endl;
            break;
  
        default:
            break;
    }

}

void initFruitRouter(int type){
    switch (type) {
        case 0:
            position1[type]=Point(340,720);
            break;
        case 1:
            position1[type]=Point(0,280);
            break;
        case 2:
            position1[type]=Point(1280,180);
            break;
        case 3:
            position1[type]=Point(400,0);
            break;
        case 4:
            cout<<"haha"<<endl;
            position1[type]=Point(900,0);
            break;
        default:
            break;
    }
}

void createTrackBars() {
    string windowName = "track bar";
    namedWindow(windowName, WINDOW_NORMAL);
    createTrackbar("H_MIN", windowName, &H_MIN, H_MAX, on_trackbar);
    createTrackbar("H_MAX", windowName, &H_MAX, H_MAX, on_trackbar);
    createTrackbar("S_MIN", windowName, &S_MIN, S_MAX, on_trackbar);
    createTrackbar("S_MAX", windowName, &S_MAX, S_MAX, on_trackbar);
    createTrackbar("V_MIN", windowName, &V_MIN, V_MAX, on_trackbar);
    createTrackbar("V_MAX", windowName, &V_MAX, V_MAX, on_trackbar);
    createTrackbar("Erode", windowName, &kerode, 31, on_trackbar);
    createTrackbar("Dilate", windowName, &kdilate, 31, on_trackbar);
    createTrackbar("Blur", windowName, &kblur, 255, on_trackbar);
    createTrackbar("Thresh", windowName, &threshval, 255, on_trackbar);
}

void drawFruit(Point point,Mat* f){
    
    Mat temp=*f;
    Rect roi=Rect(point.x,point.y,pic.rows,pic.cols);
    pic.copyTo(temp(roi ));
}

void switchKey(int key) {
    switch (key)
    {
        case 'm':
            doMorph =! doMorph;
            if (doMorph)
                cout << "Morph on" << endl;
            else
            {
                cout << "Morph off" << endl;
            }
            break;
        case 'e':
            doShowEdge = !doShowEdge;
            if (doShowEdge) {
                cout << "show edge on" << endl;
            }
            else
            {
                cout << "show edge off" << endl;
            }
            break;
        case'b':
            doBlur = !doBlur;
            if (doBlur) {
                cout << "do blur on" << endl;
            }
            else
            {
                cout << "do blur off" << endl;
            }
            break;
        default:
            break;
    }
}

void morphit(Mat &img)
{
    erode(img, img, getStructuringElement(MORPH_RECT, Size(kerode, kerode)));
    dilate(img, img, getStructuringElement(MORPH_RECT, Size(kdilate, kdilate)));
}

void blurthresh(Mat &img)
{
    //medianBlur(img,img,kblur%2+3+kblur);
    blur(img, img, Size(kblur, kblur), Point(-1, -1), BORDER_DEFAULT);
    threshold(img, img, threshval, 255, THRESH_BINARY_INV);
}

Point showEdge(Mat &threshedImage, Mat &originalImage)
{
    vector<Mat> contours;
    Mat hierarchy;
    int largest_area = 0;
    int largest_contour_index = 0;
    findContours(threshedImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    //this will find largest contour
    for (int i = 0; i < contours.size(); i++) // iterate through each contour.
    {
        double a = contourArea(contours[i], false);  //  Find the area of contour
        if (a > largest_area)
        {
            largest_area = a;
            largest_contour_index = i;                //Store the index of largest contour
        }
        
    }
    //search for largest contour has end
    
    
    
    Moments moment = moments(threshedImage, true);
    Point center(moment.m10 / moment.m00, moment.m01 / moment.m00);
    circle(threshedImage, center, 8, Scalar(0, 0, 0), CV_FILLED);
    return center;
    /*
    // 寻找指尖
    vector<Point> couPoint = contours[largest_contour_index];
    vector<Point> fingerTips;
    Point tmp;
    int max=0, count=0, notice=0;
    for (int i = 0; i < couPoint.size(); i++)
    {
        tmp = couPoint[i];
        int dist = (tmp.x - center.x) * (tmp.x - center.x) + (tmp.y - center.y) * (tmp.y - center.y);
        if (dist > max)
        {
            max = dist;
            notice = i;
        }
        
        // 计算最大值保持的点数，如果大于40（这个值需要设置，本来想根据max值来设置，
        // 但是不成功，不知道为何），那么就认为这个是指尖
        if (dist != max)
        {
            count++;
            if (count > 40)
            {
                count = 0;
                max = 0;
                bool flag = false;
                // 低于手心的点不算
                if (center.y < couPoint[notice].y)
                    continue;
                // 离得太近的不算    
                for (int j = 0; j < fingerTips.size(); j++)
                {
                    if (abs(couPoint[notice].x - fingerTips[j].x) < 20)
                    {
                        flag = true;
                        break;
                    }
                }
                if (flag) continue;
                fingerTips.push_back(couPoint[notice]);
                circle(threshedImage, couPoint[notice], 6, Scalar(0, 255, 0), CV_FILLED);
                line(threshedImage, center, couPoint[notice], Scalar(255, 0, 0), 2);
            }
        }*/
     
}
    

void on_trackbar(int, void*)
{//This function gets called whenever a
    // trackbar position is changed
    if (kerode == 0)
        kerode = 1;
    if (kdilate == 0)
        kdilate = 1;
    if (kblur == 0)
        kblur = 1;
}
