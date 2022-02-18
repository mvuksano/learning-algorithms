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
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(utils_test utils.cc utils_test.cc)
target_link_libraries(utils_test gtest_main)

include(GoogleTest)
gtest_discover_tests(utils_test)
