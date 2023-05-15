cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

project("Geometry Processing")

# Maybe stop from CMAKEing in the wrong place
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Source and build directories cannot be the same. Go use the /build directory.")
endif()

### Configure output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### Compiler options
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 ) # Emit a compile flags file to support completion engines 

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using Clang (linux or apple) or GCC
  message("Using clang/gcc compiler flags")
  SET(BASE_CXX_FLAGS "-std=c++11 -Wall -Wextra -g3")
  SET(DISABLED_WARNINGS " -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-deprecated-declarations -Wno-missing-braces")
  SET(TRACE_INCLUDES " -H -Wno-error=unused-command-line-argument")

  if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message("Setting clang-specific options")
    SET(BASE_CXX_FLAGS "${BASE_CXX_FLAGS} -ferror-limit=5 -fcolor-diagnostics")
    SET(CMAKE_CXX_FLAGS_DEBUG          "-fsanitize=address -fno-limit-debug-info")
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(BASE_CXX_FLAGS "${BASE_CXX_FLAGS} -fmax-errors=5")
    message("Setting gcc-specific options")
    SET(DISABLED_WARNINGS "${DISABLED_WARNINGS} -Wno-maybe-uninitialized -Wno-format-zero-length -Wno-unused-but-set-parameter -Wno-unused-but-set-variable")
  endif()

  SET(CMAKE_CXX_FLAGS "${BASE_CXX_FLAGS} ${DISABLED_WARNINGS}")
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TRACE_INCLUDES}") # uncomment if you need to track down where something is getting included from
  SET(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -g3")
  SET(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_RELEASE        "-march=native -O3 -DNDEBUG")
  SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif()

# Find Eigen
# find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Add polyscope
message("\n\n == CMAKE recursively building Polyscope\n")
add_subdirectory("deps/polyscope")
add_subdirectory("deps/Eigen")

# Add libIGL
# With these lines commented out, we use libIGL in "header only" mode.  Uncomment to recurse and get the full package.
# (and see the libIGL build documentation)
#message("\n\n == CMAKE recursively building libIGL\n")
#set(LIBIGL_WITH_EMBREE            OFF CACHE BOOL "Use Embree")
#set(LIBIGL_WITH_OPENGL            OFF CACHE BOOL "Use OpenGL")
#set(LIBIGL_WITH_OPENGL            OFF CACHE BOOL "Use OpenGL")
#set(LIBIGL_WITH_OPENGL_GLFW       OFF CACHE BOOL "Use GLFW"  )
#set(LIBIGL_WITH_OPENGL_GLFW_IMGUI OFF CACHE BOOL "Use ImGui" )
#add_subdirectory("deps/libIGL")

# Create an executable

# Include settings
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/libigl/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*/*.cpp )


add_executable(psr ${SOURCE_FILES} "exec/psr.cpp")
target_link_libraries(psr polyscope Eigen3::Eigen)

add_executable(smooth ${SOURCE_FILES} "exec/smooth.cpp")
target_link_libraries(smooth polyscope Eigen3::Eigen)

add_executable(viewmesh ${SOURCE_FILES} "exec/viewmesh.cpp")
target_link_libraries(viewmesh polyscope Eigen3::Eigen)

add_executable(tutte ${SOURCE_FILES} "exec/tutte.cpp")
target_link_libraries(tutte polyscope Eigen3::Eigen)