cmake_minimum_required(VERSION 2.8)
project( Wall_Detect )
find_package( OpenCV REQUIRED )
add_executable( Wall_Detect Wall_Detect.cpp )
target_link_libraries( Wall_Detect ${OpenCV_LIBS})
