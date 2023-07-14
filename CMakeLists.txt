cmake_minimum_required(VERSION 3.13.4)
project(my_project C)

add_subdirectory(foo)
add_subdirectory(bar)

add_library(foo foo.c)
target_include_directories(foo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)

add_executable(foo_test foo_tests.c)
target_link_libraries(foo_test foo)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)

