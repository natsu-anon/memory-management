#ifndef ARENA_H
#define ARENA_H

#include <stdint.h>

typedef struct arena_t
{
	uint8_t* allocation;
	int32_t cap;
	int32_t size;
} arena_t;

void arena_init(arena_t* arena, int32_t cap);

void arena_resize(arena_t* arena, int32_t cap);

void arena_grow(arena_t* arena, int32_t new_cap);

void* arena_alloc(arena_t* arena, int32_t size);

void* arena_alloc_align(arena_t* arena, int32_t size, uint32_t pow2);

void arena_free_all(arena_t* arena);
#endif /* End ARENA_H */
