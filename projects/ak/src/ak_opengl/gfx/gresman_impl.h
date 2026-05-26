#ifndef ak_gfx_resman_impl_h
#define ak_gfx_resman_impl_h

#include "ak/gfx/gresman.h"

#include <glad/glad.h>

bool
ak_gresman_acquire_tex(ak_gresman* grm,
                       ak_gresid gid,
                       GLuint* o_glint);

#endif
