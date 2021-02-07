/* dgl.h - v0.1
   No warranty is offered or implied; Use this code at your own risk

   This file is written mostly for my self and a work in progress!

 ============================================================================
   You MUST
      #define DGL_IMPLEMENTATION
   in EXACTLY _one_ C or C++ file that includes this header, BEFORE the
   include, like this:
      #define DGL_IMPLEMENTATION
      #include "dgl.h"
   All other files should just #include "dgl.h" without the #define.
 ============================================================================

LICENSE
   This software is dual-licensed (MIT and public domain) -- See the LICENSE file in this repository
   (https://github.com/0xd61/dgl/blob/main/LICENSE) for more information.

CREDITS
 Written by Daniel Glinka.

 Credits to Sean Barrett and his stb style libraries which inspired this library.
*/

#ifndef DGL_H
#define DGL_H

//
// Compilers and Platforms
//

#ifndef COMPILER_MSVC
#define COMPILER_MSVC 0
#endif

#ifndef COMPILER_LLVM
#define COMPILER_LLVM 0
#endif

#if !COMPILER_MSVC && !COMPILER_LLVM
#if _MSC_VER
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#elif __llvm__
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif

//
// Useful defines
//

#define internal static
#define global static
#define local_persist static
#define local_inline static inline

// NOTE(dgl): Casts can be very annoying while debugging. This is to identiy/search for them faster.
#define cast(type) (type)
#define array_count(array) (sizeof(array) / sizeof((array)[0]))


#if DGL_DEBUG
#include <stdio.h> // fprintf
#define assert(cond, msg) do                                                   \
{                                                                              \
    if (!(cond))                                                               \
    {                                                                          \
      fprintf(stderr, "Fatal error: %s:%d: assertion '%s' failed with %s\n",   \
      __FILE__, __LINE__, #cond, #msg);                                        \
      __builtin_trap();                                                        \
    }                                                                          \
} while(0)
#else
#define assert(cond, msg)
#endif

//
// Types
//

#define kilobytes(value) ((value)*1024LL)
#define megabytes(value) (kilobytes(value)*1024LL)
#define gigabytes(value) (megabytes(value)*1024LL)
#define terabytes(value) (gigabytes(value)*1024LL)

typedef unsigned char      uint8 ;
typedef   signed char       int8 ;
typedef unsigned short     uint16;
typedef   signed short      int16;
typedef unsigned int       uint32;
typedef   signed int        int32;
typedef unsigned long long uint64;
typedef          long long  int64;
typedef          float     real32;
typedef          double    real64;
typedef          int32     bool32;
#define true 1
#define false 0

// TODO(dgl): could we define a intptr without stdint.h?
#include <stdint.h>
typedef uintptr_t uintptr;
#include <stddef.h>
typedef size_t usize;

//
// Safe Truncate
//

local_inline uint32
dgl_safe_truncate_uint32(uint64 value)
{
    assert(value <= 0xFFFFFFFF, "Failed to safely truncate value");
    uint32 result = cast(uint32)value;
    return(result);
}

local_inline int32
dgl_safe_truncate_int32(int64 value)
{
    assert(value <= 0xFFFFFFFF, "Failed to safely truncate value");
    int32 result = cast(int32)value;
    return(result);
}

local_inline uint32
dgl_safe_size_to_uint32(usize value)
{
    assert(value <= 0xFFFFFFFF, "Failed to safely truncate value");
    uint32 result = cast(uint32)value;
    return(result);
}

local_inline int32
dgl_safe_size_to_int32(usize value)
{
    assert(value <= 0xFFFFFFFF, "Failed to safely truncate value");
    int32 result = cast(int32)value;
    return(result);
}


//
// Intrinsics
//

#include <math.h>
local_inline uint32
dgl_round_real32_to_uint32(real32 value)
{
    assert(value >= 0.0f, "Number must be positive");
    uint32 result = (uint32)roundf(value);
    return(result);
}

local_inline int32
dgl_round_real32_to_int32(real32 value)
{
    int32 result = (int32)roundf(value);
    return(result);
}

// NOTE(dgl): In the platform layer we use the native __sync_val_compare_and_swap
// to be more flexible. We should change this in the future to be more consistent.
// But this is easier for now, to support different types.
#if COMPILER_LLVM
local_inline uint32
dgl_atomic_compare_exchange_uint32(uint32 volatile *value, uint32 new, uint32 expected)
{
    uint32 result = __sync_val_compare_and_swap(value, expected, new);
    return(result);
}
local_inline int32
dgl_atomic_compare_exchange_int32(int32 volatile *value, int32 new, int32 expected)
{
    int32 result = __sync_val_compare_and_swap(value, expected, new);
    return(result);
}
local_inline uintptr
dgl_atomic_compare_exchange_uintptr(uintptr volatile *value, uintptr new, uintptr expected)
{
    uintptr result = __sync_val_compare_and_swap(value, expected, new);
    return(result);
}
// TODO(dgl): not tested
#elif COMPILER_MSVC
local_inline uint32
dgl_atomic_compare_exchange_uint32(uint32 volatile *value, uint32 new, uint32 expected)
{
    uint32 result = _InterlockedCompareExchange((long *)value, new, expected);

    return(result);
}
local_inline int32
dgl_atomic_compare_exchange_int32(int32 volatile *value, int32 new, int32 expected)
{
    int32 result = _InterlockedCompareExchange((long *)value, new, expected);
    return(result);
}
local_inline uintptr
dgl_atomic_compare_exchange_uintptr(uintptr volatile *value, uintptr new, uintptr expected)
{
    uintptr result = _InterlockedCompareExchange(value, new, expected);
    return(result);
}
#else
// TODO(dgl): support other compilers
#endif

#endif // DGL_H
