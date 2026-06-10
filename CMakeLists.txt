cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 17)

project(SpartysBoots)

# Set the application library
set(APPLICATION_LIBRARY SpartysBootsLib)

# Request the required wxWidgets libs
# Turn off wxWidgets own precompiled header system
set(wxBUILD_PRECOMP OFF)
find_package(wxWidgets COMPONENTS core base xrc html xml REQUIRED)

# Include the wxWidgets use file to initialize various settings
include(${wxWidgets_USE_FILE})

# Add the application library directory
add_subdirectory(${APPLICATION_LIBRARY})
include_directories(${APPLICATION_LIBRARY})

# Define a variable containing a list of source files for the project
set(SOURCE_FILES main.cpp SpartysBootsApp.cpp SpartysBootsApp.h pch.h SpartysBoots.rc SpartysBoots.ico)

# Define the build target for the executable
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCE_FILES})

# Link required libraries to the executable
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${APPLICATION_LIBRARY})

# Use precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE pch.h)

# Copy images and levels into the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/images/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/levels/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/levels/)

# Handle Mac-specific settings
if(APPLE)
    set(RESOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/SpartysBoots.app/Contents/Resources)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images/ DESTINATION ${RESOURCE_DIR}/images/)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/levels/ DESTINATION ${RESOURCE_DIR}/levels/)
endif()

# Add test directory
add_subdirectory(Tests)
