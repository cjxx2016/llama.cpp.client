#ifndef __NL_INC_MACRO_H__
#define __NL_INC_MACRO_H__

/*!
	The version of C++ standard that is being used.
	The C++20 standard.
*/
#ifndef NL_HAS_CPP20
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201703L
#			define NL_HAS_CPP20	1
#		else /* _MSVC_LANG < 201703L */
#			define NL_HAS_CPP20	0
#		endif /* _MSVC_LANG > 201303L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201703L
#			define NL_HAS_CPP20	1
#		else /* __cplusplus > 201703L */
#			define NL_HAS_CPP20	0
#		endif /* __cplusplus > 201703L */
	#endif /* _MSVC_LANG */
#endif /* NL_HAS_CPP20 */

/*!
	The version of C++ standard that is being used.
	The C++17 standard.
*/
#ifndef NL_HAS_CPP17
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201402L
#			define NL_HAS_CPP17	1
#		else /* _MSVC_LANG > 201402L */
#			define NL_HAS_CPP17	0
#		endif /* _MSVC_LANG > 201402L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201402L
#			define NL_HAS_CPP17	1
#		else /* __cplusplus > 201402L */
#			define NL_HAS_CPP17	0
#		endif /* __cplusplus > 201402L */
	#endif /* _MSVC_LANG */
#endif /* NL_HAS_CPP17 */
 
/*!
	The version of C++ standard that is being used.
	The C++14 standard.
*/
#ifndef NL_HAS_CPP14
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 201103L
#			define NL_HAS_CPP14	1
#		else /* _MSVC_LANG > 201103L */
#			define NL_HAS_CPP14	0
#		endif /* _MSVC_LANG > 201103L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 201103L
#			define NL_HAS_CPP14	1
#		else /* __cplusplus > 201103L */
#			define NL_HAS_CPP14	0
#		endif /* __cplusplus > 201103L */
#	endif /* _MSVC_LANG */
#endif /* NL_HAS_CPP14 */
 
/*!
	The version of C++ standard that is being used.
	The C++11 standard.
*/
#ifndef NL_HAS_CPP11
#	ifdef _MSVC_LANG
#		if _MSVC_LANG > 199711L
#			define NL_HAS_CPP11	1
#		else /* _MSVC_LANG > 199711L */
#			define NL_HAS_CPP11	0
#		endif /* _MSVC_LANG > 199711L */
#	else /* _MSVC_LANG */
#		if __cplusplus > 199711L
#			define NL_HAS_CPP11	1
#		else /* __cplusplus > 199711L */
#			define NL_HAS_CPP11	0
#		endif /* __cplusplus > 199711L */
#	endif /* _MSVC_LANG */
#endif /* NL_HAS_CPP11 */

#endif // __NL_INC_MACRO_H__