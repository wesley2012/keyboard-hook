#ifdef KEYHOOK_EXPORTS
#define KEYHOOK_API extern "C" __declspec(dllexport) 
#else
#define KEYHOOK_API extern "C" __declspec(dllimport)
#endif


KEYHOOK_API void SetKbHook(void);
KEYHOOK_API void RemoveKbHook(void);


