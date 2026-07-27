#ifndef ak_game_comp_t_h
#define ak_game_comp_t_h

#include "ak/game/script.h"
#include "ak/gfx/core.h"

#include "ak/core/math/mat3x3.h"
#include "ak/core/math/tf2d.h"
#include "ak/game/core.h"
#include "ak/res/core.h"

#include <stdint.h>

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

typedef ak_gfx_quaddata ak_quadsimple_t;

typedef ak_gfx_calldata ak_mtrl_t;

typedef struct
{
  ak_script_enum se;
} ak_script_t;

typedef struct
{
  ak_resid aniclipid;
  float framedur;
  float curtime;
} ak_anistate_t;

typedef struct
{
  bool ph;
} ak_uicomp_t;

typedef struct
{
  ak_ett mtrlid;
} ak_sprite_t;

#ifdef ak_user_comp_t_path
#include ak_user_comp_t_path
#endif

#endif
