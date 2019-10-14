#include <iostream>           //Basic I/O Operations

#include "opencv2/opencv.hpp" //Include all OpenCV header
int main( int argc, char** argv ) {
    if( argc != 2 ) {
        std::cout << "Usage: ./OpenCV <Pfad-zum-Bild>\n"; // Pritn usage message
        return -1;                                        // Failure occurs
    }
    std::string path_to_image = std::string( argv[ 1 ] );
    std::cout << "Got path to image:" << path_to_image << std::endl;
    //------------------------------//
    //--- Load iamge and test it ---//
    //------------------------------//
    cv::Mat img = cv::imread( path_to_image ); // Load image
    if( img.rows <= 0 ) {
        std::cout << "Faulty iamge file\n"; // Pritn error msg
        return -1;                          // Failure occurs
    }//End check image
    cv::namedWindow("Image", 0);        //Create window with adjustable settigns
    cv::imshow("Image", img);           //Move image to window
    cv::waitKey(0);                     //Pritn iamge and wait for user input
    //---------------------------//
    //--- Access pixel values ---//
    //---------------------------//
    int C=0;
    int R=0;
    std::cout << "I[0,0]" << static_cast<int>(img.at<unsigned char>(0)) << std::endl;
    std::cout << "I[0,0]" << static_cast<int>(img.at<unsigned char>(0,0)) << std::endl;
    std::cout << "I[0,0]" << static_cast<int>(img.at<unsigned char>(cv::Point(C,R))) << std::endl;
} // end main function