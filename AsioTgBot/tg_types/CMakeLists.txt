cmake_minimum_required(VERSION 3.25)

project(TG_TYPES VERSION 1.0)

set(src_files "src/message.cpp")

add_library(tg_types STATIC ${src_files})

target_include_directories(tg_types PUBLIC include)

