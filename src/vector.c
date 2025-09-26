#include "vector.h"

void vector_translate(vector_t* dest, const float x, const float y)
{
	dest->x += x;
	dest->y += y;
}

void vector_sum(vector_t* dest, vector_t* a, vector_t* b)
{
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
}

void vector_scale(vector_t* dest, const float k, vector_t* v)
{
	dest->x = k * v->x;
	dest->y = k * v->y;
}
