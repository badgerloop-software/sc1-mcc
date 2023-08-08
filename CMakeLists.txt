cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET sc1-mcc)
set(INCLUDE_DIR include)
set(SRC_DIR src)
set(HEADER_FILES
    ${INCLUDE_DIR}/analog.h
    ${INCLUDE_DIR}/can.h
    ${INCLUDE_DIR}/common.h
    ${INCLUDE_DIR}/drive.h
    ${INCLUDE_DIR}/gpio.h
    ${INCLUDE_DIR}/mc_status.h
    ${INCLUDE_DIR}/monitor.h
)
set(SRC_FILES
    ${SRC_DIR}/analog.cpp
    ${SRC_DIR}/can.cpp
    ${SRC_DIR}/drive.cpp
    ${SRC_DIR}/gpio.cpp
    ${SRC_DIR}/mc_status.cpp
    ${SRC_DIR}/monitor.cpp
)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET} main.cpp ${SRC_FILES} ${HEADER_FILES})

target_include_directories(${APP_TARGET} PRIVATE ${INCLUDE_DIR} ${SRC_DIR})

target_sources(${APP_TARGET}
    PRIVATE
        main.cpp
)

target_link_libraries(${APP_TARGET}
    PRIVATE
        mbed-os
)

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
