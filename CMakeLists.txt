cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME AnalyzerDemo)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CORE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/lib/libCore)
set(QWT_ROOT  ${CMAKE_CURRENT_SOURCE_DIR}/lib/3rdparty/qwt-6.1.5)

#spdlog
set(SPDLOG_MASTER_PROJECT OFF)
set(SPDLOG_INSTALL OFF)
set(SPDLOG_BUILD_SHARED OFF)
set(SPDLOG_WCHAR_FILENAMES ON)

#rpath
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib;\$ORIGIN/../lib64")

#linker
set(LINK_LIBRARIES
    -static-libgcc
    -static-libstdc++
)

#libraries
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/libCore)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/analyzers/libSpAnalyzer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/analyzers/libTcpAnalyzer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/analyzers/libDbgAnalyzer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/3rdparty/qwt-6.1.5)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/3rdparty/spdlog-1.9.2)

#qt packages
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} WIN32
  ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${QWT_ROOT}/src
    ${CORE_ROOT}/src
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
    Core
    Qwt
    spdlog
    ${LINK_LIBRARIES}
)

install(TARGETS ${TARGET_NAME}
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
