cmake_minimum_required(VERSION 3.25)

project(AsioTgBotPrj)

add_subdirectory(network)
add_subdirectory(tg_types)

add_library(AsioTgBot INTERFACE)

target_link_libraries(AsioTgBot INTERFACE tg_types network)
