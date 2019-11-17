set(BOOST_ROOT "/usr/local")

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
# set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
# set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
# set(Boost_USE_STATIC_RUNTIME    OFF)

SET ( CMAKE_INSTALL_PREFIX "/usr/local/mln" )

set(CMAKE_INSTALL_DEFAULT_DIRECTORY_PERMISSIONS
        OWNER_READ
        OWNER_WRITE
        OWNER_EXECUTE
        GROUP_READ
    )

# SET ( CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type (default Debug)" )
# SET ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE} )
# SET ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib )
# SET ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BUILD_TYPE}/lib )


find_package(Boost 1.69.0 COMPONENTS log log_setup system thread REQUIRED)
if(Boost_FOUND)
    message(STATUS "---------------------")
    message(STATUS "Boost_FOUND: ${Boost_FOUND}")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "---------------------")

    include_directories(${Boost_INCLUDE_DIRS})
    #target_link_libraries(test ${Boost_LIBRARIES})
elseif()
    message("Boost NOT Found!")
endif()

# file(GLOB SOURCES  *.cpp)
add_library(mln STATIC
    stdafx.cpp
    connectionImpl.cpp
    logger.cpp
    messageBuffer.cpp
    messageProcedure.cpp
    netService.cpp
    netServiceAcceptor.cpp
    netServiceConnector.cpp
    netServiceImpl.cpp
    netUtil.cpp
    packetParserSimple.cpp
    test_interface.cpp
    timeUtil.cpp
)

install(TARGETS mln DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(FILES connection.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES connectionImpl.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES eventReceiver.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES logger.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES macros.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES memoryPool.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES messageBuffer.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES messageProcedure.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES netService.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES netServiceAcceptor.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES netServiceConnector.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES netServiceImpl.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES netUtil.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES packetEncType.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES packetParserSimple.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES singleton.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES stdafx.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES test_interface.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES threadStaticValue.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES timeUtil.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)
install(FILES xmlParsingSupport.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include/net)

install(FILES ../mln_net_constants.dat DESTINATION ${CMAKE_INSTALL_PREFIX}/include)


# install(TARGETS mln
#         INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include
#         RUNTIME  DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
#         LIBRARY  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
#         ARCHIVE  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
# )


