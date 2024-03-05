cmake_minimum_required(VERSION 3.25)
project(CppProject)

set(CMAKE_CXX_STANDARD 17)

# Включение флагов отладки
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")

# Включение флагов для проверки памяти (используется Valgrind)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -Wno-unused-but-set-variable")

add_executable(CppProject main.cpp Tests.h Tests.cpp CircularBuffer.h)
