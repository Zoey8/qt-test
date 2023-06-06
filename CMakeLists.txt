cmake_minimum_required(VERSION 3.22)
project(qt_test VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Multimedia)
add_definitions(-DQT_NO_KEYWORDS)
set(QT_REQUIRE_LIB
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Multimedia
)

set(WEBRTC_DIR ${CMAKE_SOURCE_DIR}/third_party/webrtc)
include_directories(${WEBRTC_DIR}/include)
add_library(WEBRTC STATIC IMPORTED)
set_target_properties(WEBRTC PROPERTIES IMPORTED_LOCATION ${WEBRTC_DIR}/lib/libwebrtc.a)
set(WEBRTC_THIRD_PARTY_INCLUDE_DIRS
    ${WEBRTC_DIR}/include/third_party/abseil-cpp
    ${WEBRTC_DIR}/include/third_party/libyuv/include
)
include_directories(${WEBRTC_THIRD_PARTY_INCLUDE_DIRS})
add_definitions(-DWEBRTC_POSIX)

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    find_library(CORE_AUDIO NAMES CoreAudio)
    find_library(AUDIO_TOOL_BOX NAMES AudioToolbox)
    set(WEBRTC_REQUIRED_LIB ${CORE_AUDIO} ${AUDIO_TOOL_BOX})
endif()

file(GLOB_RECURSE PROJECT_SOURCES src/*.cc include/*.h)
add_executable(helloworld main.cc ${PROJECT_SOURCES})
target_link_libraries(helloworld PRIVATE WEBRTC ${QT_REQUIRE_LIB} ${WEBRTC_REQUIRED_LIB})

if(CMAKE_BUILD_TYPE MATCHES Release)
    set_target_properties(helloworld PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER qt.zoey.cloud
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
    install(TARGETS helloworld
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
endif()
