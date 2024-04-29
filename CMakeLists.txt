cmake_minimum_required(VERSION 3.25)

project(test)

add_subdirectory(AsioTgBot)

add_executable(test main.cpp)

target_link_libraries(test AsioTgBot)
