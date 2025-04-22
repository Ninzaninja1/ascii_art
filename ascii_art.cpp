#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp> 

using namespace cv; 
using namespace std; 

void ImageToAscii(string imagePath);
void VideoToAscii(string video);
void ImageIteration(Mat frame);

int main(void) {

  string pathToImage = "lloy_boi_mm.png";
  string webcam = "/dev/video0";
  string video = "bad_apple.mp4";
  string input;

  cout << "Input the file name below (with the file extension) or '/dev/video0' for your webcam: \n" << endl;
  getline(cin, input);

  if(input.substr(input.find_last_of(".") + 1) == "png") {
   ImageToAscii(input); 
  }
  else if (input.substr(input.find_last_of(".") + 1) == "mp4"){
    VideoToAscii(input);
  }
  else if (input == webcam){
    VideoToAscii(webcam);
  }
  else {
    cout << "Please use a valid file (only png or mp4 or webcam) ";
  }


  ImageToAscii(input);
  // VideoToAscii(webcam);

  return 0;
}

void ImageIteration(Mat frame){

  uchar greyscale;
  int average;

  // different ascii textures
  array<char, 12>  texture1{'.' , ',' , '-' , '~' , ':' , ';' , '=' , '!' , '*' , '#' , '$' , '@'};
  array<char, 12>  texture2{' ' , '.' , '`' , ':' , ';' , '-' , '~' , '0' , '#' , '$' , '&' , '@'};
  array<char, 10>  texture3{' ' , '.' , ':' , 'c' , 'o' , 'P' , '0' , '?' , '@', '#'};
  array<char, 10>  texture4{'#' , '@' , '?' , '0' , 'P' , 'o' , 'c' , ':' , '.', ' '};

  // iterate thrgouh the image
  for (int row = 0; row < frame.rows; row+=8) {
    for (int col = 0; col < frame.cols; col+=8) {

      greyscale = frame.at<uchar>(row,col);
      average = greyscale % 10 ;
       
      cout << texture3[average];
      cout << " ";
       
    }
    cout << "\n";
 }

}

void ImageToAscii(string imagePath){

  Mat image = imread(imagePath, IMREAD_GRAYSCALE); // read image as greyscale
  Mat imageScaled = image;
  Mat imageContrast = image;


    // Check for failure
  if (image.empty()) {
    cout << "Could not open or find the image" << endl;
    cin.get(); //wait for any key press
    return;
  }

  imageScaled.convertTo(imageContrast, -1, 2.0, -100);

  ImageIteration(imageContrast);

  imshow("hehe its lloy", imageContrast);
  waitKey(0);

}


void VideoToAscii(string video){

  Mat frame;
  Mat frameContrast;
  VideoCapture cap;

  uchar greyscale;
  int average;

  cap.open(video);
  // check if the file is opened
  if (cap.isOpened()) {
    cout << "Opened" << endl;
  }
  else {
    cout << "Video not able to open" << endl;
    return;
  }

  // loop through each frame it video
  for (;;){
    cap.read(frame);

    // Check for failure
    if (frame.empty()) {
      cout << "Could not open or find the image" << endl;
      cin.get(); //wait for any key press
      return;
    }

    cvtColor(frame, frame, COLOR_BGR2GRAY);
    // add cotrast to frame
    frame.convertTo(frameContrast, -1, 2.0, 0);

   
    ImageIteration(frameContrast);

    int framerate = 1000/24;

    imshow("", frame);
    waitKey(framerate);

    system("clear");
  }

  cap.release();
}
