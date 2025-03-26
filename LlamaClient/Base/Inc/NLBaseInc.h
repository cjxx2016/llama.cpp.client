#pragma once
#ifndef __CJ_TOOLS_BASE_INC_H__
#define __CJ_TOOLS_BASE_INC_H__
/*
* NL库
	@cjxx 2011.08.01 起
	@cjxx 2013.04.25

	说明: 
		内置了几种简明的数据结构为编写提速
		
		对应关系如下:

		fcn		-> std::function
		str		-> std::string
		wstr	-> std::wstring
		arr		-> std::vector
		srr		-> std::array
		deq		-> std::deque
		set 	-> std::set(不推荐)
		uni		-> std::unordered_set
		map		-> std::map(不推荐)
		dic		-> std::unordered_map

*		提供系列超高性能数据容器:
		
		NLBuf 
		NLChan 
		NLSign
		NLSignChan
		NLMultiTask
		NLStepThread
		NLTaskQue
		... ...

		主体功能使用 C++14 标准.
		部分功能针对 C++17 做了性能优化.
		少数功能用到 C++20 的特性.

*		一般情况下, C++ 有着良好的向下兼容特性, 可尽量选择高版本的 C++ 编译器进行编译.
*/

//	GAME_VERSION
#define GAME_VERSIION				1001

// Definition of platforms
#define NL_TARGET_WIN32				1
#define NL_TARGET_LINUX				2
#define NL_TARGET_APPLE				3
#define NL_TARGET_FLASH				4
#define NL_TARGET_ANDROID			5

// Definition of compilers
#define BASE_COMPILER_MSVC			1
#define BASE_COMPILER_GNUC			2

// Find platform
#if defined _FLASH
#	define NL_TARGET_PLATFORM		NL_TARGET_FLASH
#elif defined (__WIN32__) || defined (_WIN32)
#	define NL_TARGET_PLATFORM		NL_TARGET_WIN32
#elif defined (__APPLE_CC__)
#	define NL_TARGET_PLATFORM		NL_TARGET_APPLE
#elif defined (__ANDROID__)
#	define NL_TARGET_PLATFORM		NL_TARGET_ANDROID
#else
#	define NL_TARGET_PLATFORM		NL_TARGET_LINUX
#endif

#include "NLMacro.h"

typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef long				LONG;
typedef unsigned long       DWORD;

#if NL_TARGET_PLATFORM == NL_TARGET_WIN32

#pragma warning(disable:4788)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#pragma warning(disable:4244)
#pragma warning(disable:4800)
#pragma warning(disable:4996)

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
// 从 Windows 头文件中排除极少使用的内容
#define WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#define NOMINMAX
#endif
// windows 不能定义在 winsock2.h 前面. 否则会报错
#include <Windows.h>
#include <MMSystem.h>
#include <float.h>
#include <direct.h>
#include <sys/timeb.h>
#include <signal.h>

typedef signed	 __int64			int64_t;
typedef unsigned __int64			uint64_t;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 1
#endif

#endif

#if NL_TARGET_PLATFORM == NL_TARGET_APPLE

#include <inttypes.h>

#endif

// Linux/Apple Settings
#if NL_TARGET_PLATFORM == NL_TARGET_LINUX || NL_TARGET_PLATFORM == NL_TARGET_APPLE || NL_TARGET_PLATFORM == NL_TARGET_FLASH || NL_TARGET_PLATFORM == NL_TARGET_ANDROID

#if defined(__clang__)
# pragma clang diagnostic ignored "-Wsign-compare"
# pragma clang diagnostic ignored "-Wchar-subscripts"
# pragma clang diagnostic ignored "-Wswitch"
# pragma clang diagnostic ignored "-Winvalid-constexpr"
# pragma clang diagnostic ignored "-Wpessimizing-move"
#elif defined(__GNUC__)
# pragma GCC diagnostic ignored "-Wsign-compare"
# pragma GCC diagnostic ignored "-Wchar-subscripts"
# pragma GCC diagnostic ignored "-Wswitch"
# pragma GCC diagnostic ignored "-Winvalid-constexpr"
# pragma GCC diagnostic ignored "-Wpessimizing-move"
#endif

#include <sys/time.h>
#include <cstdint>
#include <string.h>
#include <dlfcn.h>

typedef signed char					INT8,	*PINT8;
typedef signed short				INT16,	*PINT16;
typedef signed int					INT32,	*PINT32;
typedef unsigned char				UINT8,	*PUINT8;
typedef unsigned short				UINT16, *PUINT16;
typedef unsigned int				UINT32, *PUINT32;

typedef int							INT;
typedef unsigned long				UINT;
typedef uint64_t					UINT64;
typedef int64_t						INT64;
typedef int64_t						__int64;
typedef long                        LONG;
typedef unsigned long				ULONG;

#undef FALSE
#undef TRUE
#undef NULL
#undef BOOL
#undef CONST
#undef _snprintf
#undef _atoi64
#undef HWND

#define FALSE						0
#define TRUE						1
#define NULL						0
// typedef int							BOOL;
#define CONST						const
#define _snprintf					snprintf
#define _atoi64						atoll
#define HWND						void*

#endif

// 标准C
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstdarg>
#include <cmath>

//	STL 容器
#include <string>
#include <vector>
#include <array>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <bitset>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

// STL算法和函数
#include <algorithm>
#include <functional>
#include <limits>
#include <numeric>
#include <random>

// C++流操作
#include <fstream>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <format>

#include <utility>
#include <assert.h>
#include <memory>

#include <sys/types.h>
#include <sys/stat.h>

#include <math.h>
#include <time.h>

// 多线程
#include <atomic>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <thread>
#include <future>

// C++17
#if NL_HAS_CPP17
#include <optional>
#endif

#ifndef DEF_BASE_TYPE
#define DEF_BASE_TYPE		1

#define	int8				/*signed */char
#define	int16				/*signed */short
#define int32				/*signed */int
#ifndef int64
#define int64				int64_t
#endif

#define uint8				unsigned char
#define uint16				unsigned short

typedef unsigned int		uint32;

#ifndef uint64
#define uint64				uint64_t
#endif

#ifndef ulong
#define ulong				unsigned long
#endif

// 定义容器数据结构
typedef double				real;
typedef std::string			str;
typedef std::wstring		wstr;

// C++17 
#if NL_HAS_CPP17
typedef std::string_view	str_v;
typedef std::wstring_view	wstr_v;
#endif

// using 定义全局的 数据类型缩写 - 这些类型缩写.作为系统关键字统一使用
template <class _Fty> 									 		using fcn = std::function<_Fty>;

template <class _Ty, class _Alloc = std::allocator<_Ty>> 		using arr = std::vector<_Ty, _Alloc>;

template <class _Ty, size_t _Size>						 		using srr = std::array<_Ty, _Size>;

//* 大部分情况 lst 应该考虑使用 deq - 包括添加/遍历和删除
//template <class _Ty, class _Alloc = std::allocator<_Ty>> 		using lst = std::list<_Ty, _Alloc>;	

template <class _Ty, class _Alloc = std::allocator<_Ty>> 		using deq = std::deque<_Ty, _Alloc>;

//* 大部分情况 set 应该考虑使用 uni - 除非需要有序遍历
template <class _Kty, class _Pr = std::less<_Kty>, 
	class _Alloc = std::allocator<_Kty>> 				 		using set = std::set<_Kty, _Pr, _Alloc>;

//* 大部分情况 map 应该考虑使用 dic - 除非需要有序遍历
template <class _Kty, class _Ty, class _Pr = std::less<_Kty>, 
	class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>> 	using map = std::map<_Kty, _Ty, _Pr, _Alloc>;

template <class _Kty, class _Hasher = std::hash<_Kty>, 
	class _Keyeq = std::equal_to<_Kty>, 
	class _Alloc = std::allocator<_Kty>>						using uni = std::unordered_set<_Kty, _Hasher, _Keyeq, _Alloc>;

template <class _Kty, class _Ty, 
	class _Hasher = std::hash<_Kty>, 
	class _Keyeq = std::equal_to<_Kty>,
    class _Alloc = std::allocator<std::pair<const _Kty, _Ty>>>	using dic = std::unordered_map<_Kty, _Ty, _Hasher, _Keyeq, _Alloc>;	


#endif	//	DEF_BASE_TYPE

#ifndef _FMT_I64
#	ifndef	WIN32
#		define _FMT_I64		"%ld"
#		define _WFMT_I64	L"%ld"
#	else
#		define _FMT_I64		"%I64d"
#		define _WFMT_I64	L"%I64d"
#	endif
#endif

#ifdef WIN32
#	ifndef INT64_MAX
#		define INT64_MAX	9223372036854775807i64
#	endif
#	ifndef UINT64_MAX
#		define UINT64_MAX	0xffffffffffffffffui64
#	endif
#else
#	ifndef INT64_MAX
#		define INT64_MAX	9223372036854775807LL
#	endif
#	ifndef UINT64_MAX
#		define UINT64_MAX	0xffffffffffffffffLL
#	endif
#endif

#ifndef MAKE_ID_64
#define MAKE_ID_64(l, h)	((uint64)(((uint32)((uint64)(l) & 0xffffffff)) | ((uint64)((uint32)((int64)(h) & 0xffffffff))) << 32))
#endif

#ifndef LO_ID_32
#define LO_ID_32(l)			((uint32)((uint64)(l) & 0xffffffff))
#endif

#ifndef HI_ID_32
#define HI_ID_32(h)			((uint32)((((uint64)(h)) >> 32) & 0xffffffff))
#endif

// MyDbgNew.h
// Defines global operator new to allocate from
// client blocks
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
 

// 这里如果开启 - 会导致某些函数名字冲突 - 因此关闭, 虽然 win 上面可以进行内存泄漏方面的排查. 但是这里只能先忽略了.
// 后面有需要再开启
// #ifndef MYDEBUG_NEW
// #	ifdef WIN32
// #		ifdef _DEBUG
// #			define MYDEBUG_NEW		new( _NORMAL_BLOCK, __FILE__, __LINE__)
// #			define new_dbg			MYDEBUG_NEW
// #			define malloc(s)		_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #			define calloc(c, s)		_calloc_dbg(c, s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #			define realloc(p, s)	_realloc_dbg(p, s, _NORMAL_BLOCK, __FILE__, __LINE__)
// #			define free(p)			_free_dbg(p, _NORMAL_BLOCK)
// #		else
// #			define MYDEBUG_NEW
// #			define new_dbg			new
// #		endif
// #	else
// #		define new_dbg				new
// #	endif
// #endif

//* if_const 宏 - 若 x 为常量, 则编译器可能优化代码,实际运行时会去掉 if判断. 从而提高运行效率
//* 此功能需要 c++17 及以上版本才能支持.
#if NL_HAS_CPP17
#	define if_const(x)		if constexpr (x)
#	define NLConst 			constexpr
#else 
#	define if_const(x)		if (x)
#	define NLConst
#endif

// enum class 获取-变量值
#define em_v(v) (std::underlying_type_t<decltype(v)>)(v)

// 对字符串计算哈希值
constexpr size_t prime = 0x100000001B3ull;
constexpr size_t basis = 0xCBF29CE484222325ull;

constexpr size_t _hash(const char* p) {
    size_t ret{basis};
   
    while(*p) {  
        ret ^= *p;
        ret *= prime;
        p++;
    }
    return ret;  
}

constexpr size_t _hash(const str& p) {
	return _hash(p.c_str());
}

constexpr size_t operator "" _hash(const char* p, size_t) {
    return _hash(p);
}

// pre-calculation of CRC table
constexpr std::array<uint32_t, 256> __generate_crc_table() {
    std::array<uint32_t, 256> crc_table = {};
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (size_t j = 0; j < 8; j++) {
            if (c & 1) {
				c = 0xEDB88320 ^ (c >> 1);
			} else {
				c = c >> 1;
			}
        }
        crc_table[i] = c;
    }
    return crc_table;
}

constexpr static std::array<uint32_t, 256> crc_table = __generate_crc_table();

constexpr int32_t _crc32(const char* data) {
    uint32_t crc = 0xFFFFFFFF;

    // Process each character of the string
    while (*data) {
        uint8_t byte = static_cast<uint8_t>(*data++);
        crc = crc_table[(crc ^ byte) & 0xFF] ^ (crc >> 8);
    }

    // return crc ^ 0xFFFFFFFF;
    // Map the uint32_t range to the int32_t range to avoid overflow
    return static_cast<int32_t>((crc ^ 0xFFFFFFFF) / 2);
}

constexpr int32_t _crc32(const str& p) {
	return _crc32(p.c_str());
}

constexpr int32_t operator "" _crc32(const char* p, size_t) {
    return _crc32(p);
}

// 大括号开始
#define _NL_RG_BEGIN {
#define _NL_RG_END }	


// 通过某个指针逆向找到所在对象的指针 - 从lubuv 中学来.
#if defined _MSC_VER && !defined _CRT_USE_BUILTIN_OFFSETOF
    #ifdef __cplusplus
        #define offsetof(s,m) ((::size_t)&reinterpret_cast<char const volatile&>((((s*)0)->m)))
    #else
        #define offsetof(s,m) ((size_t)&(((s*)0)->m))
    #endif
#else
	#ifndef offsetof
    	#define offsetof(s,m) __builtin_offsetof(s,m)
	#endif
#endif

// ptr 是当前指针(可以是任意类型), type 是要转换到的类型, member 是 ptr 对应对象中的成员名字
#define container_of(ptr, type, member) \
  ((type *) ((char *) (ptr) - offsetof(type, member)))


// 获取数组长度
#ifndef lenof
#define lenof(arr) (sizeof(arr) / sizeof(arr[0]))
#else
#error "NL use key world 'lenof' have been used."
#endif

#if !defined(_WIN32)
constexpr void strcpy_s(char* dest, const char* src) {
	// Linux 安全替代方案
	size_t dest_size = strlen(src);
	assert(dest_size == (sizeof(dest)/sizeof(*dest)));
	strncpy(dest, src, dest_size - 1);
	dest[dest_size - 1] = '\0';  // 强制终止
}
#endif

#if defined(_WIN32)
#define API_CALL __stdcall
#else
#define API_CALL
#endif

#endif	//	__CJ_TOOLS_BASE_INC_H__