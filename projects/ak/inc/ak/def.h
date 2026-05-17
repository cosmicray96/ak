#ifndef ak_def_h
#define ak_def_h

// --- count args ---

#define ak_nargs_(_1,                       \
                  _2,                       \
                  _3,                       \
                  _4,                       \
                  _5,                       \
                  _6,                       \
                  _7,                       \
                  _8,                       \
                  _9,                       \
                  _10,                      \
                  _11,                      \
                  _12,                      \
                  _13,                      \
                  _14,                      \
                  _15,                      \
                  _16,                      \
                  N,                        \
                  ...)                      \
  N

#define ak_nargs(...)                       \
  ak_nargs_(__VA_ARGS__,                    \
            16,                             \
            15,                             \
            14,                             \
            13,                             \
            12,                             \
            11,                             \
            10,                             \
            9,                              \
            8,                              \
            7,                              \
            6,                              \
            5,                              \
            4,                              \
            3,                              \
            2,                              \
            1)

#endif
