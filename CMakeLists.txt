cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("tools/gate/cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
  SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)


#имя проекта
project(lab6)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")



hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost 1.72.0 CONFIG REQUIRED system filesystem)

hunter_add_package(Boost COMPONENTS log)
find_package(Boost 1.72.0 CONFIG REQUIRED log)

hunter_add_package(nlohmann_json)
find_package(nlohmann_json 3.1.4 CONFIG REQUIRED)


add_executable(CPP
 ${CMAKE_CURRENT_SOURCE_DIR}/sources/CPP.cpp
)

target_include_directories(CPP
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} Boost::log)

