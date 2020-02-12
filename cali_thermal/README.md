## DEPENDENCIES
1. CMake >= 2.8.3
2. OpenCV >= 3.0
3. ROS/tf

## STRUCTURE ORGANIZATION
- libcbdetect: C++ implmentation for chessboard detection from [this repo](https://github.com/ftdlyc/libcbdetect)
- align_thermal_to_rgb: align thermal image to RGB image (in this case, Intel RealSense D435) with the help of chessboard
  - include
    - cali
      - align_thermal_to_rgb.h: class definition for `align_thermal_to_rgb`
  - src
    - helper.hpp: some helper functions
    - align_thermal_to_rgb.cpp: class implementation for `align_thermal_to_rgb`
    - get_extrinsic.cpp: intrinsic/ extrinsic calibration for thermal camera, user provideds **intrinsic of rgb and thermal cameras, rgb, depth and thermal images**, and **an interger for mean filter kernel size**, then it will generate a file with **thermal camera intrinsic, distortion coefficients** and **rgb-to-thermal extrinsic**, named in `camera_model.txt`
    - example.cpp: `align_thermal_to_rgb` usage example
  - bin
    - get_extrinsic -> src/get_extrinsic.cpp: get thermal intrinsic and rgb-to-thermal extrinsic parameters with images with chessboard inside
      - ./get_extrinsic [path_to_rgb_intrisic_file] [path_to_thermal_intrinsic_file] [path_to_rgb_image] [path_to_depth_image] [path_to_thermal_image] [kernel_size]
    - example -> src/example.cpp: test calibrated camera model from `get_extrinsic` result
      - ./example [path_to_camera_model_file] [path_to_rgb_intrisic_file] [path_to_rgb_image] [path_to_depth_image] [path_to_thermal_image]
  - lib
    - libalign_thermal_to_rgb.so: library for `align_thermal_to_rgb`, link this library in your own source codes
  - result: some concrete alignment result
  - example_input: example input for `get_extrinsic` execution
  - CMakeLists.txt
 
## BUILD
Since the `align_thermal_to_rgb` should do chessboard detection, so we have to build the `libcbdetect` first  
  1. cd && git clone https://github.com/sean85914/cali_thermal.git  
  2. cd cali_thermal/libcbdetect  
  3. mkdir devel && cd devel && cmake ../ && make  
  **(Note: If you want to show more detail information, use cmake ../ -DCMAKE_BUILD_TYPE=Debug then make)**
then we can build the `align_thermal_to_rgb`  
  4. cd ~/cali_thermal/align_thermal_to_rgb  
  5. mkdir devel && cd devel && cmake ../ && make && cd ../bin  
  6. Then, if you have data for testing (**i.e., chessboard in the image**), run  
  `./get_extrinsic [path_to_rgb_intrisic_file] [path_to_thermal_intrinsic_file] [path_to_rgb_image] [path_to_depth_image] [path_to_thermal_image] 7`  
    Then you will get the output file **camera_model.txt**, an **aligned thermal image** and a **combined one**
  7. With this file, you can use `align_thermal_to_rgb` class, for example,
  `./example [path_to_output_txt] [path_to_rgb_intrinsic] [path_to_rgb_image] [path_to_depth_image] [path_to_thermal_image]`

## RESULT
![image](https://github.com/sean85914/align_thermal_to_rgb/blob/master/align_thermal_to_rgb/result/result.jpg)
Sample output:  
``
intrinsic: [606.526, 625.874, 283.076, 247.947]
``
  
``
distortion: [0.144775, -32.6391, 0.0255662, 0.0092998, 274.744]
``
  
``
quaternion: [-0.0151522, 0.0415791, -0.0190183, 0.998839]
``
  
``
origin: [-0.0579184, 0.0172561, 0.119621]
``