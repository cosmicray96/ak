#ifndef ak_platform_core_h
#define ak_platform_core_h

typedef struct ak_platform ak_platform;
typedef struct ak_window ak_window;

typedef enum
{
  ak_winevt_none,
  ak_winevt_close,
} ak_winevt_type;

typedef struct
{
  ak_winevt_type type;
  ak_window* win;
} ak_winevt;

#endif
