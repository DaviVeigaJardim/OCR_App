cmake_minimum_required(VERSION 3.10)
project(OCR_Project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(/usr/include/tesseract)
include_directories(/usr/include/leptonica)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

add_executable(main main.cpp OcrApp.cpp)

target_link_libraries( main 
    ${OpenCV_LIBS} 
    Qt5::Widgets 
    tesseract 
    lept 
    archive 
    curl
)