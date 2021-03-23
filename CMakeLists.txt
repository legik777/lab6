cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)
option(BUILD_COVERAGE "Build code coverage" OFF)

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

#hunter_add_package(GTest)
#find_package(GTest CONFIG REQUIRED)

#hunter_add_package(Boost COMPONENTS system filesystem)
hunter_add_package(Boost COMPONENTS log thread system filesystem program_options)
find_package(Boost COMPONENTS log log_setup thread system filesystem program_options)
find_package(Threads)
hunter_add_package(nlohmann_json)
find_package(nlohmann_json 3.1.4 CONFIG REQUIRED)


#add_executable(tests
#  # TODO: enum your files and delete this comment
#  ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
#)

# TODO: change template word to project name and delete this comment

target_include_directories(tests
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)
add_executable( ${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/sources/CPP.cpp
)
target_include_directories(hasher PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${Boost_INCLUDE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third-party/PicoSHA2>"
        "$<INSTALL_INTERFACE:include>"
        )
target_link_libraries(hasher nlohmann_json::nlohmann_json
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries( ${PROJECT_NAME} hasher )
		
		
# TODO: change template word to project name and delete this comment
#target_link_libraries(tests GTest::gtest_main)

#enable_testing()
#add_test(NAME unit_tests COMMAND tests)

#file(COPY misc DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if(BUILD_COVERAGE)
  function(ENABLE_PROJECT_COVERAGE PROJECT)
    target_compile_options(${PROJECT} PRIVATE --coverage)
    target_link_libraries(${PROJECT} --coverage)
    add_coverage(${PROJECT})
  endfunction()

  set(ENABLE_COVERAGE ON CACHE BOOL "Enable coverage build." FORCE)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
  find_package(codecov)
  # TODO: change template word to project name and delete this comment
  enable_project_coverage(tests)
  list(APPEND LCOV_REMOVE_PATTERNS "'${PROJECT_SOURCE_DIR}/tests/*'")
  coverage_evaluate()
endif()
