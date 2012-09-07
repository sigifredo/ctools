set(UI_DIR ${CMAKE_CURRENT_LIST_DIR})
include_directories(${UI_DIR}/include)

set( UI_HDRS
${UI_DIR}/include/UIGlobal.hpp
)

set( UI_MOC_HDRS
${UI_DIR}/include/UIMainWindow.hpp
)

set( UI_SRCS
${UI_DIR}/src/UIMainWindow.cpp
)

qt4_wrap_cpp( UI_MOC_SRCS ${UI_MOC_HDRS} )

add_library( ct_ui SHARED
${UI_HDRS}
${UI_MOC_SRCS}
${UI_SRCS}
)

target_link_libraries( ct_ui ${DEPS_LIBRARIES} )
