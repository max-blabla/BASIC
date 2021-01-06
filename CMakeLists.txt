cmake_minimum_required(VERSION 3.14)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code Basic/Basic.cpp Basic/evalstate.cpp Basic/exp.cpp Basic/parser.cpp Basic/program.cpp Basic/statement.cpp
        StanfordCPPLib/tokenscanner.cpp StanfordCPPLib/error.cpp StanfordCPPLib/simpio.cpp StanfordCPPLib/strlib.cpp)