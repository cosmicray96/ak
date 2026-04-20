#ifndef ak_platform_core_h
#define ak_platform_core_h

#include <stdint.h>
typedef struct ak_plat ak_plat;

typedef enum
{
  ak_winevt_none,
  ak_winevt_close,
  ak_winevt_resize,
  ak_winevt_mouse,
  ak_winevt_key,
  ak_winevt_visible,
  ak_winevt_invisible,
  ak_winevt_focus_gained,
  ak_winevt_focus_lost,
} ak_winevt_type;

typedef enum
{
  ak_mouseaction_none,
  ak_mouseaction_pressed,
  ak_mouseaction_released,
  ak_mouseaction_dclicked
} ak_mouseaction;

typedef enum
{
  ak_mouse_none,
  ak_mouse_left,
  ak_mouse_mid,
  ak_mouse_right,
} ak_mousecode;

typedef struct
{
  ak_mousecode code;
  ak_mouseaction action;
  int32_t x;
  int32_t y;
  int32_t dx;
  int32_t dy;
} ak_mouseevt;

typedef enum
{
  ak_key_none,

  // chars
  /*
ak_key_space,
ak_key_apostrophe,
ak_key_comma,
ak_key_minus,
ak_key_period,
ak_key_lbracket,
ak_key_rbracket,
ak_key_slash,
ak_key_backslash,
ak_key_colon,
ak_key_semicolon,
ak_key_equal,
  */
  ak_key_0,
  ak_key_1,
  ak_key_2,
  ak_key_3,
  ak_key_4,
  ak_key_5,
  ak_key_6,
  ak_key_7,
  ak_key_8,
  ak_key_9,
  ak_key_a,
  ak_key_b,
  ak_key_c,
  ak_key_d,
  ak_key_e,
  ak_key_f,
  ak_key_g,
  ak_key_h,
  ak_key_i,
  ak_key_j,
  ak_key_k,
  ak_key_l,
  ak_key_m,
  ak_key_n,
  ak_key_o,
  ak_key_p,
  ak_key_q,
  ak_key_r,
  ak_key_s,
  ak_key_t,
  ak_key_u,
  ak_key_v,
  ak_key_w,
  ak_key_x,
  ak_key_y,
  ak_key_z,

  // function
  ak_key_f1,
  ak_key_f2,
  ak_key_f3,
  ak_key_f4,
  ak_key_f5,
  ak_key_f6,
  ak_key_f7,
  ak_key_f8,
  ak_key_f9,
  ak_key_f10,
  ak_key_f11,
  ak_key_f12,

  // navigation
  ak_key_up,
  ak_key_down,
  ak_key_left,
  ak_key_right,
  ak_key_home,
  ak_key_end,
  ak_key_pageup,
  ak_key_pagedown,
  ak_key_insert,
  ak_key_delete,

  // editing
  ak_key_enter,
  ak_key_backspace,
  ak_key_tab,
  ak_key_escape,

  // modifiers
  ak_key_lshift,
  ak_key_rshift,
  ak_key_lctrl,
  ak_key_rctrl,
  ak_key_lalt,
  ak_key_ralt,
  ak_key_lsuper,
  ak_key_rsuper,
  ak_key_capslock,
  ak_key_numlock,
  ak_key_scrolllock,

  // numpad
  ak_key_np0,
  ak_key_np1,
  ak_key_np2,
  ak_key_np3,
  ak_key_np4,
  ak_key_np5,
  ak_key_np6,
  ak_key_np7,
  ak_key_np8,
  ak_key_np9,
  /*
ak_key_np_decimal,
ak_key_np_divide,
ak_key_np_multiply,
ak_key_np_minus,
ak_key_np_plus,
ak_key_np_enter,

// misc
ak_key_printscreen,
  */
  ak_key_count
} ak_keycode;

typedef enum
{
  ak_keyaction_none,
  ak_keyaction_pressed,
  ak_keyaction_longpress,
  ak_keyaction_released
} ak_keyaction;

typedef enum
{
  ak_keymode_none = 0,
  ak_keymode_ctrl = 1 << 1,
  ak_keymode_shift = 1 << 2,
  ak_keymode_alt = 1 << 3,
} ak_keymode;

typedef struct
{
  ak_keycode code;
  ak_keyaction action;
  ak_keymode mode;
} ak_keyevt;

typedef struct
{
  ak_winevt_type type;
  union
  {
    ak_mouseevt mouse;
    ak_keyevt key;
    struct
    {
      uint32_t w;
      uint32_t h;
    } resize;
  };
} ak_winevt;

#endif
