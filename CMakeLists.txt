project (FXServer_DBWizard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Project Files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

#Project Executable
add_executable(FXServer_DBWizard ${HEADERS} ${SOURCES})

#Project Includes
target_include_directories(FXServer_DBWizard PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
