project( ctools )
cmake_minimum_required( VERSION 2.8 )

set(DIVS "conf" "dep" "mod")

foreach(it ${DIVS})
    message("-> Buscando archivos ${it}")
    file(GLOB_RECURSE MODULOS "*_${it}.cmake")
    foreach(jt ${MODULOS})
        message(STATUS "Módulo: ${jt} encontrado")
        include(${jt})
    endforeach(jt)
endforeach(it)

include(CPack)

