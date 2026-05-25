#ifndef ak_gfx_gresman_h
#define ak_gfx_gresman_h

#include "ak/core/mem/allocator.h"
#include "ak/gfx/core.h"
#include "ak/system/resman.h"
#include <stdint.h>

typedef enum
{
  ak_grestype_tex,
  ak_grestype_fb
} ak_grestype;

typedef enum
{
  ak_gres_not_loaded,
  ak_gres_loading,
  ak_gres_loaded,
} ak_gres_status;

typedef struct ak_gresman ak_gresman;
ak_gresman*
ak_gresman_startup(ak_resman* rm,
                   ak_gfx* gf,
                   ak_alct alct);
void
ak_gresman_shutdown(ak_gresman* grm);

void
ak_gresman_register_tex_from_rid(
  ak_gresman* grm,
  ak_gresid gid,
  ak_resid rid);

void
ak_gresman_register_framebuffer(
  ak_gresman* grm,
  ak_gresid gid);

ak_gres_status
ak_gresman_status(ak_gresman* grm,
                  ak_gresid gid);
ak_grestype
ak_gresman_type(ak_gresman* grm,
                ak_gresid gid);

void
ak_gresman_load(ak_gresman* grm,
                ak_gresid gid);
void
ak_gresman_unload(ak_gresman* grm,
                  ak_gresid gid);
void
ak_gresman_release(ak_gresman* grm,
                   ak_gresid gid);
void
ak_gresman_update(ak_gresman* grm);

#endif
