cmake_minimum_required(VERSION 2.8 FATAL_ERROR )

project(Object_detection)

#set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

enable_testing()

if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
      message(FATAL_ERROR "FATAL: In-source builds are not allowed. Create a separate directory for build files.")
endif()

if  (CMAKE_BUILD_TYPE STREQUAL Debug)
    set(BUILD_POSTFIX "_d")
else(CMAKE_BUILD_TYPE STREQUAL Release)
    set(BUILD_POSTFIX "")
endif()

#SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

message("CMAKE_MODULE_PATH is ${CMAKE_MODULE_PATH}")

if (MSVC)
    if(MSVC_VERSION LESS 1800) # 1700 = VS 2013
       message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
else()
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    else()
       	message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
     endif()
endif()

#message ("CMAKE_SOURCE_DIR : " ${CMAKE_SOURCE_DIR})
#message ("CMAKE_BINARY_DIR : " ${CMAKE_BINARY_DIR})

SET (OpenCV_DIR "D:/CPP_Library/opencv/GccBuild")
find_package( OpenCV REQUIRED)
if(OpenCV_FOUND)
   include_directories(${OpenCV_INCLUDE_DIRS})
   message(STATUS "Found OpenCV")
endif()

SET (BOOST_ROOT "D:/CPP_Library/boost_1_69_0/boost_1_69_0")
SET (BOOST_INCLUDEDIR "D:/CPP_Library/boost_1_69_0/boost_1_69_0/gccbuild/include/boost-1_69")
SET (BOOST_LIBRARYDIR "D:/CPP_Library/boost_1_69_0/boost_1_69_0/gccbuild/lib")
SET (Boost_USE_MULTITHREADED ON)
SET (Boost_USE_STATIC_LIBS ON)
SET (Boost_USE_STATIC_RUNTIME OFF)
#SET (Boost_DEBUG on)
if  (MSVC)
    find_package(Boost 1.69.0 REQUIRED)
    if(Boost_FOUND)
       include_directories(${Boost_INCLUDE_DIRS})
       link_directories(${BOOST_LIBRARYDIR})
       message(STATUS "Found BOOST")
    endif()
else()
    find_package(Boost 1.69.0 REQUIRED COMPONENTS system thread filesystem serialization REQUIRED )
	if(Boost_FOUND)
       include_directories(${Boost_INCLUDE_DIRS})
       link_directories(${BOOST_LIBRARYDIR})
       message(STATUS "Found BOOST")
    endif()
endif()

#SET (dlib_DIR "D:/CPP_Library/Dlib/dlib-19.12/GccBuild/install/lib/cmake/dlib")
SET (dlib_DIR "D:/CPP_Library/dlib-19.15/gccBuild/install/lib/cmake/dlib")
find_package(dlib REQUIRED)
if (dlib_FOUND)
    message(STATUS "Found DLIB ")
	#include_directories( ${dlib_INCLUDE_DIRS} )       
endif ()	

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB SOURCES ProjectProperty/src/*.cpp 
     DBModule/src/*.cpp 
     FeatureExtraction/src/*cpp )
     #FeatureMatch/src/*cpp)

add_library(libobjdtect STATIC ${SOURCES})

#set_target_properties(libobjdtect PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(libobjdtect PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set_target_properties(libobjdtect PROPERTIES OUTPUT_NAME libobjdtect${BUILD_POSTFIX})
if (MSVC)
    target_link_libraries( libobjdtect ${OpenCV_LIBS})
    #target_link_libraries( libobjdtect ${Boost_iostreams_LIBRARY})
    #target_link_libraries( libobjdtect ${Boost_date_time_LIBRARY})
    #target_link_libraries( libobjdtect ${Boost_signals_LIBRARY})
    #target_link_libraries( libobjdtect dlib::dlib)
else()
    target_link_libraries( libobjdtect ${OpenCV_LIBS} ${Boost_LIBRARIES})
    #target_link_libraries( libobjdtect ${Boost_LIBRARIES} )
    target_link_libraries( libobjdtect ${Boost_system_LIBRARY})
    target_link_libraries( libobjdtect ${Boost_thread_LIBRARY})
    target_link_libraries( libobjdtect ${Boost_serialization_LIBRARY})
    target_link_libraries( libobjdtect ${Boost_FILESYSTEM_LIBRARY})
    target_link_libraries( libobjdtect dlib::dlib)
    TARGET_LINK_LIBRARIES(libobjdtect dlib::dlib)    
endif()
add_subdirectory( testModule )
add_executable(Object_detection main.cpp)
target_link_libraries(Object_detection libobjdtect)
#set_target_properties(Object_detection PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Object_detection PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
set_target_properties(Object_detection PROPERTIES OUTPUT_NAME Object_detection${BUILD_POSTFIX})