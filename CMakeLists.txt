cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#option(BUILD_TESTS "Build tests" ON)
#option(BUILD_DOCS "Build documentation" OFF)
#option(BUILD_COVERAGE "Build code coverage" OFF)

set(
  HUNTER_CACHE_SERVERS
  "https://github.com/bmstu-iu8-cpp-sem-3/hunter-cache"
  CACHE STRING "Default cache server"
)

include("cmake/HunterGate.cmake")

HunterGate(
  URL "https://github.com/cpp-pm/hunter/archive/v0.23.278.tar.gz"
  SHA1 "35fa55bc1dfcf9f4d7264f8bb26cd1759ce1ae07"
)

#Имя проекта
project(lab6 VERSION 0.1.0)
string(APPEND CMAKE_CXX_FLAGS " -pedantic -Werror -Wall -Wextra")
string(APPEND CMAKE_CXX_FLAGS " -Wno-unused-command-line-argument")
string(APPEND CMAKE_CXX_FLAGS " -Wshadow -Wnon-virtual-dtor")
# ищем в хантере библиотеки
hunter_add_package(Boost COMPONENTS log)
find_package(Boost 1.72.0 CONFIG REQUIRED log)

hunter_add_package(nlohmann_json)
find_package(nlohmann_json 3.1.4 CONFIG REQUIRED)

#основной файл с кодом
add_executable(CPP
  ${CMAKE_CURRENT_SOURCE_DIR}/sourses/CPP.cpp
)

#добавляем всё, что находиться в папке include
target_include_directories(${PROJECT_NAME} PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:include>"
)


target_link_libraries(${PROJECT_NAME} Boost::log)




include(CPackConfig.cmake)

install(TARGETS ${PROJECT_NAME} demo
  EXPORT "${PROJECT_NAME}-targets"
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  INCLUDES DESTINATION include
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)

install(EXPORT "${PROJECT_NAME}-targets"
  NAMESPACE "${PROJECT_NAME}::"
  DESTINATION "lib/cmake"
)
