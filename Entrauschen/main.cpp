#include <iostream>           //Basic I/O Operations
#include "opencv2/opencv.hpp" //Include all OpenCV header
//------------------------------------------//
// Function: addNoise(...)                  //
// Descr: This function adds noise to a     //
//      given image.                        //
// Param: cv::Mat & img     Image to disturb//
//          int param       Noise factor    //
// Return: -                                //
//------------------------------------------//
void addNoise( cv::Mat& img ) {
    cv::Mat noise( img.size( ), img.type( ) ); // Create memory for noise
    cv::randn( noise, 0., 50. );               // Create noise matrix with N(0,50)
    img += noise;
}
int main( int, char** ) {
    cv::Mat img = cv::imread( "lena.png" ); // Load OpenCV test image
    cv::Mat img_raw = img.clone( );         // Get noise free clone
    cv::Mat img_norm;                       // Memory for normalized block filter
    cv::Mat img_Gauss;                      // Memory for Gaussian filter
    cv::Mat img_Median;                     // Memory for Median filter
    //-------------------------//
    //--- Create noisy data ---//
    //-------------------------//
    std::vector< cv::Mat > BGR;  // Channel images
    cv::split( img, BGR );       // Get channels
    std::for_each( BGR.begin( ), // From ...
                   BGR.end( ),   //... to apply ...
                   addNoise );   //... this function
    cv::merge( BGR, img );       // Merge noisy data
    //--------------------//
    //--- Denoise data ---//
    //--------------------//
    int Kernel_width = 9;  // Width of smoothing kernel
    int Kernel_height = 9; // Height of smoothing kernel
    cv::Size KernelSize = cv::Size( Kernel_width, Kernel_height );
    //--- Normalized block filter ---//
    cv::blur( img,                // Source
              img_norm,           // Destination
              KernelSize,         // Kernel size (cv::Size)
              cv::Point( -1, -1 ) // Use center of kernel as anchor
              );                  // Apply filter
    //--- Gaussian blur ---//
    cv::GaussianBlur( img,        // Source
                      img_Gauss,  // Destination
                      KernelSize, // Kernel size (cv::Size)
                      0,
                      0  // Calculate sigma using kernel size
                      ); // Apply filter
    //--- Median filter ---//
    cv::medianBlur( img,         // Source
                    img_Median,  // Destination
                    Kernel_width // Kernel size (is square) - int
                    );           // Apply filter
    //----------------//
    //--- Vis data ---//
    //----------------//
    cv::Mat dummy;
    cv::namedWindow( "AllImgs", 0 );
    cv::namedWindow( "Normalized", 0 );
    cv::namedWindow( "Gaussian", 0 );
    cv::namedWindow( "Median", 0 );
    //--- Show images ---//
    dummy = img.clone( );
    dummy.push_back( img_norm );
    cv::imshow( "Normalized", dummy );      // Move image to window
    cv::imwrite( "noise_norm.png", dummy ); // Store image
    dummy = img.clone( );
    dummy.push_back( img_Gauss );
    cv::imshow( "Gaussian", dummy );         // Move image to window
    cv::imwrite( "noise_gauss.png", dummy ); // Store image
    dummy = img.clone( );
    dummy.push_back( img_Median );
    cv::imshow( "Median", dummy );            // Move image to window
    cv::imwrite( "noise_median.png", dummy ); // Store image
    dummy = img.clone( );
    dummy.push_back( img_raw );
    cv::imshow( "AllImgs", dummy ); // Move image to window
    cv::waitKey( 0 );               // Plot image on screen and wait for user input
    //--- Store results ---//
    cv::hconcat(img_raw,img_norm, img_norm);
    cv::hconcat(img_norm, img_Median, img_norm);
    cv::hconcat(img_norm, img_Gauss, img_norm);
    cv::imwrite( "comparison.png", img_norm ); // Store image
    return 0;
}