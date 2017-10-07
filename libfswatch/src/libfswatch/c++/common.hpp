#ifndef FSW_COMMON_H
#	define FSW_COMMON_H

#if defined (HAVE_WINDOWS) && !defined(HAVE_CYGWIN)
  #ifdef LIBFSWATCH_COMMON_EXPORTS
    #define LIBFSWATCH_API __declspec(dllexport)
  #else
    #define LIBFSWATCH_API __declspec(dllimport)
  #endif
#else
	#define LIBFSWATCH_API
#endif

#endif	// FSW_COMMON_H