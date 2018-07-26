cmake_minimum_required(VERSION 3.0)
project(netsysctrld)

# C++ version
set(CMAKE_CXX_STANDARD 11)

# What boost components this project needs
set(boost_components_used system thread)

# Find the required boost components
# Compilation will fail if the required libs can't be found
find_package(Boost REQUIRED COMPONENTS ${boost_components_used})
include_directories(${Boost_INCLUDE_DIRS})

# Header directories
include_directories(${PROJECT_SOURCE_DIR}/command)
include_directories(${PROJECT_SOURCE_DIR}/daemon/arg_parser)
include_directories(${PROJECT_SOURCE_DIR}/gpio)
include_directories(${PROJECT_SOURCE_DIR}/network)
include_directories(${PROJECT_SOURCE_DIR}/system)

# Executable and source files
add_executable(
        netsysctrld
        ${PROJECT_SOURCE_DIR}/daemon/netsysctrld.cpp
        ${PROJECT_SOURCE_DIR}/command/CommandParser.cpp
        ${PROJECT_SOURCE_DIR}/daemon/arg_parser/ArgParser.cpp
        ${PROJECT_SOURCE_DIR}/daemon/arg_parser/ArgParserFunction.cpp
        ${PROJECT_SOURCE_DIR}/gpio/GpioController.cpp
        ${PROJECT_SOURCE_DIR}/network/SocketWrapper.cpp
        ${PROJECT_SOURCE_DIR}/network/TcpServer.cpp
        ${PROJECT_SOURCE_DIR}/system/SystemUtils.cpp
)

# Link
target_link_libraries(netsysctrld ${Boost_LIBRARIES})