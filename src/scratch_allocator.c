#include "scratch_allocator.h"
#include <stdlib.h>

#define SCRATCH_OVERHEAD 4096
#define SCRATCH0 8

static scratch_t* available = NULL;
static scratch_t* unavailable = NULL;


__attribute__((constructor))
static void scratch_init(void)
{
	available = calloc(SCRATCH0, sizeof *available);
	arena_init((arena_t*)available, SCRATCH_OVERHEAD);
	for (int32_t i = 0; i < SCRATCH0 - 1; ++i)
	{
		available[i].next = available + i + 1;
	}
}

static void scratch_more(const int32_t size)
{
	available = calloc(SCRATCH0, sizeof *available);
	arena_init((void*)available, size);
	for (int32_t i = 0; i < SCRATCH0 - 1; ++i)
	{
		available[i].next = available + i + 1;
	}
}

arena_t* scratch_checkout(void)
{
	if (!available)
	{
		scratch_more(SCRATCH_OVERHEAD);
	}
	scratch_t* res = available;
	res->arena.size = 0;
	available->next = unavailable;
	unavailable = res;
	return (arena_t*)res;
}

arena_t* scratch_checkout_size(const int32_t size)
{
	if (!available)
	{
		scratch_more(size + SCRATCH_OVERHEAD);
	}
	else if (available->arena.cap < size)
	{
		arena_grow((arena_t*)available, size);
	}
	scratch_t* res = available;
	res->arena.size = 0;
	available->next = unavailable;
	unavailable = res;
	return (arena_t*)res;
}

void scratch_checkin(scratch_t* scratch)
{
	scratch_t** s;
	for(s = &unavailable; *s; *s = (*s)->next)
	{
		if (*s == scratch)
		{
			*s = scratch->next;
			scratch->next = available;
			available = scratch;
		}
	}
}

void scratch_checkin_all(void)
{
	if (unavailable)
	{
		scratch_t* s = unavailable;
		for(/* LOL */; s->next; s = s->next);
		s->next = available;
		available = unavailable;
		unavailable = NULL;
	}
}
