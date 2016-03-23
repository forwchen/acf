cmake_minimum_required(VERSION 2.8)
PROJECT(ped_acf)


Find_package(OpenCV 2.0 REQUIRED)

set(OPT_CXX_FLAGS "-O3 -fopenmp -funroll-loops -march=native -mtune=native --fast-math -mfpmath=sse -msse3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DNDEBUG ${OPT_CXX_FLAGS}")

FILE(GLOB SRC
	"ACF/*.h"
	"ACF/*.cpp"
	"ACF/*.hpp")


include_directories(
	${OpenCV_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries( ${PROJECT_NAME}  
	${OpenCV_LIBRARIES}
)




