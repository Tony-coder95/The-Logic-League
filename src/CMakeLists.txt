cmake_minimum_required(VERSION 3.10)
project(WalmartSalesFLTK)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)

# Specify FLTK paths
set(FLTK_INCLUDE_DIR /opt/homebrew/Cellar/fltk/1.4.0/include)
set(FLTK_LIBRARIES /opt/homebrew/Cellar/fltk/1.4.0/lib)

# Add executable
add_executable(WalmartSalesFLTK main.cpp)

# Include and link FLTK
target_include_directories(WalmartSalesFLTK PUBLIC ${FLTK_INCLUDE_DIR})
target_link_directories(WalmartSalesFLTK PUBLIC ${FLTK_LIBRARIES})
target_link_libraries(WalmartSalesFLTK fltk fltk_images fltk_forms)
