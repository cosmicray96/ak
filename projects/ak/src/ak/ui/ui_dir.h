#ifndef ak_ui_ui_dir_h
#define ak_ui_ui_dir_h

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
      ak_uibound x;
      ak_uibound y;
    } bound;
    ak_uibound bounds[2];
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

ak_cnst*
ak_uielm_cnst_axis(ak_uielm* elm,
                   ak_uiaxistype uia);
ak_uibound*
ak_uielm_bound_axis(ak_uielm* elm,
                    ak_uiaxistype uia);
float*
ak_uielm_pos_axis(ak_uielm* elm,
                  ak_uiaxistype uia);
float*
ak_uielm_size_axis(ak_uielm* elm,
                   ak_uiaxistype uia);

ak_fcnst*
ak_ui_tree(ak_ui* ui);

void
ak_ui_layout(ak_ui* ui,
             uint32_t width,
             uint32_t height);

#endif
