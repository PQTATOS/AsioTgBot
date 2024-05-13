cmake_minimum_required(VERSION 3.25)

project(AsioTgBotPrj)

set(network_src
	"network/src/dispatcher.cpp"
	"network/src/poller.cpp"
	"network/src/request.cpp"
	"network/src/socket.cpp"
	"network/src/tgbot.cpp"
)
set(tg_types_src "tg_types/src/message.cpp")

#find_package(Boost 1.84.0 REQUIRED COMPONENTS asio)
find_package(OpenSSL REQUIRED)

add_library(AsioTgBot STATIC ${tg_types_src} ${network_src})

target_include_directories(AsioTgBot PUBLIC tg_types/include)
target_include_directories(AsioTgBot PUBLIC network/include)
target_link_libraries(AsioTgBot ${OPENSSL_LIBRARIES})
target_include_directories(AsioTgBot PUBLIC ${OPENSSL_INCLUDE_DIR})
