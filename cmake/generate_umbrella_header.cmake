function( generate_umbrella_header UMBRELLA_HEADER_NAME INCLUDE_DIRECTORY OUTPUT_DIRECTORY )

    # ========================================
    # ARGUMENT VALIDATION
    # ========================================

    get_filename_component( EXTENSION ${UMBRELLA_HEADER_NAME} EXT )
    if( NOT ${EXTENSION} STREQUAL "" )
        message( FATAL_ERROR "UMBRELLA_HEADER_NAME must not contain an extension (${UMBRELLA_HEADER_NAME})!" )
    endif()

    if( NOT IS_ABSOLUTE ${INCLUDE_DIRECTORY} )
        message( FATAL_ERROR "INCLUDE_DIRECTORY must be an absolute path (${INCLUDE_DIRECTORY})!" )
    endif()


    # ========================================
    # HEADER GENERATION
    # ========================================

    string( TOUPPER ${UMBRELLA_HEADER_NAME} HEADER_GUARD )
    string( APPEND HEADER_GUARD _HPP )
    set( UMBRELLA_HEADER_FILE ${UMBRELLA_HEADER_NAME}.hpp )

    # include guards
    set( CONTENT "// UMBRELLA HEADER: auto-generated in cmake from globbed headers...\n\n" )
    string( APPEND CONTENT "#ifndef ${HEADER_GUARD}\n" )
    string( APPEND CONTENT "#define ${HEADER_GUARD}\n\n\n" )

    # glob, exclude umbrella, sort
    file( GLOB_RECURSE INCLUDE_FILE_PATHS "${INCLUDE_DIRECTORY}/*.hpp" )
    list( FILTER INCLUDE_FILE_PATHS EXCLUDE REGEX ".*${UMBRELLA_HEADER_FILE}$" )
    list( SORT INCLUDE_FILE_PATHS COMPARE NATURAL ORDER ASCENDING )

    # extract pch and move to front
    set( PCH_PATHS ${INCLUDE_FILE_PATHS} )
    list( FILTER PCH_PATHS INCLUDE REGEX ".*/pch\\.hpp$" )
    list( FILTER INCLUDE_FILE_PATHS EXCLUDE REGEX ".*/pch\\.hpp$" )
    list( PREPEND INCLUDE_FILE_PATHS ${PCH_PATHS} )

    # append headers
    foreach( INCLUDE_FILE_PATH ${INCLUDE_FILE_PATHS} )
        file( RELATIVE_PATH REL_PATH ${INCLUDE_DIRECTORY} ${INCLUDE_FILE_PATH} )
        string( APPEND CONTENT "#include \"${REL_PATH}\"\n" )
        if( REL_PATH MATCHES ".*/pch\\.hpp$" )
            string( APPEND CONTENT "\n" )
        endif()
    endforeach()

    string( APPEND CONTENT "\n\n#endif //!${HEADER_GUARD}\n" )

    cmake_path( APPEND OUTPUT_DIRECTORY ${UMBRELLA_HEADER_FILE} OUTPUT_VARIABLE OUTPUT_FILE_PATH )
    file( WRITE ${OUTPUT_FILE_PATH} ${CONTENT} )

endfunction()