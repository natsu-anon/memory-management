

#ifndef SCRATCH_H
#define SCRATCH_H

#include "arena.h"

// in scratch.c:
// static scratch_t* available = NULL;
// static scratch_t* unavailable = NULL;
typedef struct scratch_t
{
	arena_t arena; // scratch is a pun on arenas, so I can pass scratch ptrs into arena funcs
	struct scratch_t* next;
} scratch_t;


arena_t* scratch_checkout(void);

arena_t* scratch_checkout_size(const int32_t size);

void scratch_checkin(scratch_t* scratch);

void scratch_checkin_all(void);

#endif /* End SCRATCH_H */
