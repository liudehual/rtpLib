cmake_minimum_required(VERSION 2.8)
project(MDRtpLib)
include_directories(.)

SET(CMAKE_BUILD_TYPE "Debug") 
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb") 
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(SRC_LIST
	multiplexPs.cpp
	packet.cpp
	utils.cpp
	thread.cpp
	instance.cpp
	normalScheduler.cpp
	osHeap.cpp
	task.cpp
	timer.cpp
	socket.cpp
	socketHelper.cpp
	strPtr.cpp
	tcpSocket.cpp
	udpSocket.cpp
	server.cpp
	client.cpp
	log.cpp
	fastLog.cpp
	sdkManager.cpp
	rtpRtcpTask.cpp
	rtpInstance.cpp
	rtpLib.cpp
)
add_library(MDRtpLib ${SRC_LIST})
target_link_libraries(MDRtpLib pthread)
add_subdirectory(test)
