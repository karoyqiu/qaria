# 主 CMakeLists.txt 文件
cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_VISIBILITY_PRESET "hidden")
set(CMAKE_CXX_VISIBILITY_PRESET "hidden")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
set(CMAKE_SKIP_BUILD_RPATH OFF)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

# 添加项目的模块路径
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

option(BUILD_SHARED_LIBS "If true, this will cause all libraries to be built shared unless the library was explicitly added as a static library.")
option(BUILD_TESTING "Build the testing tree." OFF)

project(qaria VERSION 0.0.1.1 LANGUAGES CXX)

# Release 版本开启 IPO
include(CheckIPOSupported OPTIONAL)
if(COMMAND check_ipo_supported)
    check_ipo_supported(RESULT PROJECT_IS_IPO_SUPPORTED)

    if(PROJECT_IS_IPO_SUPPORTED)
        message(STATUS "IPO is supported.")
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    endif()
endif()

# 查找第三方库：Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Widgets LinguistTools REQUIRED)

include(AddQtExecutable)

# 添加源代码子目录
add_subdirectory(src)
