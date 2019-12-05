#ifndef L_FUNCTIONAL_INCLUDED
#define L_FUNCTIONAL_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <glib.h>

#ifdef __GNUC__
static inline void __autofree(void *p) {
  void **_p = (void **)p;
  free (*_p);
}

static inline G_GNUC_PURE bool str_empty(const char *str) {return str[0] == '\0';}

#define array_foreach(p) for (; *p != NULL; ++p)

#define array_find(arr, ...)                  \
({                                            \
  array_foreach(arr) if (__VA_ARGS__) break;  \
  *arr;                                       \
})

#define lambda(return_type, ...) \
({                               \
  return_type __fn__ __VA_ARGS__ \
  __fn__;                        \
})
#endif //__GNUC__


#define union_decl(alg, ...)\
typedef struct alg {        \
  enum { __VA_ARGS__ } kind;\
  union {                   

#define union_type(type, ...)                              \
  struct type { __VA_ARGS__} type;

#define union_end(alg)                                     \
  };} alg;

#define union_set(instance, type, ...)                     \
  G_STMT_START {                                           \
    (instance)->kind = (type);                             \
    (instance)->type = (struct type) { __VA_ARGS__};       \
  }G_STMT_END

#define union_new(alg, type, ...)                          \
({  \
  alg *instance = g_new(alg, 1);                           \
  instance->kind = (type);                                 \
  instance->type = (struct type) {__VA_ARGS__};            \
  instance;                                                \
})

#define g_assert_e(...)                                                              \
    ({                                                                               \
        g_assert(__VA_ARGS__);                                                       \
        NULL;                                                                        \
    })

#define union_fail(...)(g_assert_e(((void)(__VA_ARGS__), false)), (__VA_ARGS__))

#endif //L_FUNCTIONAL_INCLUDED

