cmake_minimum_required(VERSION 2.8)
project(jniw)

################################
# Compiler setup
################################
set(JNIW_VERSION "0.0.1")

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    find_package(JNI)
    include_directories(${JNI_INCLUDE_DIRS})
elseif ("${CROSS_TARGET}" STREQUAL "android")
    find_package(Android)
    include_directories(${ANDROID_INCLUDE_DIRS})
endif()


################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.c)

include_directories(include)

################################
# Build targets
################################
add_library(jniw
    ${SRC}
    ${INCLUDES})

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    target_link_libraries(jniw
        ${JNI_LIBRARIES})
elseif ("${CROSS_TARGET}" STREQUAL "android")
    target_link_libraries(jniw
        ${ANDROID_LIBRARIES})
endif()

################################
# Export library
################################
export_project(
    NAME jniw
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    LIBS jniw
    VERSION ${JNIW_VERSION}
)

