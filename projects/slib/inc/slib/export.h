#ifndef slib_export_h
#define slib_export_h

#define slib_ex

#if defined(slib_shared)
#undef slib_ex

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef slib_inside
#define slib_ex __declspec(dllexport)
#else
#define slib_ex __declspec(dllimport)
#endif

#elif defined(__GNUC__) || defined(__clang__)
#ifdef slib_inside
#define slib_ex __attribute__((visibility("default")))
#else
#define slib_ex
#endif

#else
#error "unknown compiler"
#endif

#endif

#endif
