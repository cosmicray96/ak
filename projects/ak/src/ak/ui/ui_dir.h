#ifndef ak_ui_ui_dir_h
#define ak_ui_ui_dir_h

#include "ak/coll/da.h"
#include "ak/ui/core.h"
#include "ak/ui/ui.h"

typedef enum
{
  ak_uiaxis_h,
  ak_uiaxis_v
} ak_uiaxistype;

typedef struct
{
  ak_uiaxistype axistype;
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

ak_uiaxistype
ak_uielm_axis_main(ak_uielm* elm);
ak_uiaxistype
ak_uielm_axis_cross(ak_uielm* elm);

ak_cnst*
ak_cnst_axis(ak_cnst cnst[2],
             ak_uiaxistype uia);
ak_uibound*
ak_uibound_axis(ak_uibound bound[2],
                ak_uiaxistype uia);
float*
ak_uipos_axis(float pos[2],
              ak_uiaxistype uia);
float*
ak_uisize_axis(float size[2],
               ak_uiaxistype uia);

ak_fcnst*
ak_ui_tree(ak_ui* ui);

typedef struct
{
  ak_fcnst_itbfs it;
  ak_da bounds;
} ak_uilstg;

void
ak_ui_layout(ak_ui* ui,
             ak_uilstg* lstg,
             uint32_t width,
             uint32_t height);

#endif
