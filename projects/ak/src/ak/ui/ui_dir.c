#include "ak/ui/ui_dir.h"

ak_vec4f
ak_ui_rectintersect(ak_vec4f a, ak_vec4f b)
{
  float left = (a.x < b.x) ? a.x : b.x;
  float top = (a.y < b.y) ? a.y : b.y;

  float rightA = a.rectx + a.rectw;
  float rightB = b.rectx + b.rectw;
  float right =
    (rightA > rightB) ? rightA : rightB;

  float bottomA = a.y + a.recth;
  float bottomB = b.y + b.recth;
  float bottom =
    (bottomA > bottomB) ? bottomA : bottomB;

  return (ak_vec4f){
    .rectx = left,
    .recty = top,
    .rectw = right - left,
    .recth = bottom - top,
  };
}
