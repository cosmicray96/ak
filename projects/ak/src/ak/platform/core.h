#ifndef ak_platform_core_h
#define ak_platform_core_h

#include <stdint.h>
typedef struct ak_platform ak_platform;
typedef struct ak_window ak_window;

typedef enum
{
  ak_winevt_none,
  ak_winevt_close,
  ak_winevt_mouse
} ak_winevt_type;

typedef enum
{
  ak_winevt_mouse_none,
  ak_winevt_mouse_left,
  ak_winevt_mouse_mid,
  ak_winevt_mouse_right,
} ak_winevt_mousebtn;

typedef struct
{
  ak_winevt_type type;
  ak_window* win;
  union
  {
    struct
    {
      int32_t x;
      int32_t y;
      ak_winevt_mousebtn btn;
    } mouse;
  };
} ak_winevt;

#endif
