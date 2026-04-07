#ifndef ak_core_errcode_h
#define ak_core_errcode_h

#define ak_errcode_list                     \
  X(ak_err)                                 \
  X(ak_ok)                                  \
  X(ak_err_invalid_args)                    \
  X(ak_err_invalid_type)                    \
  X(ak_err_crash_in_crash)                  \
  X(ak_err_unreachable)                     \
  X(ak_err_not_impled)                      \
  X(ak_err_null_ptr)                        \
  X(ak_err_index_out_of_bounds)             \
  X(ak_err_double_free)                     \
  X(ak_err_out_of_memory)                   \
  X(ak_err_bad_format)

typedef enum
{
#define X(name) name,
  ak_errcode_list
#undef X
} ak_errcode;

static const char* ak_errcode_str[] = {
#define X(name) #name,
  ak_errcode_list
#undef X
};

static const char*
ak_errcode_to_str(ak_errcode code)
{
  return ak_errcode_str[code];
}

#endif
