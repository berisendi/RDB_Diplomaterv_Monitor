# Defining the name of the executable
set(EXECUTABLE_NAME "RDB_Didplomaterv_Monitor_${PROJECT_VERSION}")
message(STATUS "------------------------------------------------")
message(STATUS "Executable: ${EXECUTABLE_NAME}")

# Setting the source files
set(SOURCE_FILES main.cpp
                 data_point.cpp
                 data_line.cpp
                 diagram.cpp
		 data_processor.cpp
		 gui.cpp
		 serial_port.cpp
)

# Adding the executable
# TODO: This may need a WIN32 under windows later
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})

# Setting the include files
set(INCLUDE_FILES ${CMAKE_CURRENT_BINARY_DIR}/*.hpp)
message(STATUS "The include files: ${INCLUDE_FILES}")
target_include_directories(${EXECUTABLE_NAME} PUBLIC ${INCLUDE_FILES})

# Linking the executable
target_link_libraries(${EXECUTABLE_NAME} 
				Qt5::Widgets
				Qt5::Charts
				Boost::system)

