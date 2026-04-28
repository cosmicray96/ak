#ifndef ak_game_stg_h
#define ak_game_stg_h

#include <stdint.h>

typedef uint32_t ak_ett;

typedef struct
{
  uint32_t x;
  uint32_t y;
} ak_comp1;

typedef struct
{
  uint32_t x;
  uint32_t y;
  uint32_t w;
  uint32_t h;
} ak_comp2;

#endif
