# rm -rf build
# cmake -S . -B build -DCMAKE_CXX_COMPILER=g++-10 -DCMAKE_BUILD_TYPE=Release
# cmake --build ./build

# To run LFU cache tests:
# cmake --build ./build --target test ARGS="-C LFUTest"
# To run perfect cache tests:
# cmake --build ./build --target test ARGS="-C PerfectTest"

cmake_minimum_required(VERSION 3.11)
project(HWC)

set(LC_SRCS
	${CMAKE_SOURCE_DIR}/LFU/src/LFU_Cache.cpp
)
set(PC_SRCS
	${CMAKE_SOURCE_DIR}/perfect_caching/src/perfect_caching.cpp
)
add_executable(LC.x) # LFU cache.
add_executable(PC.x) # Perfect cache.

target_sources(LC.x PRIVATE ${LC_SRCS})
target_include_directories(LC.x PUBLIC "${CMAKE_SOURCE_DIR}/LFU/include")
target_compile_features(LC.x PUBLIC cxx_std_20)

target_sources(PC.x PRIVATE ${PC_SRCS})
target_include_directories(PC.x PUBLIC "${CMAKE_SOURCE_DIR}/perfect_caching/include")
target_compile_features(PC.x PUBLIC cxx_std_20)

enable_testing()
set(LC_TESTS "${CMAKE_SOURCE_DIR}/LCTests")

file(GLOB testfiles "${LC_TESTS}/*.dat")
foreach(file ${testfiles})
  add_test(
    NAME ${file}
    CONFIGURATIONS LFUTest
    COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ./LC.x"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(${file} PROPERTIES DEPENDS LC.x)
endforeach()

set(PC_TESTS "${CMAKE_SOURCE_DIR}/PCTests")

file(GLOB testfiles "${PC_TESTS}/*.dat")
foreach(file ${testfiles})
	add_test(
		NAME ${file}
		CONFIGURATIONS PerfectTest
		COMMAND bash -c "${CMAKE_SOURCE_DIR}/runtest.sh ${file} ./PC.x"
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set_tests_properties(${file} PROPERTIES DEPENDS PC.x)
endforeach()
