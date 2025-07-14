#ifndef _KPM_FNMATCH__H
#define _KPM_FNMATCH__H
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
char **kpm_fnmatch(char **patterns, char **strings, size_t pattern_count, size_t strings_count);

#endif