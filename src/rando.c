#include "rando.h"
#include <stdlib.h>
#include <stdint.h>



float rand01() {
	#if _WIN32
	union { uint32_t u32; float f; } u = { .u32 = rand() << 7 | rand() >> 8 | 0x40000000 };
	#else
	union { uint32_t u32; float f; } u = { .u32 = rand() >> 9 | 0x40000000 };
	#endif
    return u.f - 2.0;
}
