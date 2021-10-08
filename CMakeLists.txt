cmake_minimum_required(VERSION 3.14)

project(j2asn1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
  j2asn1.c
)

set(HEADERS
  j2asn1.h
)

add_library(j2asn1 STATIC
  ${SOURCES}
  ${HEADERS}
)

target_compile_definitions(j2asn1 PRIVATE J2ASN1_LIBRARY)
