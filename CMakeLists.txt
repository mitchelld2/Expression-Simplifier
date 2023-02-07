cmake_minimum_required(VERSION 3.10)
project(ExpressionSimplifier)

set(CMAKE_CXX_STANDARD 14)

add_executable(Simplifier main.cpp ExpressionTree.cpp TreeNode.cpp)
