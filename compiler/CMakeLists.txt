CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(PROJECT_NAME compiler)
PROJECT(${PROJECT_NAME})

SET(CMAKE_CXX_STANDARD 17)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

INCLUDE_DIRECTORIES(includes)

ADD_LIBRARY(${PROJECT_NAME} STATIC
    src/lexer.cpp
    includes/lexer.hpp
    includes/parser.hpp
    src/parser.cpp
    includes/ast.hpp
    src/ast.cpp)

ADD_SUBDIRECTORY(tests)
