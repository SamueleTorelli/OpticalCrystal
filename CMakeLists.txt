cmake_minimum_required(VERSION 3.12)

project(CrystalOptical
        DESCRIPTION "Geant4 simulation framework for Optical Crystal simulation"
        LANGUAGES C CXX)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MACOSX_RPATH ON)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Find dependencies
find_package(Geant4 REQUIRED ui_all vis_all)
find_package(GSL REQUIRED)
find_package(HDF5 REQUIRED)

option(With_Opticks OFF)

if(With_Opticks)
add_definitions(-DWith_Opticks)
message(STATUS "WITH_OPTICKS is set ON")
  option(With_G4OpticksTest OFF)  # This is used to compare simulated photon events between G4 and Opticks by producing them in the same event.
  if(With_G4OpticksTest)
     message(STATUS "Warning! With_G4OpticksTest Option is set ON!")
     message(STATUS "This option allows you to save both G4 and Opticks photon hits into the same file so hits can be compared between two")
  endif ()
  list(APPEND CMAKE_MODULE_PATH "$ENV{OPTICKS_HOME}/cmake/Modules")
  #list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake/Modules")
  set(OPTICKS_PREFIX "$ENV{OPTICKS_PREFIX}")
  message(STATUS "OPTICKS_PREFIX : $ENV{OPTICKS_PREFIX}")
  include(OpticksBuildOptions)
  message(STATUS "---- OPTICKS ----")
  message(STATUS "CMAKE_MODULE_PATH --> ${CMAKE_MODULE_PATH}")
  message(STATUS "OPTICKS HOME --> $ENV{OPTICKS_HOME}")
  message(STATUS "---- OPTICKS END ----")
  find_dependency(PLog)  # For some reason it was not finding PLog; doing this compiles
  find_package(G4CX REQUIRED CONFIG)
  find_package(U4 REQUIRED CONFIG)
endif ()

### Garfield Implementation
option(With_GarField OFF)
if(With_GarField)
  ## ROOT
  find_package(ROOT QUIET REQUIRED COMPONENTS RIO Net)
  if(NOT ROOT_FOUND)
    message(STATUS "ROOT package not found.")
    return()
  endif()

  message(STATUS "Garfield Mode On.")

  ## Garfield
  if(NOT TARGET Garfield::Garfield)
    find_package(Garfield REQUIRED)
  endif()

  if(NOT Garfield_FOUND)
    message(STATUS "Garfield package not found.")
    return()
  endif()

  message(STATUS "Garfield package found.")

  # NEST
  find_package(NEST REQUIRED)
  if(NOT NEST_FOUND)
    message(STATUS "NEST package not found.")
    return()
  endif()

  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

# Create temporary folder to contain headers in the build tree
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})

# Instead of multiple source folders, we now use:
#   - All header files from "include/" (with extension .hh)
#   - All source files from "src/" (with extension .cc)
file(GLOB HDRS ${CMAKE_SOURCE_DIR}/include/*.hh)
file(GLOB SRCS ${CMAKE_SOURCE_DIR}/src/*.cc)

# Copy header files into the build include directory
file(COPY ${HDRS} DESTINATION ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME})

# Build a shared library from the source files in "src/"
add_library(lib SHARED ${SRCS})
set_target_properties(lib PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# Add the directory containing the header files (so the compiler finds them)
target_include_directories(lib PRIVATE ${CMAKE_SOURCE_DIR}/include/CrystalOptical)

# Existing include directories (build/include and external dependencies)
target_include_directories(lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}>
    $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    ${Geant4_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS}
)

if(With_GarField)
  target_include_directories(lib PRIVATE ${NEST_INCLUDE_DIRS} ${ROOT_INCLUDE_DIRS})
  target_link_libraries(lib PUBLIC Garfield::Garfield ROOT::RIO ROOT::Net NEST::NESTCore NEST::NESTG4)
  message(STATUS "Using Garfield Version ${Garfield_VERSION}")
  option(GarfieldNewerVersion OFF)
  if(${Garfield_VERSION} VERSION_GREATER_EQUAL 0.3.0)
    set(GarfieldNewerVersion ON CACHE BOOL "GarfieldVersionCatch" FORCE)
  endif()
endif ()

if(With_Opticks)
  target_link_libraries(lib PUBLIC Opticks::G4CX Opticks::U4)
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
endif ()

target_include_directories(lib PRIVATE ${Geant4_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS} ${HDF5_INCLUDE_DIRS})
target_link_libraries(lib PUBLIC ${Geant4_LIBRARIES} PRIVATE ${GSL_LIBRARIES} ${HDF5_LIBRARIES})
configure_file(config.h.in ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME}/config.h)

# Build the main executable from CrystalOptical.cc (located at the top level)
add_executable(exe ${CMAKE_SOURCE_DIR}/CrystalOptical.cc)
set_target_properties(exe PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
target_link_libraries(exe PRIVATE lib)

# (Optional) Remove the Test executable if tests are not used.
#install only lib and exe targets.
install(TARGETS lib exe
        RUNTIME DESTINATION bin  
        LIBRARY DESTINATION lib)

install(DIRECTORY ${CMAKE_BINARY_DIR}/include/${PROJECT_NAME} DESTINATION include
        FILES_MATCHING PATTERN "*.hh")
