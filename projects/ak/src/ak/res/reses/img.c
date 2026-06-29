#include "ak/res/reses/img.h"

ak_ex ak_stmerr
ak_stm_read_img(ak_stm stm,
                ak_img* o_img,
                ak_alct alct);
ak_ex void
ak_img_destroy(ak_img* img);

ak_ex ak_res
ak_img_into_res(const ak_img* img);
