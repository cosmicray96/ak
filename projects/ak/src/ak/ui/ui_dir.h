#ifndef ak_ui_ui_dir_h
#define ak_ui_ui_dir_h

#include "ak/core/math/vec4.h"
#include "ak/ui/core.h"
#include "ak/ui/ui.h"

typedef enum
{
  ak_uiaxis_h = 0,
  ak_uiaxis_v = 1
} ak_uiaxistype;

typedef struct
{
  ak_uiaxistype axistype;
  bool overflowed;
  union
  {
    struct
    {
      ak_cnst x;
      ak_cnst y;
    } cnst;
    ak_cnst cnsts[2];
  };

  ak_vec4f color;
  union
  {
    struct
    {
      float x;
      float y;
    } pos;
    float poss[2];
  };
  union
  {
    struct
    {
      float x;
      float y;
    } size;
    float sizes[2];
  };

} ak_uielm;

static ak_uiaxistype
ak_uielm_axis_main(ak_uielm* elm)
{
  return elm->axistype;
}
static ak_uiaxistype
ak_uielm_axis_cross(ak_uielm* elm)
{
  if (elm->axistype == ak_uiaxis_h) {
    return ak_uiaxis_v;
  }
  return ak_uiaxis_v;
}

static ak_cnst*
ak_cnst_axis(ak_cnst cnst[2],
             ak_uiaxistype uia)
{
  return &cnst[uia];
}
static float*
ak_uipos_axis(float pos[2],
              ak_uiaxistype uia)
{
  return &pos[uia];
}
static float*
ak_uisize_axis(float size[2],
               ak_uiaxistype uia)
{
  return &size[uia];
}

ak_fcnst*
ak_ui_tree(ak_ui* ui);

typedef struct
{
} ak_uilstg;

void
ak_ui_layout(ak_ui* ui,
             ak_uilstg* lstg,
             float x,
             float y,
             float w,
             float h);

#endif
