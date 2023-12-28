cmake_minimum_required(VERSION 3.14)
set(PROJECT_NAME DbgAnalyzer)
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(${PROJECT_NAME} SHARED
  ${SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CORE_ROOT}/src
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core
    Qt5::Widgets
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    CORE_LIBRARY
)

install(TARGETS ${TARGET_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
