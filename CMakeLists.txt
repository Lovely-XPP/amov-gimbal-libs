cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT_VERSION 0.2.0)
project(AmovGimbalCtrl VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_definitions(-DAPI_EXPORTS)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(CMAKE_GNUtoMS ON)
set(CXX_STANDARD 17)
if(UNIX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

add_subdirectory(lib/serial)
if(WIN32)
  add_subdirectory(lib/pthread-win32)
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
endif()

message(STATUS "System:${CMAKE_HOST_SYSTEM_PROCESSOR}")

# Public header
set(
  public_HEADS
  include/sv_gimbal.h
)

include_directories(
  src/AT10
  src/FIFO
  src/G1
  src/GX40 
  src/Q10f 
  include
)

# Gimbal Sources
set(driver_SRCS
  src/FIFO/Ring_Fifo.cpp
)
file(GLOB DRV_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/G1/*.cpp)
list(APPEND driver_SRCS ${DRV_LIB_FILES})
file(GLOB DRV_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/AT10/*.cpp)
list(APPEND driver_SRCS ${DRV_LIB_FILES})
file(GLOB DRV_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/Q10f/*.cpp)
list(APPEND driver_SRCS ${DRV_LIB_FILES})
file(GLOB DRV_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/GX40/*.cpp)
list(APPEND driver_SRCS ${DRV_LIB_FILES})
file(GLOB DRV_LIB_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(APPEND driver_SRCS ${DRV_LIB_FILES})

set(gimbal_SRCS
  sv_gimbal.cpp
  sv_gimbal_io.hpp
)

# Gimbal Lib
add_library(sv_gimbal SHARED ${driver_SRCS} ${gimbal_SRCS})
if(LINUX)
  target_link_libraries(sv_gimbal rt pthread serial)
endif()
if(APPLE)
  target_link_libraries(sv_gimbal pthread serial)
endif()
if(WIN32)
  target_link_libraries(sv_gimbal pthreadVC3 ws2_32 serial)
endif()
