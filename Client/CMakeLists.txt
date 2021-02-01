cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 14)

project(Client)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)


# Tell CMake to create the Client executable
add_executable(Client 
               main.cpp
	       mainwindow.cpp
               client_connection.cpp
               utils.cpp
               mainwindow.ui
)

# Use the Widgets module from Qt 5.
target_link_libraries(Client Qt5::Widgets Qt5::Core Qt5::Gui -pthread)