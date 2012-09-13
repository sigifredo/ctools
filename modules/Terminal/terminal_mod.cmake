set(TERMINAL_DIR ${CMAKE_CURRENT_LIST_DIR} )

include_directories(${TERMINAL_DIR}/include)

set( TERMINAL_HDRS
${TERMINAL_DIR}/include/Types.hpp
)

set( TERMINAL_MOC_HDRS
${TERMINAL_DIR}/include/Terminal.hpp
)

set( TERMINAL_SRCS
${TERMINAL_DIR}/src/Terminal.cpp
)

qt4_wrap_cpp( TERMINAL_MOC_SRC
${TERMINAL_MOC_HDRS}
)

add_library(XTerminal STATIC
${TERMINAL_HDRS}
${TERMINAL_MOC_SRC}
${TERMINAL_SRCS}
)

_add_executable( terminal ${TERMINAL_DIR}/src/terminal.cpp )

target_link_libraries( XTerminal ${QT_LIBRARIES} )
target_link_libraries( terminal XTerminal ${QT_LIBRARIES} )
#target_link_libraries( XTerminalStatic ${QT_LIBRARIES} )
