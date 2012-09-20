set(UI_DIR ${CMAKE_CURRENT_LIST_DIR})
include_directories(${UI_DIR}/include)

set( UI_HDRS
${UI_DIR}/include/UIGlobal.hpp
${UI_DIR}/include/UITypes.hpp
)

set( UI_MOC_HDRS
${UI_DIR}/include/UIMainWindow.hpp
${UI_DIR}/include/UITerminalWidget.hpp
)

set( UI_SRCS
${UI_DIR}/src/UIMainWindow.cpp
${UI_DIR}/src/UITerminalWidget.cpp
)

qt4_wrap_cpp( UI_MOC_SRCS ${UI_MOC_HDRS} )

add_library( ct_ui SHARED
${UI_HDRS}
${UI_MOC_SRCS}
${UI_SRCS}
)

_add_executable( cterminal ${UI_DIR}/src/term.cpp )
_add_executable( mWindow ${UI_DIR}/src/mWindow.cpp )

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries( ct_ui ct_base ${DEPS_LIBRARIES} )
else()
    target_link_libraries( ct_ui ${DEPS_LIBRARIES} )
endif()

target_link_libraries( cterminal ct_ui ${DEPS_LIBRARIES} )
target_link_libraries( mWindow ct_ui ${DEPS_LIBRARIES} )
