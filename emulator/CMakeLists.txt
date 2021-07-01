CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(PROJECT_NAME qchip8)
PROJECT(${PROJECT_NAME})

SET(CMAKE_CXX_STANDARD 17)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON)

FIND_PACKAGE(Qt5 COMPONENTS Widgets REQUIRED)

INCLUDE_DIRECTORIES(includes)

ADD_EXECUTABLE(${PROJECT_NAME}
    src/mainwindow.ui
    src/editorwindow.ui
    src/main.cpp
    src/mainwindow.cpp
    src/editorwindow.cpp
    src/cpu.cpp
    src/registerset.cpp
    src/is.cpp
    src/memory.cpp
    src/emulatorworker.cpp
    includes/mainwindow.h
    includes/editorwindow.h
    includes/cpu.h
    includes/is.h
    includes/memory.h
    includes/datatypes.h
    includes/emulatorworker.h
    includes/registerset.h
)

TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC "../compiler/lib/includes")
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Qt5::Widgets compiler_lib)
