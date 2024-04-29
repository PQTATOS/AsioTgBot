cmake_minimum_required(VERSION 3.25)

project(NETWORK)

set(src_files 
	"src/dispatcher.cpp"
	"src/poller.cpp"
	"src/request.cpp"
	"src/socket.cpp"
	"src/tgbot.cpp"
)

add_library(network STATIC ${src_files})

target_include_directories(network PUBLIC include)



