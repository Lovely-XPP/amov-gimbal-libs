cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
cmake_policy(SET CMP0054 NEW)

set(PROJECT_VERSION 0.2.0)
project(AmovGimbalCtrl VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_definitions(-DAPI_EXPORTS)
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_subdirectory(lib/serial)
if(WIN32)
  add_subdirectory(lib/pthread-win32)
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /w")
endif()

## JETSON, X86_CUDA, X86_INTEL
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
add_library(sv_gimbal STATIC ${driver_SRCS} ${gimbal_SRCS})
if(UNIX)
  target_link_libraries(sv_gimbal rt pthread serial)
endif(UNIX)
if(WIN32)
  target_link_libraries(sv_gimbal pthreadVC3 ws2_32 serial)
endif()
