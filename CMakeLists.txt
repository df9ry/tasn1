cmake_minimum_required(VERSION 3.14)

project(tasn1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(AFTER SYSTEM "/usr/local/${CMAKE_BUILD_TYPE}/include")

set(SOURCES
  tasn1.c

  array.cpp
  node.cpp
  map.cpp
  number.cpp
  octetsequence.cpp
)

set(HEADERS
  tasn1.h

  tasn1/types.h
  tasn1/array.hpp
  tasn1/node.hpp
  tasn1/map.hpp
  tasn1/number.hpp
  tasn1/octetsequence.hpp
)

add_library(tasn1 STATIC
  ${SOURCES}
  ${HEADERS}
)

target_compile_features(tasn1
    PUBLIC cxx_std_17)

target_include_directories(tasn1 PUBLIC
    BEFORE "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/include/")

set_target_properties(tasn1 PROPERTIES
  PUBLIC_HEADER "${HEADERS}"
)

target_compile_definitions(tasn1 PRIVATE TASN1_LIBRARY)

install(TARGETS tasn1
  LIBRARY DESTINATION       "${CMAKE_BUILD_TYPE}/lib"
  ARCHIVE DESTINATION       "${CMAKE_BUILD_TYPE}/lib"
  PUBLIC_HEADER DESTINATION "${CMAKE_BUILD_TYPE}/include/tasn1")

set(APP_EXE tasn1_test)
add_executable(${APP_EXE} tasn1_test.cpp)
target_include_directories(${APP_EXE} PUBLIC
    BEFORE "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/include/")
target_link_directories(${APP_EXE} PUBLIC
    BEFORE "${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE}/lib/")
target_link_libraries(${APP_EXE}
    tasn1
    JsonX)
add_test(Test, ${APP_EXE})
