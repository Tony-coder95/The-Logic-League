cmake_minimum_required(VERSION 3.10)
project(WalmartSalesFLTK)

# Set C++ version
set(CMAKE_CXX_STANDARD 20)

# Specify FLTK paths
set(FLTK_INCLUDE_DIR /opt/homebrew/Cellar/fltk/1.4.0/include)
set(FLTK_LIBRARIES /opt/homebrew/Cellar/fltk/1.4.0/lib)

# Specify Oracle Instant Client SDK paths
set(OCI_INCLUDE_DIR /Users/tonymolina/Oracle/instantclient/sdk/include)
set(OCI_LIBRARIES /Users/tonymolina/Oracle/instantclient)

# Add executable
add_executable(WalmartSalesFLTK main.cpp)

# Include and Link Libraries
target_include_directories(WalmartSalesFLTK PUBLIC ${FLTK_INCLUDE_DIR} ${OCI_INCLUDE_DIR})
target_link_directories(WalmartSalesFLTK PUBLIC ${FLTK_LIBRARIES} ${OCI_LIBRARIES})
target_link_libraries(WalmartSalesFLTK fltk fltk_images fltk_forms occi clntsh)
