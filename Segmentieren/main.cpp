#include <iostream>           //Basic I/O Operations
#include "opencv2/opencv.hpp" //Include all OpenCV header
int main( int, char** ) {
    cv::Mat img = cv::imread( "./keyDetector.jpg" ); // Load OpenCV test image
    //--------------------//
    //--- Get channels ---//
    //--------------------//
    std::vector< cv::Mat > BGR; // Memory for channels
    cv::split( img, BGR );      // Get channels
    //-----------------------//
    //--- Do thresholding ---//
    //-----------------------//
    int TH = 128;                    // Threshold value
    int TH2 = 180;                   // Threshold value
    cv::Mat img_TH;                  // Memory for thresholded image
    cv::Mat img_TH2;                 // Memory for thresholded image
    cv::threshold( BGR[ 0 ],         // Source
                   img_TH,           // Thresholded image
                   TH,               // threshold value
                   255,              // We set all values above TH to 255
                   cv::THRESH_BINARY // Different options implemented in OpenCV
                   );
    cv::threshold( BGR[ 0 ],         // Source
                   img_TH2,          // Thresholded image
                   TH2,              // threshold value
                   255,              // We set all values above TH to 255
                   cv::THRESH_BINARY // Different options implemented in OpenCV
                   );
    //-----------//
    //--- ExB ---//
    //-----------//
    cv::Mat ExB_TH;                                // Memory for thresholded image
    std::for_each( BGR.begin( ),                   // Start of BGR image memory (separated cv::Mat channels)
                   BGR.end( ),                     // End of BGR image memory
                   []( cv::Mat& A ) {              // Fkt to apply for each element
                       A.convertTo( A, CV_64FC1 ); // Convert to double
                   } );
    cv::Mat ExB = 2. * BGR[ 0 ] - BGR[ 1 ] - BGR[ 2 ]; // Get Exb image
    double min, max;                                   // Memory for min,max --> normalization
    cv::minMaxIdx( ExB, &min, &max );                  // Get min, max values
    ExB = ( ( ExB - min ) / ( max - min ) ) * 255.;    // Nomalize image
    ExB.convertTo( ExB, CV_8UC1 );                     // Convert to 8 bit image
    cv::threshold( ExB,                                // Source
                   ExB_TH,                             // Thresholded image
                   TH2,                                // threshold value
                   255,                                // We set all values above TH to 255
                   cv::THRESH_BINARY                   // Different options implemented in OpenCV
                   );
    //----------------------//
    //--- Do OTSU on ExB ---//
    //----------------------//
    cv::Mat otsu;                      // Memory for otsus result
    cv::threshold( ExB,                // Source
                   otsu,               // Thresholded image
                   0,                  // Value is ignored due to OTSU
                   255,                // We set all values above TH to 255
                   cv::THRESH_BINARY | // Do binary thresholding and...
                       CV_THRESH_OTSU  //... apply Otsus method
                   );

    //-------------------------//
    //--- Visualize results ---//
    //-------------------------//
    //--- Prepare images ---//
    cv::resize( img, img, cv::Size( img.cols / 4, img.rows / 4 ) );
    cv::resize( img_TH, img_TH, cv::Size( img_TH.cols / 4, img_TH.rows / 4 ) );
    cv::resize( img_TH2, img_TH2, cv::Size( img_TH2.cols / 4, img_TH2.rows / 4 ) );
    cv::resize( otsu, otsu, cv::Size( otsu.cols / 4, otsu.rows / 4 ) );
    cv::resize( ExB, ExB, cv::Size( ExB.cols / 4, ExB.rows / 4 ) );
    cv::resize( BGR[ 0 ], BGR[ 0 ], cv::Size( BGR[ 0 ].cols / 4, BGR[ 0 ].rows / 4 ) );
    cv::resize( ExB_TH, ExB_TH, cv::Size( ExB_TH.cols / 4, ExB_TH.rows / 4 ) );
    cv::cvtColor( img_TH, img_TH, CV_GRAY2RGB );
    cv::cvtColor( img_TH2, img_TH2, CV_GRAY2RGB );
    cv::cvtColor( otsu, otsu, CV_GRAY2RGB );
    cv::cvtColor( ExB, ExB, CV_GRAY2RGB );
    BGR[ 0 ].convertTo( BGR[ 0 ], CV_8UC1 );
    cv::cvtColor( BGR[ 0 ], BGR[ 0 ], CV_GRAY2RGB );
    cv::cvtColor( ExB_TH, ExB_TH, CV_GRAY2RGB );
    //--- Variables for visualizations ---//
    std::vector<cv::Mat> dummy;     //Dummy memory 
    cv::Mat Result; // Resulting image
    //--- Plot imgs ---//
    cv::namedWindow( "Result", 0 );
    cv::namedWindow( "Result2", 0 );
    cv::namedWindow( "ExB", 0 );
    cv::namedWindow( "ExBOtsu", 0 );
    //--- Calc resulting images ---//
    cv::hconcat( img, img_TH, Result );
    cv::imwrite( "result.png", Result );
    cv::imshow( "Result", Result );
    cv::hconcat( img, img_TH2, Result );
    cv::imshow( "Result2", Result );
    cv::imwrite( "result2.png", Result );
    cv::imshow( "ExB", ExB );
    cv::imwrite( "ExB.png", ExB );
    dummy = {img, ExB_TH, otsu};
    cv::hconcat(dummy, Result);
    cv::imshow("ExBOtsu",Result);
    cv::waitKey( 0 );
    //------------------------//
    //--- Store comparison ---//
    //------------------------//
    dummy = {img, BGR[ 0 ], img_TH2};
    cv::hconcat( dummy, Result );
    cv::imwrite( "TH_BGR.png", Result );
    dummy = {img, ExB, ExB_TH};
    cv::hconcat( dummy, Result );
    cv::imwrite( "TH_ExG.png", Result );
    dummy = {img, ExB_TH, otsu};
    cv::hconcat( dummy, Result );
    cv::imwrite( "TH_ExG_otsu.png", Result );

    return 0;
}