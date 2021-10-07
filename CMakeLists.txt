cmake_minimum_required(VERSION 3.10)

project(Client VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(include)
file(GLOB SOURCES "./*.cpp")

find_package(Boost REQUIRED system)
find_package(OpenCV REQUIRED )
INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

add_executable(Client ${SOURCES})


target_link_libraries(Client ${Boost_LIBRARIES})
target_link_libraries( Client ${OpenCV_LIBS} )