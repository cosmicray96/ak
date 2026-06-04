#ifndef ak_ui_ui_dir_h
#define ak_ui_ui_dir_h

#include "ak/core/math/vec4.h"
#include "ak/core/mem/allocator.h"
#include "ak/ui/core.h"
#include "ak/ui/ui.h"

//===== ak_uielm =====//
typedef enum
{
  ak_uiaxis_h = 0,
  ak_uiaxis_v = 1
} ak_uiaxistype;

typedef struct
{
  ak_uiaxistype axistype;
  bool clipping;
  bool visible;
  ak_vec4f color;
  union
  {
    struct
    {
      ak_cnst x;
      ak_cnst y;
    } cnst;
    ak_cnst cnsts[2];
  };

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
  return ak_uiaxis_h;
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

ak_vec4f
ak_ui_rectintersect(ak_vec4f a, ak_vec4f b);

//===== ui_layout =====//
ak_uilstg
ak_uilstg_make(ak_alct alct);
void
ak_uilstg_destroy(ak_uilstg* lstg);
void
ak_ui_layout(ak_ui* ui,
             float x,
             float y,
             float w,
             float h);

ak_uirstg
ak_uirstg_make(ak_alct alct);
void
ak_uirstg_destroy(ak_uirstg* rstg);

#endif
