cmake_minimum_required(VERSION 3.16...3.21)
project(B3a)

# Geant4 setup
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add executable for Geant4 simulation
add_executable(exampleB3a exampleB3a.cpp ${sources} ${headers})
target_link_libraries(exampleB3a ${Geant4_LIBRARIES})

# MATLAB setup
set(Matlab_ROOT_DIR "/home/mohammed/asmaa/MATLAB")
set(Matlab_INCLUDE_DIRS "${Matlab_ROOT_DIR}/extern/include")
set(Matlab_MEX_LIBRARY "${Matlab_ROOT_DIR}/bin/glnxa64/libmex.so") 
set(Matlab_MX_LIBRARY "${Matlab_ROOT_DIR}/bin/glnxa64/libmx.so")
set(Matlab_MAT_LIBRARY "${Matlab_ROOT_DIR}/bin/glnxa64/libmat.so")

# Include MATLAB headers for MEX
include_directories(${Matlab_INCLUDE_DIRS})

# Add MEX target library
add_library(mex_geant4 SHARED mex_geant4.cpp ${sources})

# Link MEX target with MATLAB and Geant4 libraries
target_include_directories(mex_geant4 PRIVATE ${Matlab_INCLUDE_DIRS})
target_link_libraries(mex_geant4 ${Geant4_LIBRARIES} ${Matlab_MEX_LIBRARY} ${Matlab_MX_LIBRARY} ${Matlab_MAT_LIBRARY})

# Install the executables
install(TARGETS exampleB3a mex_geant4 DESTINATION bin)

