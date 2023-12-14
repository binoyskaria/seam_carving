##Seam Carving
```
Problem Statement : Apply seam carving content aware image-resizing algorithm on a given image. Take the height and width (in pixels) of the output image as inputs from the user.

What is Seam Carving?
        ◦ Seam-carving is a content-aware image resizing technique where the image is reduced in size by one pixel of height (or width) at a time.
        ◦ A vertical seam in an image is a path of pixels connected from the top to the bottom with one pixel in each row.
        ◦ A horizontal seam is a path of pixels connected from the left to the right with one pixel in each column.
        ◦ Steps:
            ▪ Energy Calculation: Each pixel has some RGB values. Calculate energy for each pixel. For ex.- You can use a dual-gradient energy function but you are free to use any energy function of your choice. Also, you can refer to this link for details.
            ▪ Seam Identification: Identify the lowest energy seam.
            ▪ Seam Removal: Remove the lowest energy seam.

Program Flow:
    1. Extract individual pixel’s RGB values from the sample image. Load the RGB values in a 3D matrix (Height x Width x 3).
    2. Apply seam carving algorithm.
    3. Generate sample image output using the RGB values for resized image (New_Height x New_Width x 3).

```

How to Run

Update your package list:
```
sudo apt update
```
Install the necessary packages for building OpenCV:
```
sudo apt install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
```
Install additional image and video I/O libraries:
```
sudo apt install libjpeg-dev libpng-dev libtiff-dev libjpeg8-dev libjpeg-turbo8-dev libtiff5-dev libjasper-dev libdc1394-22-dev libgstreamer-plugins-base1.0-dev libavresample-dev
```
Install Python and the necessary packages for Python bindings (if needed):
```
sudo apt install python3-dev python3-pip python3-numpy
```
Clone the OpenCV repository from GitHub:
```
git clone https://github.com/opencv/opencv.git
```
Create a build directory and navigate to it:
```
cd opencv
mkdir build
cd build
```
Run CMake to configure the build (adjust options as needed):
/usr/local - this depends on the system final include path for some systems is /usr/include/opencv2/opencv.hpp
```
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local ..
```
Compile OpenCV:
```
make -j$(nproc)
```

Install OpenCV system-wide:
```
sudo make install
```
you should be able to include the <opencv2/opencv.hpp>

you can compile your code using the following command:
```
g++ seam_carving.cpp -o seam_carving `pkg-config --cflags --libs opencv4`
```


