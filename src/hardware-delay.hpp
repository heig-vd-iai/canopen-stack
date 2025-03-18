#pragma once

#if defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#else
extern void usleep(int);
#endif
