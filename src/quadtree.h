#ifndef QUADTREE_H
#define QUADTREE_H

#include "vector.h"

typedef struct quadtree_t {
	struct quadtree_t* parent;
	union {
		struct quadtree_t* child[4];
		struct {
			struct quadtree_t* ne_child;
			struct quadtree_t* nw_child;
			struct quadtree_t* se_child;
			struct quadtree_t* sw_child;
		};
	};
	vector_t origin;
	vector_t half_dims;
} quadtree_t;


void quadtree_divide(quadtree_t* quadtree, quadtree_t* children);

#endif /* End QUADTREE_H */
