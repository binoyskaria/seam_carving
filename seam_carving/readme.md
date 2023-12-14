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
