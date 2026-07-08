#ifndef ak_core_errcode_h
#define ak_core_errcode_h

#define ak_errcode_list                     \
  X(ak_ok)                                  \
  X(ak_err)                                 \
  X(ak_err_invalid_args)                    \
  X(ak_err_invalid_state)                   \
  X(ak_err_invalid_operation)               \
  X(ak_err_index_out_of_bounds)             \
  X(ak_err_allocation_failed)               \
  X(ak_err_not_found)                       \
  X(ak_err_double_remove)                   \
  X(ak_err_stm_end)                         \
  X(ak_err_stm_badformat)                   \
  X(ak_err_unreachable)                     \
  X(ak_err_not_impled)

typedef enum
{
#define X(name) name,
  ak_errcode_list
#undef X
    ak_errcode_count
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

#define ak_err_try(x)                       \
  do {                                      \
    ak_errcode ak_err_var = x;              \
    if (ak_err_var != ak_ok) {              \
      return ak_err_var;                    \
    }                                       \
  } while (0)

#endif
