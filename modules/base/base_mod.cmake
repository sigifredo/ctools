set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR})
include_directories(${BASE_DIR}/include)

set( BASE_HDRS
${BASE_DIR}/include/BaseGlobal.hpp
)

set( BASE_MOC_HDRS
${BASE_DIR}/include/Process.hpp
${BASE_DIR}/include/Thread.hpp
)

set( BASE_SRCS
${BASE_DIR}/src/Process.cpp
${BASE_DIR}/src/Thread.cpp
)

qt4_wrap_cpp( BASE_MOC_SRCS ${BASE_MOC_HDRS} )

add_library( ct_base SHARED
${BASE_HDRS}
${BASE_MOC_SRCS}
${BASE_SRCS}
)

add_executable( exe_c ${BASE_DIR}/src/exe_c.cpp )
target_link_libraries( ct_base ${DEPS_LIBRARIES} )
target_link_libraries( exe_c ct_base )
