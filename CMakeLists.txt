# Make Setting
message("Root: --- ----- ----- ----- ----- --")

add_compile_definitions(__PLATFORM_NAME="${PLATFORM_NAME}")
add_compile_definitions(__PLATFORM_VERSION="${PLATFORM_VERSION}")
add_compile_definitions(__PLATFORM_EXTENSION="${PLATFORM_EXTENSION}")

# All Configs
cmake_minimum_required (VERSION 3.14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ENABLE_TEST ON)
# Enable C++17
set(CMAKE_CXX_STANDARD 17)
# Enable C11
set(C_STANDARD 11)
# Enable UNICODE
add_compile_definitions(UNICODE)
message("Root: PROJECT_BINARY_DIR = ${PROJECT_BINARY_DIR}")


# MSVC Policy
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()
# Set CMP0076 Policy NEW
cmake_policy(SET CMP0076 NEW)


# Message Platfrom
message("Root: ${PLATFORM_NAME}-${PLATFORM_VERSION}-${PLATFORM_EXTENSION}")

# Threads Enable
add_compile_definitions(_PTHREADS)

# Project
project("Symbolization")

# 设置构建共享库
set(BUILD_SHARED_LIBS ON)

# 确保在Windows上为DLL生成导入库
if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

include_directories(./include)

find_package(Convention::Shared REQUIRED)

aux_source_directory(./src SRC_FILES)
add_executable(Symbolization ${SRC_FILES})

install(TARGETS Symbolization RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

message("Root: CMAKE_CXX_STANDARD = ${CMAKE_CXX_STANDARD}" )
message("Root: CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
message("Root: CMAKE_CXX_COMPILER_VERSION = ${CMAKE_CXX_COMPILER_VERSION}")



message("Root: ----- ----- ----- ----- -----")