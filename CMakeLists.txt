cmake_minimum_required(VERSION 3.13)
project(project)

set(CMAKE_CXX_STANDARD 14)
list(APPEND CMAKE_CXX_FLAGS "-pthread -std=c++11 ${CMAKE_CXX_FLAGS}")

add_executable(project main.cpp Command.h OpenServerCommand.cpp OpenServerCommand.h ConnectCommand.cpp ConnectCommand.h
        DefineVarCommand.cpp DefineVarCommand.h Interpreter.cpp Interpreter.h BinaryExpression.h Divi.cpp Divi.h
        Expression.h Minus.h Minus.cpp Mul.cpp Mul.h Number.h Number.cpp Plus.cpp Plus.h LoopCommand.cpp LoopCommand.h
        ConnectToServer.cpp ConnectToServer.h IfCommand.h IfCommand.cpp
        EqualsCommand.cpp EqualsCommand.h PrintCommand.cpp PrintCommand.h DataReaderServer.h DataReaderServer.cpp SleepCommand.cpp SleepCommand.h
        ConnectToServer.cpp ConnectToServer.h exitCommand.cpp exitCommand.h)