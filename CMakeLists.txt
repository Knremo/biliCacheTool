cmake_minimum_required(VERSION 2.8)

project(biliconvert)

set(CXX_STANDARD_REQUIRED 11)

include_directories("ffmpeg/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/lib")

#link_libraries(libavformat.dll.a)
#link_libraries(libavutil.dll.a)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} libavformat libavutil)