cmake_minimum_required(VERSION 3.5)

set(NAME_PROJECT DEX_Network.out)
set(CMAKE_CXX_STANDARD 11)

project(${NAME_PROJECT}
    LANGUAGES CXX C)

add_executable(${NAME_PROJECT} ./graph-without-weight-maker.cpp)
target_sources(${NAME_PROJECT} PRIVATE ./graph-without-weight-maker.cpp)

add_executable(Info_edges.out ./info-edges-maker.cpp)
target_sources(Info_edges.out PRIVATE ./info-edges-maker.cpp)

target_link_libraries(${NAME_PROJECT} 
    SwapsReader
)

target_link_libraries(Info_edges.out
    SwapsReader
)

set (LIBRARY_OUTPUT_PATH ./build)

add_library(SwapsReader STATIC ./SwapsReader.cpp)
