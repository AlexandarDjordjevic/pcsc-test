cmake_minimum_required(VERSION 3.8.0)
if(POLICY CMP0092)
  cmake_policy(SET CMP0092 NEW)
endif()

project(pcsc-cpp)

set(target pcsc-test)

set(CMAKE_CXX_STANDARD 17)
# The libpcsc-cpp library itself.

add_executable(${target}
  src/main.cpp
)

if(MSVC)
  # no extra includes required
  target_link_libraries(${target} PRIVATE winscard)
elseif(APPLE)
  # no extra includes required
  target_link_libraries(${target} PRIVATE "-framework PCSC")
else()
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(PCSC libpcsclite REQUIRED)
  target_include_directories(${target} PRIVATE ${PCSC_INCLUDE_DIRS})
  target_link_libraries(${target} PRIVATE ${PCSC_LIBRARIES})
endif()

