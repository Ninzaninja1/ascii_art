#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp> 

using namespace cv; 
using namespace std; 

void ImageToAscii(string imagePath);
void VideoToAscii(string video);
Mat ImageIteration(Mat frame);
Mat DifferenceOfGaussian(Mat input);

int main(void) {

  string pathToImage = "lloy_boi_mm.png";
  string webcam = "/dev/video0";
  string video = "bad_apple.mp4";
  string input;

  cout << "Input the file name below (with the file extension) or 'webcam' for your webcam: \n" << endl;
  getline(cin, input);

  if(input.substr(input.find_last_of(".") + 1) == "png") {
   ImageToAscii(input); 
  }
  else if (input.substr(input.find_last_of(".") + 1) == "mp4"){
    VideoToAscii(input);
  }
  else if (input == "webcam"){
    VideoToAscii(webcam);
  }
  else {
    cout << "Please use a valid file (only png or mp4 or webcam) ";
  }

  return 0;
}

Mat DifferenceOfGaussian(Mat input){
  Mat gaussianImage1;
  Mat gaussianImage2;
  
  float tau = 1;

  GaussianBlur(input, gaussianImage1, Size(15, 15), 0);
  GaussianBlur(input, gaussianImage2, Size(35, 35), 0);

  input = gaussianImage2 - (tau * gaussianImage1);

  return input;
}

Mat ImageIteration(Mat frame){

  int average;

  // different ascii textures
  array<char, 12>  texture1{'.' , ',' , '-' , '~' , ':' , ';' , '=' , '!' , '*' , '#' , '$' , '@'};
  array<char, 12>  texture2{' ' , '.' , '`' , ':' , ';' , '-' , '~' , '0' , '#' , '$' , '&' , '@'};
  array<char, 10>  texture3{' ' , '.' , ':' , 'c' , 'o' , 'P' , '0' , '?' , '@', '#'};
  array<char, 10>  texture4{'#' , '@' , '?' , '0' , 'P' , 'o' , 'c' , ':' , '.', ' '};

  // iterate thrgouh the image
  for (int row = 0; row < frame.rows; row+=4) {
    for (int col = 0; col < frame.cols; col+=4) {
      average = (int)(frame.at<uchar>(row,col)) % 10;
       
      cout << texture3[average];
      cout << " ";
       
    }
    cout << "\n";
 }

  // imshow("", frame);
 return frame;
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
  imageContrast = DifferenceOfGaussian(imageContrast);

  imageContrast = ImageIteration(imageContrast);

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

    cvtColor(frame, frame, COLOR_BGR2GRAY); // change image to greyscale
    // frame.convertTo(frameContrast, -1, 2.0, 0); // add cotrast to frame
    frameContrast = DifferenceOfGaussian(frame); // apply the difference of gaussians 

    frameContrast = ImageIteration(frameContrast);

    int framerate = 1000/24;

    imshow("", frameContrast);
    waitKey(framerate);

    //system("clear");
  }

  cap.release();
}
