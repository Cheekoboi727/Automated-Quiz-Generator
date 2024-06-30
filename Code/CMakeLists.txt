cmake_minimum_required(VERSION 3.10)

project(DSA)

add_executable(Project CodeFile.cpp)

target_link_libraries(Project D:/libharu-libharu-0c598be/src/libhpdf.a)

include_directories(D:/libharu-libharu-0c598be/include)
