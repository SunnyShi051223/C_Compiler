cmake_minimum_required(VERSION 3.28)
project(ShiXvn_Compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(ShiXvn_Compiler
        main.cpp
        src/Lexer.cpp
        src/ErrorHandler.cpp
        src/SymbolTable.cpp
        src/Grammar.cpp
        src/PrecedenceParser.cpp
        src/SemanticAnalyzer.cpp
        src/AST.cpp
        src/CodegenVisitor.cpp
)

target_include_directories(ShiXvn_Compiler PRIVATE include)
