cmake_minimum_required(VERSION 3.14)

project(learning)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

include(FetchContent)
FetchContent_Declare(
	googletest
	GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG main
)
FetchContent_Declare(
	glog
	GIT_REPOSITORY https://github.com/google/glog.git
	GIT_TAG master
)
FetchContent_MakeAvailable(glog googletest)

add_executable(bench bench/main.cc)
target_link_libraries(bench glog::glog)

enable_testing()
add_executable(utils_test utils.cc utils_test.cc)
target_link_libraries(utils_test gtest_main)

add_executable(median_of_two_arrays_test probs/median-of-two-sorted-arrays/main.cc probs/median-of-two-sorted-arrays/main_test.cc)
target_link_libraries(median_of_two_arrays_test gtest_main)

include(GoogleTest)
gtest_discover_tests(utils_test median_of_two_arrays_test)
