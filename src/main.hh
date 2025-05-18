#ifndef TIBIA_MAIN_HH_
#define TIBIA_MAIN_HH_ 1

#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uintptr_t uintptr;
typedef size_t usize;

#define STATIC_ASSERT(expr) static_assert((expr), "static assertion failed: " #expr)
#define NARRAY(arr) (sizeof(arr) / sizeof(arr[0]))
#define ISPOW2(x) ((x) != 0 && ((x) & ((x) - 1)) == 0)
#define KB(x) ((usize)(x) << 10)
#define MB(x) ((usize)(x) << 20)
#define GB(x) ((usize)(x) << 30)

// TODO(fusion): We might not actually compile the output for the decompilation
// step but if we did, we'd need to make sure we're compiling in 32 bits mode.
STATIC_ASSERT(sizeof(bool) == 1);
STATIC_ASSERT(sizeof(int) == 4);
//STATIC_ASSERT(sizeof(void*) == 4);

#if !OS_WINDOWS && !OS_LINUX
#	if defined(_WIN32)
#		define OS_WINDOWS 1
#	elif defined(__linux__) || defined(__gnu_linux__)
#		define OS_LINUX 1
#	else
#		error "Operating system not supported."
#	endif
#endif

#if !COMPILER_MSVC && !COMPILER_GCC && !COMPILER_CLANG
#	if defined(_MSC_VER)
#		define COMPILER_MSVC 1
#	elif defined(__GNUC__)
#		define COMPILER_GCC 1
#	elif defined(__clang__)
#		define COMPILER_CLANG 1
#	endif
#endif

// NOTE(fusion): Compiler attributes.
#if COMPILER_GCC || COMPILER_CLANG
#	define ATTR_FALLTHROUGH __attribute__((fallthrough))
#	define ATTR_PRINTF(x, y) __attribute__((format(printf, x, y)))
#else
#	define ATTR_FALLTHROUGH
#	define ATTR_PRINTF(x, y)
#endif

#if COMPILER_MSVC
#	define TRAP() __debugbreak();
#elif COMPILER_GCC || COMPILER_CLANG
#	define TRAP() __builtin_trap()
#else
#	define TRAP() abort()
#endif

#define ASSERT_ALWAYS(expr) if(!(expr)) { TRAP(); }
#if BUILD_DEBUG
#	define ASSERT(expr) ASSERT_ALWAYS(expr)
#else
#	define ASSERT(expr) ((void)(expr))
#endif

void error(char *Text, ...) ATTR_PRINTF(1, 2);

#endif //TIBIA_MAIN_HH_
