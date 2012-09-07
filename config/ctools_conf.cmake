
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Debug / Release
if(CMAKE_BUILD_TYPE MATCHES Release)
    set( _WIN32_ WIN32 )
    add_definitions( -DNDEBUG )
else()
    set( _WIN32_ "" )
    add_definitions( -DCTOOLS_DEBUG )
endif()

macro( _add_executable _NAME_ _PATH_ )
    add_executable( ${_NAME_} ${_WIN32_} ${_PATH_} )
endmacro( _add_executable )
