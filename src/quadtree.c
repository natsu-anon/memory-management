#include "quadtree.h"
/* #include <stdlib.h> */
#include <string.h>
#include <stdio.h>

// NOTE: not asserting because I reuse
void quadtree_divide(quadtree_t* quadtree, quadtree_t* children) {
	const float delta_x = 0.5f * quadtree->half_dims.x;
	const float delta_y = 0.5f * quadtree->half_dims.y;
	const vector_t half_dims = { .x = delta_x, .y = delta_y };
	for (int i = 0; i < 4; ++i) {
		memcpy(&children[i].half_dims, &half_dims, sizeof(vector_t));
		memcpy(&children[i].origin, &quadtree->origin, sizeof(vector_t));
		vector_translate(&children[i].origin, i % 2 ? delta_x : -delta_x, i < 2 ? delta_y : -delta_y);
		quadtree->child[i] = children + i;
		children[i].parent = quadtree;
		memset(children[i].child, 0x00, 4 * sizeof(quadtree_t*));
		/* children[i].child = { 0 }; */
	}
}
