cmake_minimum_required(VERSION 3.28 FATAL_ERROR)

include(FetchContent)

#--- general
set(CMAKE_CXX_STANDARD 20)

#check if project is toplevel
if(${CMAKE_CURRENT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Dependencies/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif()

project(
  CAudio
  VERSION 1.0
  LANGUAGES CXX)
  
find_package(Vorbis CONFIG REQUIRED)

#dependencies directory is always in CMAKE_SOURCE_DIR
set(OPENAL_BUILD_DIR ${CMAKE_SOURCE_DIR}/Dependencies_build/openal-soft/Release)
set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependencies/openal-soft/include/)

include(${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles.cmake)
list(TRANSFORM INC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/include/)
list(TRANSFORM SRC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_library(${PROJECT_NAME} SHARED ${INC_FILES} ${SRC_FILES} )
target_link_libraries(${PROJECT_NAME} 
${OPENAL_BUILD_DIR}/OpenAL32.lib
Vorbis::vorbis
Vorbis::vorbisfile
)

target_include_directories(${PROJECT_NAME} PRIVATE
${CMAKE_CURRENT_SOURCE_DIR}/src
${CMAKE_CURRENT_SOURCE_DIR}/include
${OPENAL_INCLUDE_DIR}
)


set(${PROJECT_NAME}_INCLUDE_DIRS
${CMAKE_CURRENT_SOURCE_DIR}/include
)

#for other cmake projects to process/copy those files
set(${PROJECT_NAME}_LIBS
${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.lib
CACHE INTERNAL "${PROJECT_NAME}: LIBS" FORCE
)

set(${PROJECT_NAME}_DLLS
${CMAKE_CURRENT_BINARY_DIR}/ogg.dll
${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.dll
${CMAKE_CURRENT_BINARY_DIR}/vorbis.dll
${CMAKE_CURRENT_BINARY_DIR}/vorbisfile.dll
${OPENAL_BUILD_DIR}/OpenAL32.dll

CACHE INTERNAL "${PROJECT_NAME}: DLLs" FORCE
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
  target_link_options(${PROJECT_NAME} PRIVATE /PROFILE)
endif()

# Expose public includes to other subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)