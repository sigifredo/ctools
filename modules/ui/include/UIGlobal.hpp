
#ifndef UIGLOBAL_HPP
#define UIGLOBAL_HPP

// #ifdef CTOOLS_DEBUG
// #  include <iostream>
// #  define print(X)	std::cout << __LINE__ << ": " << X << std::endl
// #  define debug()	std::cout << __LINE__ << ": " << __FILE__ << std::endl
// #else
// #  define print(X)
// #  define debug()
// #endif

typedef unsigned int width_t;

#ifdef _WIN32
#  ifdef ct_ui_EXPORTS
#    define	UI_EXPORT	__declspec( dllexport )
#  else
#    define	UI_EXPORT	__declspec( dllimport )
#  endif
#else
#  define	UI_EXPORT
#endif

#endif
