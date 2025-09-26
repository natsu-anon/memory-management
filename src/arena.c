#include "arena.h"
#include <stdlib.h>
#include <assert.h>

#define ARENA_OVERHEAD 4096

// Thanks, GingerBill
static inline int32_t align_forward(void* ptr, uint32_t pow2)
{
	assert((pow2 & (pow2 - 1)) == 0 && "can only align to powers of 2!");
	uint64_t addr = (uint64_t)ptr;
	int32_t modulo = addr & (pow2 - 1);
	return modulo ? (int32_t)pow2 - modulo : 0;
}


void arena_init(arena_t* arena, int32_t cap)
{
	arena->allocation = malloc(cap);
	// assert
	arena->cap = cap;
	arena->size = 0;
}

void arena_grow(arena_t* arena, int32_t new_cap)
{
	if (new_cap > arena->cap)
	{
		void* ptr = realloc(arena->allocation, new_cap + ARENA_OVERHEAD);
		assert(ptr && "failed to realloc up in arena_grow!");
		arena->allocation = ptr;
		arena->cap = new_cap + ARENA_OVERHEAD;
	}
}

void arena_resize(arena_t* arena, int32_t cap)
{
	arena->allocation = realloc(arena->allocation, cap);
	// asssert
	arena->cap = cap;
	arena->size = arena->size < cap ? arena->size : cap;
}


void* arena_alloc(arena_t* arena, int32_t size)
{
	assert(arena->size + size <= arena->cap && "arena_alloc overflow!");
	void* res = arena->allocation + arena->size;
	arena->size += size;
	return res;
}

void* arena_alloc_align(arena_t* arena, int32_t size, uint32_t pow2)
{
	const int32_t padding = align_forward(arena->allocation + arena->size, pow2);
	assert(arena->size + size + padding <= arena->cap && "arena_alloc_align overflow!");
	void* res = arena->allocation + arena->size + padding;
	arena->size += size + padding;
	return res;
}

void arena_free_all(arena_t* arena)
{
	arena->size = 0; // and that's it :D
}
