#ifndef __LOGREGX__
#define __LOGREGX__
#ifdef __cplusplus
// https://stackoverflow.com/a/48841860/8642222
extern "C"
{
#endif

#include <stdbool.h>

bool replaceWithOne(const char*regexp, const char *rewrite, const char* original, const char *single, const char *global, int globalCount);

#ifdef __cplusplus
}
#endif

#endif

