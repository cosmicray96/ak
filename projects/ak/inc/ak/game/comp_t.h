#ifndef ak_game_comp_t_h
#define ak_game_comp_t_h

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/tf2d.h"
#include "ak/game/stg/core.h"
#include "ak/gfx/mtrl.h"
#include <stdint.h>

#include "ak/gfx/mtrl.h"

typedef ak_tf2 ak_tf2d_t;
typedef ak_mat3 ak_gmat3_t;

typedef struct
{
  bool ph;
} ak_camera_t;
typedef struct
{
  uint32_t w;
  uint32_t h;
} ak_screen_t;
typedef struct
{
  bool ph;
} ak_rect_t;

typedef struct
{
  ak_ett base_id;
  ak_mtrl_quaddata data;
} ak_mtrl_t;

typedef struct
{
  ak_mtrl_enum me;
  ak_mtrl_basedata data;
} ak_mtrl_base_t;

#ifdef ak_user_comp_t_path
#include ak_user_comp_t_path
#endif

#endif
