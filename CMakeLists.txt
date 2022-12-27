cmake_minimum_required(VERSION 3.10)

project(ChatService)

SET(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


set(SRC_LIST 
main.cpp
ChatService.cpp 
ClientUser.cpp
RoomMgr.cpp
ChatRoom.cpp
RoomIDMgr.cpp
)

include_directories("include")

#add_compile_definitions(_GLIBCXX_HAS_GTHREADS)

add_executable(ChatService ${SRC_LIST})
#add_library(ServiceBase STATIC ${SRC_LIST})


if (WIN32)

    set(link_LIBRARIES
        ${PROJECT_SOURCE_DIR}/lib/libServiceBase.a
        WSock32
        WS2_32
    )
    target_link_libraries(ChatService ${link_LIBRARIES})

else()
    set(link_LIBRARIES ${PROJECT_SOURCE_DIR}/lib/libServiceBase.a)
    target_link_libraries(ChatService ${link_LIBRARIES})

endif()

