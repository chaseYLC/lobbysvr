ADD_DEFINITIONS(-DBoost_USE_STATIC_LIBS=ON)

#set(BOOST_ROOT "/usr/local/Cellar/boost/1.69.0_2")
set(BOOST_ROOT "/usr/local")
set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_MULTITHREADED      ON)

find_package(Boost 1.69.0 COMPONENTS log log_setup system thread REQUIRED)
#find_package(Boost 1.60.0 COMPONENTS log log_setup system thread REQUIRED)
if(Boost_FOUND)
    message(STATUS "---------------------")
    message(STATUS "Boost_FOUND: ${Boost_FOUND}")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "---------------------")

    include_directories(${Boost_INCLUDE_DIRS})
    include_directories("/usr/local/mln/include/")

    # file(GLOB SOURCES  *.cpp)
  
    # add_executable(lobbysvr
    #     ${SOURCES}
    # )
    add_executable(lobbysvr
        brokerConnector.cpp
        connectorHandler.cpp
        connectorUser.cpp
        environment.cpp
        keyEventHandler.cpp
        lobbyAcceptorReceiver.cpp
        lobbyUser.cpp
        lobbyUserManager.cpp
        main.cpp
        mlnUtils.cpp
        nh_chat.cpp
        nh_lobby.cpp
        packetParserJson.cpp
        stdafx.cpp
        userBasis.cpp
        userManagerBasis.cpp
        watchDogHandler.cpp
    )

    # target_link_libraries(lobbysvr /usr/local/mln/lib/libmln.a -static)
    # target_link_libraries(lobbysvr
    #     /usr/local/mln/lib/libmln.a
    #     ${Boost_LIBRARIES}
    # )
    target_link_libraries(lobbysvr
        mln
        ${Boost_LIBRARIES}
    )

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(JSONCPP jsoncpp)
    include_directories(${Jsoncpp_INCLUDE_DIR})
    link_libraries(${JSONCPP_LIBRARIES})
    target_link_libraries(lobbysvr ${JSONCPP_LIBRARIES})


    set_target_properties( lobbysvr
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
    
elseif()
    message("Boost NOT Found!")
endif()

