
// ----- calling convention -----
#if defined(_WIN32)
#define CORE_CALL __cdecl
#else
#define CORE_CALL /* nothing */
#endif

// ----- visibility / import-export -----
#if defined(_WIN32)
#if defined(CORE_EXPORTS)
#define CORE_API __declspec(dllexport)
#else
#define CORE_API __declspec(dllimport)
#endif
#else
#define CORE_API __attribute__((visibility("default")))
#endif

// ----- C ABI export (for P/Invoke) -----
#ifdef __cplusplus
#define CORE_EXTERN_C extern "C"
#else
#define CORE_EXTERN_C
#endif

#define CORE_API_C CORE_EXTERN_C CORE_API