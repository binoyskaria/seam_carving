#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>  
#include <climits>
#include <math.h>

using namespace std;
using namespace cv;
void displayenergymat(Mat sample){
    for(int i = 0 ; i < sample.rows; i++){
        for(int j = 0 ; j< sample.cols; j++){
            cout<<sample.at<double>(i,j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}


double* nextseamof(Mat dp) {
    // Find seam to carve
    double* seamj = new double[dp.rows];
    int spj = 0;
    double minimlastrowval = dp.at<double>(dp.rows - 1, 0);
    
    for (int j = 0; j < dp.cols; ++j) {
        if (dp.at<double>(dp.rows - 1, j) < minimlastrowval) {
            minimlastrowval = dp.at<double>(dp.rows - 1, j);
            spj = j;
        }
    }
    
    seamj[dp.rows - 1] = spj;
    
    for (long long spi = dp.rows - 2; spi >= 0; --spi) {
        int j = seamj[spi + 1]; // Get the previous seam index
        
        if (j > 0 && dp.at<double>(spi, j - 1) <= dp.at<double>(spi, j) && dp.at<double>(spi, j - 1) <= dp.at<double>(spi, j + 1)) {
            seamj[spi] = j - 1;
        } else if (j < dp.cols - 1 && dp.at<double>(spi, j + 1) <= dp.at<double>(spi, j) && dp.at<double>(spi, j + 1) <= dp.at<double>(spi, j - 1)) {
            seamj[spi] = j + 1;
        } else {
            seamj[spi] = j;
        }
    }
    return seamj;
}

Mat dpmatof(Mat energymat){
    Mat dp(energymat.rows, energymat.cols, CV_64F);

    energymat.copyTo(dp);

    for (int i = 1; i < energymat.rows; ++i) {
        for (int j = 0; j < energymat.cols; ++j) {
            double final = dp.at<double>(i,j);
            double minim = LONG_LONG_MAX;
            minim = min(energymat.at<double>(i - 1,max(j - 1, 0)), energymat.at<double>(i - 1,j));
            minim = min(energymat.at<double>(i - 1,min(j + 1, energymat.cols - 1)), minim);
            //cout<<"minim is"<<minim;
            final =  final + minim;
            dp.at<double>(i,j) = final;
        }
    }
    return dp;
}

Mat createnergy(Mat img){
    Mat energymat(img.rows, img.cols, CV_64F);

    for(int i = 0 ; i < img.rows; i++){
        for(int j = 0 ; j < img.cols; j++){
            Vec3b pixelleft = img.at<Vec3b>(i, max(j - 1, 0));
            Vec3b pixelright = img.at<Vec3b>(i, min(j + 1, img.cols - 1));
            Vec3b pixeltop = img.at<Vec3b>(max(0, i - 1), j);
            Vec3b pixelbottom = img.at<Vec3b>(min(img.rows - 1, i + 1), j);
            double horizontaldata = pow(pixelleft[0] - pixelright[0], 2) + pow(pixelleft[1] - pixelright[1], 2) + pow(pixelleft[2] - pixelright[2], 2);
            double verticaldata = pow(pixeltop[0] - pixelbottom[0], 2) + pow(pixeltop[1] - pixelbottom[1], 2) + pow(pixeltop[2] - pixelbottom[2], 2);
            double finaldata = horizontaldata + verticaldata;

            energymat.at<double>(i, j) = finaldata;
        }
    }

    return energymat;
}


int main(int argc, char* argv[]) {
    int newxresol;
    int newyresol;
    string inpu;

    cout<<"insert new x resolution: "<<endl;
    cin>>newxresol;
    cout<<"insert new y resolution: "<<endl;
    cin>>newyresol;
    cout<<"insert image path: "<<endl;
    cin>>inpu;
    
    Mat img = imread(inpu);
    
    if (img.empty()) {
        cout << "please recheck image path";
        return 0;
    }
    //find energy
    Mat energymat = createnergy(img);
    //calculate cost
    Mat dp = dpmatof(energymat);
    //calculate seam
    double* seamj = nullptr;
    seamj = nextseamof(dp);

    int i = 0;
    int tot = img.cols - newxresol;
    while (newxresol < img.cols) {
        //find energy
        energymat = createnergy(img);
        //calculate cost
        dp = dpmatof(energymat);
        //calculate seam
        seamj = nextseamof(dp);

        for (long long spi = 0; spi < img.rows; ++spi) {
            long long i = spi;
            long long j = seamj[spi];
            //remove seam
            for (long long col = j; col < img.cols - 1; ++col) {
                img.at<Vec3b>(i, col) = img.at<Vec3b>(i, col + 1);
                energymat.at<Vec3b>(i, col) = energymat.at<Vec3b>(i, col + 1);
                dp.at<Vec3b>(i, col) = dp.at<Vec3b>(i, col + 1);
            }
        }
        delete[] seamj;
        //remove last column
        energymat = energymat(Rect(0, 0, energymat.cols - 1, energymat.rows));
        img = img(Rect(0, 0, img.cols - 1, img.rows));
        dp = dp(Rect(0, 0, dp.cols - 1, dp.rows));
        cout << "\x1b[2J\x1b[H";
        cout<<"carving columns:"<<((double)i++/(tot)) * 100<<"%, completed"<<endl;

    }
    
    //flip the image and continue the above
    transpose(img, img);
    flip(img, img, 1);



    i = 0;
    tot = img.cols - newyresol;
    while (newyresol < img.cols) {
        
        energymat = createnergy(img);

        dp = dpmatof(energymat);

        seamj = nextseamof(dp);

        for (long long spi = 0; spi < img.rows; ++spi) {
            long long i = spi;
            long long j = seamj[spi];

            if (i < 0 || i >= img.rows || j < 0 || j >= img.cols) {
                std::cerr << "Invalid pixel location." << std::endl;
                continue;
            }

            for (long long col = j; col < img.cols - 1; ++col) {
                img.at<Vec3b>(i, col) = img.at<Vec3b>(i, col + 1);
                energymat.at<Vec3b>(i, col) = energymat.at<Vec3b>(i, col + 1);
                dp.at<Vec3b>(i, col) = dp.at<Vec3b>(i, col + 1);
            }
        }
        delete[] seamj;

        energymat = energymat(Rect(0, 0, energymat.cols - 1, energymat.rows));
        img = img(Rect(0, 0, img.cols - 1, img.rows));
        dp = dp(Rect(0, 0, dp.cols - 1, dp.rows));
        
        i++;
        cout<<"carving rows:"<<((double)(i)/(tot)) * 100<<"%, completed"<<endl;
        cout << "\x1b[2J\x1b[H";
    }
    

    //flipping the image back
    transpose(img, img);
    flip(img, img, 1); 
    transpose(img, img);
    flip(img, img, 1); 
    transpose(img, img);
    flip(img, img, 1); 

    //display the image
    cv::namedWindow("Window1", cv::WINDOW_NORMAL);
    cv::imshow("Window1", img);
    cv::waitKey(0);
    cv::destroyAllWindows();

    cv::imwrite("output_image.jpg", img);
    return 0;
}

