cmake_minimum_required(VERSION 3.0.0)
project(TIN-Project-Server CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}")
set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}")

include_directories("${CMAKE_SOURCE_DIR}")

add_subdirectory(core)
get_directory_property(
  SOURCES_core DIRECTORY core DEFINITION SOURCES
)

add_subdirectory(account_manager)
get_directory_property(
  SOURCES_account_manager DIRECTORY account_manager DEFINITION SOURCES
)

add_subdirectory(configurator)
get_directory_property(
  SOURCES_configurator DIRECTORY configurator DEFINITION SOURCES
)

add_subdirectory(instructions)
get_directory_property(
  SOURCES_instructions DIRECTORY instructions DEFINITION SOURCES
)

add_subdirectory(send_msgs)
get_directory_property(
  SOURCES_send_msgs DIRECTORY send_msgs DEFINITION SOURCES
)

add_subdirectory(chat)
get_directory_property(
  SOURCES_chat DIRECTORY chat DEFINITION SOURCES
)

add_subdirectory(image_objects)
get_directory_property(
  SOURCES_image_objects DIRECTORY image_objects DEFINITION SOURCES
)

add_executable(server
  main.cc
  "${SOURCES_core}"
  "${SOURCES_image}"
  "${SOURCES_account_manager}"
  "${SOURCES_configurator}"
  "${SOURCES_instructions}"
  "${SOURCES_send_msgs}"
  "${SOURCES_chat}"
  "${SOURCES_image_objects}"
)

target_link_libraries(server
  "crypto"
)
