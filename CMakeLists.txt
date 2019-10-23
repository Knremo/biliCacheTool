cmake_minimum_required(VERSION 3.2)

project(bilicacheconvert)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories("ffmpeg/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/lib")

#link_libraries(libavformat.dll.a)
#link_libraries(libavutil.dll.a)

add_executable(${PROJECT_NAME} bilicacheconvert.cpp ffmpeg_mux.cpp)
target_link_libraries(${PROJECT_NAME} libavformat libavutil libavcodec)