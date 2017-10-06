#ifndef FSW__WINDOWS__REALPATH_H
#	define FSW__WINDOWS__REALPATH_H

#	ifdef __cplusplus
extern "C"
{
#	endif

_CRTIMP char __cdecl
*realpath( const char *__restrict__ name, char *__restrict__ resolved );

#	ifdef __cplusplus
}
#	endif

#endif	/* FSW__WINDOWS__REALPATH_H */