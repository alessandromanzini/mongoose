# # "cmake-lists": swiftlog, "author": alessandromanzini
# swiftlog lib specific logic here.
#
project( swiftlog )


# ========================================
# SOURCE FILES
# ========================================

set(OBJECT_TARGET ${PROJECT_NAME}_object)
set(STATIC_TARGET ${PROJECT_NAME})
set(SHARED_TARGET ${PROJECT_NAME}_shared)

# create object lib for single compile static/shared linking
add_library( ${OBJECT_TARGET} OBJECT src/library.cpp )

# add actual library targets
add_library( ${STATIC_TARGET} STATIC )
add_library( ${SHARED_TARGET} SHARED )

# namespaced aliases for find_package / FetchContent consumers
add_library( ${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${STATIC_TARGET} )
add_library( ${PROJECT_NAME}::shared ALIAS ${SHARED_TARGET} )


# ========================================
# TARGET CONFIGURATION
# ========================================

# object target includes the directories
target_include_directories( ${OBJECT_TARGET}
                            PUBLIC
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>
                            PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src )

# static and shared targets link against object
target_link_libraries( ${STATIC_TARGET} PUBLIC ${OBJECT_TARGET} )
set_target_properties( ${STATIC_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )

target_link_libraries( ${SHARED_TARGET} PUBLIC ${OBJECT_TARGET} )
set_target_properties( ${SHARED_TARGET} PROPERTIES OUTPUT_NAME ${PROJECT_NAME} )

# add compile macros for dynamic symbols exports
string( TOUPPER ${PROJECT_NAME} UPPER_PROJECT_NAME )
target_compile_definitions( ${STATIC_TARGET} PUBLIC ${UPPER_PROJECT_NAME}_STATIC )
target_compile_definitions( ${SHARED_TARGET}
                            PUBLIC ${UPPER_PROJECT_NAME}_SHARED
                            PRIVATE ${UPPER_PROJECT_NAME}_BUILDING )


# set maximum warning level and treat warnings as errors
#include( set_w4wx_macro )
#set_w4wx()


# ========================================
# EXTERNAL DEPENDENCIES
# ========================================

# GLM (math library)
#include( fetch_glm_macro )
#fetch_glm()

# SDL2 (graphics/input)
#include( fetch_sdl2_macro )
#fetch_sdl2()
