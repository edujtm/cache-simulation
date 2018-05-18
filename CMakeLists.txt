cmake_minimum_required(VERSION 3.8)
project(CacheSimulation)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp policies/LRUReplace.cpp policies/LRUReplace.h policies/WriteBackPolicy.h policies/WriteBackPolicy.cpp)
set(SOURCE_FILES ${SOURCE_FILES} policies/WritePolicy.h policies/ReplacementPolicy.h)
set(SOURCE_FILES ${SOURCE_FILES} caches/Cache.cpp caches/Cache.h helper/HitMissCounter.h)
add_executable(CacheSimulation ${SOURCE_FILES})
