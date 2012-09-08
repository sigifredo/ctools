
#ifndef BASE_GLOBAL_HPP
#define BASE_GLOBAL_HPP

#ifdef _WIN32
#  ifdef ct_base_EXPORTS
#    define	BASE_EXPORT	__declspec( dllexport )
#  else
#    define	BASE_EXPORT	__declspec( dllimport )
#  endif
#else
#  define	BASE_EXPORT
#endif

#endif
