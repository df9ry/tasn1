cmake_minimum_required(VERSION 3.14)

project(tasn1 LANGUAGES C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(AFTER SYSTEM "/usr/local/${CMAKE_BUILD_TYPE}/include")

set(SOURCES
  tasn1.c
)

set(HEADERS
  tasn1.h
)

add_library(tasn1 STATIC
  ${SOURCES}
  ${HEADERS}
)

set_target_properties(tasn1 PROPERTIES
  PUBLIC_HEADER tasn1.h)

target_compile_definitions(tasn1 PRIVATE TASN1_LIBRARY)

install(TARGETS tasn1
  LIBRARY DESTINATION       "${CMAKE_BUILD_TYPE}/lib"
  ARCHIVE DESTINATION       "${CMAKE_BUILD_TYPE}/lib"
  PUBLIC_HEADER DESTINATION "${CMAKE_BUILD_TYPE}/include")

set(APP_EXE tasn1_test)
add_executable(${APP_EXE} tasn1_test.c)
target_link_libraries(${APP_EXE} tasn1)
add_test(Test, ${APP_EXE})