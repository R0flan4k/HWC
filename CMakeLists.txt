# rm -rf build
# cmake -S . -B build
# cmake --build ./build

cmake_minimum_required(VERSION 3.11)
project(HWC)
set(CMAKE_CXX_FLAGS "-O2 -std=c++20")
set(CMAKE_CXX_COMPILER "g++-10")

set(LC_SRCS
	${CMAKE_SOURCE_DIR}/LFU/src/LFU_Cache.cpp
)
set(PC_SRCS
	${CMAKE_SOURCE_DIR}/perfect_caching/src/perfect_caching.cpp
)
add_executable(LC.x)
add_executable(PC.x)

target_sources(LC.x PRIVATE ${LC_SRCS})
target_include_directories(LC.x PUBLIC "${CMAKE_SOURCE_DIR}/LFU/include")
target_sources(PC.x PRIVATE ${PC_SRCS})
target_include_directories(PC.x PUBLIC "${CMAKE_SOURCE_DIR}/perfect_caching/include")
