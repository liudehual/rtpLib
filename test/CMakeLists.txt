project(test)
include_directories(.)
include_directories(..)
set(SRC_LIST
	main.cpp
)
add_executable(main ${SRC_LIST})
target_link_libraries(main pthread MDRtpLib)

#set(TEST_MULTIPLEX_PS_SRC_LIST
#	test_multiplexPs.cpp
#	../multiplexPs.cpp
#)
#add_executable(test_multiplexPs ${TEST_MULTIPLEX_PS_SRC_LIST})
#target_link_libraries(test_timer_scheduler pthread MDRtpLib)


#set(TEST_TIMER_SCHEDULER_SRC_LIST
#	test_timer_scheduler.cpp
#	../timer.cpp
#	../normalScheduler.cpp
#	../osHeap.cpp
#	../task.cpp
#	../log.cpp
#	../sdkManager.cpp
#	../thread.cpp
#	../utils.cpp
#	../fastLog.cpp
#	../rtpInstance.cpp
#	../instance.cpp
#)
#add_executable(test_timer_scheduler ${TEST_TIMER_SCHEDULER_SRC_LIST})
#target_link_libraries(test_timer_scheduler pthread)