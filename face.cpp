 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"
 #include "opencv2/opencv.hpp"
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

 using namespace cv;
 using namespace std;

 Mat src; Mat src_gray;
 int thresh = 100;
 int max_thresh = 255;
 RNG rng(12345);
int H_MIN =0;
int H_MAX = 255;
int S_MIN = 0;
int S_MAX = 255;
int V_MIN = 0;
int V_MAX = 255;
int kdilate=1,kerode=1;
int oldx=0,oldy=0;
int rd=0;
int con=0;

Display *dpy;
Window root_window;
void rightmouseClick() ;

void setpo(int,int);
void mouseClick(int button);
void on_trackbar(int, void*)
{//This function gets called whenever a
	// trackbar position is changed
	if (kerode == 0)
		kerode = 1;
	if (kdilate == 0)
		kdilate = 1;
	
}


 /// Function header
 void thresh_callback(int, void* );

/** @function main */
int main( int argc, char** argv )
 { 


		dpy = XOpenDisplay(0);





   /// Load source image and convert it to gray
 	  VideoCapture cap(0);


   while(1)
		{
			cap>>src;
			

//Rect roir=Rect(340,100,270,270);
//src=src(roir);
 

 // src = imread("/home/hitesh/Downloads/hull.jpg" );
flip(src,src,1);
   cvtColor(src,src_gray,cv::COLOR_BGR2HSV);
   // Convert image to gray and blur it

inRange(src_gray,Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX),src_gray);
//cvtColor( src, src_gray, COLOR_BGR2GRAY );
GaussianBlur( src_gray, src_gray, Size(1,1),0,0);

	
//erode(src_gray, src_gray, getStructuringElement(MORPH_RECT, Size(3, 3)));
//dilate(src_gray, src_gray,getStructuringElement(MORPH_RECT, Size(8,8)));

   /// Create Window
   char* source_window = "Source";
   namedWindow( source_window, CV_WINDOW_KEEPRATIO );
   imshow( source_window, src );
  //threshold(src_gray,src_gray,0,255,(CV_THRESH_BINARY_INV+CV_THRESH_OTSU));
on_trackbar(0, 0);
createTrackbar("Erode", source_window, &kerode, 31, on_trackbar);
createTrackbar("Dilate", source_window, &kdilate, 31, on_trackbar);
createTrackbar("H_MIN", source_window, &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", source_window, &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", source_window, &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", source_window, &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", source_window, &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX",source_window, &V_MAX, V_MAX, on_trackbar);
	


	
erode(src_gray, src_gray, getStructuringElement(MORPH_RECT, Size(kerode,kerode)));
dilate(src_gray, src_gray,getStructuringElement(MORPH_RECT, Size(kdilate,kdilate)));


createTrackbar( " Threshold:", source_window, &thresh, max_thresh, on_trackbar );
thresh_callback( 0, 0 );
 //namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE );
//imshow( "Hull demo", src_gray);
  waitKey(30);}
XCloseDisplay(0);
   waitKey(0);
   return(0);
 }







 /** @function thresh_callback */
 void thresh_callback(int, void* )
 {
   Mat src_copy = src.clone();
   Mat threshold_output;
   vector<vector<Point> > contours;
   vector<Vec4i> hierarchy;
   Scalar color(255, 255, 255);
   /// Detect edges using Threshold
   threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY);
   // namedWindow( "grgrg", CV_WINDOW_AUTOSIZE );
    //imshow( "grgrg", src_gray);
namedWindow( "out", CV_WINDOW_AUTOSIZE );
    imshow( "out", threshold_output);

if(rd==0)
{char ready=waitKey(30);
if(ready=='1')
rd=1;
}
if(rd==1)
{ //cout<<rd;
   int largest_contour_index = -1;
   int largest_area = 0;
Rect bounding_rect;
   /// Find contours
   findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

   /// Find the convex hull object for each contour
   vector<vector<int> >hull( contours.size() );
  vector< std::vector<Vec4i> > defects( contours.size() );
   for( int i = 0; i < contours.size(); i++ )
      {  convexHull( Mat(contours[i]), hull[i], false );
 }

   /// Draw contours + hull results
   Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
   for( int i = 0; i< contours.size(); i++ )
      {   drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );   

if(hull[i].size() > 3)
          convexityDefects(contours[i], hull[i], defects[i]);

      



   
  	double a = contourArea(contours[i], false);  //  Find the area of contour
    if (a>largest_area){
        largest_area = a;
        largest_contour_index = i;                //Store the index of largest contour
        bounding_rect = boundingRect(contours[i]);
        //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       //drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       //drawContours( drawing, hull, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
      }}
 Point center = Point((bounding_rect.x + bounding_rect.width)/2, (bounding_rect.y + bounding_rect.height)/2);
  // cout<<"Rectangle  Centroid position is at: " << center.x << " " << center.y << endl;
 double area0 = bounding_rect.width*bounding_rect.height;
 //cout<<"\n Area is:"<<area0;




if((largest_contour_index >= 0)&& area0>20000 && area0<304964)
{   
RotatedRect rect = minAreaRect(Mat(contours[largest_contour_index]));
Point2f rect_points[4]; rect.points( rect_points );
for( int j = 0; j < 4; j++ )
line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );

con=0;
for(const Vec4i& v : defects[largest_contour_index])
    {   
       
        float depth = v[3] / 256;
        if ((depth > 40) && depth<80) //  filter defects by depth, e.g more than 10
        {   con++;
            int startidx = v[0]; Point ptStart(contours[largest_contour_index][startidx]);
            int endidx = v[1]; Point ptEnd(contours[largest_contour_index][endidx]);
            int faridx = v[2]; Point ptFar(contours[largest_contour_index][faridx]);

            line(drawing, ptStart, ptEnd, Scalar(0, 255, 0), 1);
            line(drawing, ptStart, ptFar, Scalar(0, 255, 0), 1);
            line(drawing, ptEnd, ptFar, Scalar(0, 255, 0), 1);
            circle(drawing, ptFar, 4, Scalar(0, 255, 0), 2);
	

        }
    }

cout<<con<<"\n";

    drawContours(drawing, contours, largest_contour_index, color, 1, 8,vector<Vec4i>(), 0, Point()); // Draw the largest contour using 
    rectangle(drawing, bounding_rect, Scalar(0, 255, 0), 1, 8, 0);
    Point center = Point((bounding_rect.x + bounding_rect.width)/2, (bounding_rect.y + bounding_rect.height)/2);
 

//setting pos of mouse 
if(con<10)
{
//cout<<"\n"<<center.x<<"  "<<center.y;
int x=(3000/460)*((center.x)-100);
int y=(1800/300)*((center.y)-150);
if(abs(oldx-center.x)>1 || abs(oldy-center.y)>1)
{
oldx=center.x;oldy=center.y;
setpo(x,y);}
//cout<<"\n"<<x<<"  "<<y;
}

}


   /// Show in a window
   namedWindow( "Hull demo", CV_WINDOW_KEEPRATIO );
   imshow( "Hull demo", drawing );

 }

}

void setpo(int x,int y)
{int id=0;
root_window = XRootWindow(dpy, 0);
XSelectInput(dpy, root_window, KeyReleaseMask);
XWarpPointer(dpy, None, root_window, 0, 0, 0, 0, x, y);
if(con==1)
{
mouseClick(Button1);

}
if(con==2)
{
rightmouseClick();
sleep(1);

}
//else id=0;
XFlush(dpy); 
}



void mouseClick(int button) 
{
  Display *display = XOpenDisplay(NULL);
XTestFakeButtonEvent (dpy, 1, True,  CurrentTime);
  XTestFakeButtonEvent (dpy,1, False, CurrentTime);
    
    XFlush(display);
    XCloseDisplay(display);
}

void rightmouseClick() 
{
  Display *display = XOpenDisplay(NULL);
XTestFakeButtonEvent (dpy, 3, True,  CurrentTime);
  XTestFakeButtonEvent (dpy,3, False, CurrentTime);
    
    XFlush(display);
    XCloseDisplay(display);
}












