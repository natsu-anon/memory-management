#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include "vector.h"

class Quadtree {

public:
	Quadtree* parent;
	Quadtree* neChild;
	Quadtree* nwChild;
	Quadtree* swChild;
	Quadtree* seChild;
	vector_t origin;
	vector_t halfDims;

	Quadtree();
	Quadtree(Quadtree* parent);
	~Quadtree();
	void divide();
	void leaves(std::vector<Quadtree*>* dest);
};

#endif // QUADTREE_HPP
