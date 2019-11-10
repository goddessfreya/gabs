#include <bits/wordsize.h>

#if __WORDSIZE == 32
#include "pyconfig-32.h"
#elif __WORDSIZE == 64
#include "pyconfig-64.h"
#else
#error "Unkown word size"
#endif
