# CMakeLists.txt

# Older versions of CMake are likely to work just fine but, since
# I don't know where to cut off I just use the version I'm using
cmake_minimum_required(VERSION "3.17")

# name of this example project
project(edge-detection)

# set OpenCV_DIR variable equal to the path to the cmake
# files within the previously installed opencv program
set(OpenCV_DIR /Users/ariakillebrewbruehl/Documents/thesis/senior-thesis/opencv/install/lib/cmake/opencv4)

# Tell compiler to use C++ 14 features which is needed because
# Clang version is often behind in the XCode installation
set(CMAKE_CXX_STANDARD 14)

# configure the necessary common CMake environment variables
# needed to include and link the OpenCV program into this
# demo project, namely OpenCV_INCLUDE_DIRS and OpenCV_LIBS
find_package( OpenCV REQUIRED )

# tell the build to include the headers from OpenCV
include_directories( ${OpenCV_INCLUDE_DIRS} )

# specify the executable target to be built
add_executable(tests tests.cpp 
    extract-edges.cpp canny.cpp threshold-edges.cpp skeleton.cpp
     ../boiler-plate/read-save.cpp ../boiler-plate/type2str.cpp ../boiler-plate/invert.cpp ../boiler-plate/CIEprocess.cpp)

# tell it to link the executable target against OpenCV
target_link_libraries(tests ${OpenCV_LIBS})
