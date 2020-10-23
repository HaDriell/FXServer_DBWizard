project (FXServer_DBWizard)

#Project Files
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

#Project Executable
add_executable(FXServer_DBWizard ${HEADERS} ${SOURCES})

#Project Configuration
target_compile_features(FXServer_DBWizard PRIVATE cxx_std_17)

#Project Includes
target_include_directories(FXServer_DBWizard PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
