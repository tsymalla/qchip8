CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(PROJECT_NAME compiler_lib)
PROJECT(${PROJECT_NAME})

SET(CMAKE_CXX_STANDARD 17)
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

INCLUDE_DIRECTORIES(includes)

ADD_LIBRARY(${PROJECT_NAME} STATIC
    src/lexer.cpp
    includes/lexer.hpp
    includes/parser.hpp
    includes/parser_error_state.hpp
    src/parser.cpp
    src/parser_error_state.cpp
    includes/ast.hpp
    includes/astvisitor.hpp
    src/astvisitor.cpp
    src/ast.cpp)