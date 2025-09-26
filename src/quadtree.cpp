#include "quadtree.hpp"
#include <string.h>
#include <assert.h>

Quadtree::Quadtree() {
	parent = nullptr;
	neChild = nullptr;
	nwChild = nullptr;
	swChild = nullptr;
	seChild = nullptr;
}

Quadtree::Quadtree(Quadtree* parent) {
	this->parent = parent;
	neChild = nullptr;
	nwChild = nullptr;
	swChild = nullptr;
	seChild = nullptr;
}

Quadtree::~Quadtree() {
	parent = nullptr;
	if (neChild != nullptr) {
		delete neChild;
		delete nwChild;
		delete swChild;
		delete seChild;
		neChild = nullptr;
		nwChild = nullptr;
		swChild = nullptr;
		seChild = nullptr;
	}
}

void Quadtree::divide() {
	assert(neChild == nullptr && "cannot further divide an already divided quadtree");
	neChild = new Quadtree(this);
	nwChild = new Quadtree(this);
	swChild = new Quadtree(this);
	seChild = new Quadtree(this);
	vector_t halfDims1;
	vector_scale(&halfDims1, 0.5f, &halfDims);
	memcpy(&neChild->halfDims, &halfDims1, sizeof(vector_t));
	memcpy(&nwChild->halfDims, &halfDims1, sizeof(vector_t));
	memcpy(&swChild->halfDims, &halfDims1, sizeof(vector_t));
	memcpy(&seChild->halfDims, &halfDims1, sizeof(vector_t));
	memcpy(&neChild->origin, &origin, sizeof(vector_t));
	memcpy(&nwChild->origin, &origin, sizeof(vector_t));
	memcpy(&swChild->origin, &origin, sizeof(vector_t));
	memcpy(&seChild->origin, &origin, sizeof(vector_t));
	const float deltaX = halfDims1.x;
	const float deltaY = halfDims1.y;
	vector_translate(&neChild->origin, deltaX, deltaY);
	vector_translate(&nwChild->origin, -deltaX, deltaY);
	vector_translate(&swChild->origin, -deltaX, -deltaY);
	vector_translate(&seChild->origin, deltaX, -deltaY);
}

void Quadtree::leaves(std::vector<Quadtree*>* dest) {
	if (neChild == nullptr) {
		dest->push_back(this);
	}
	else {
		neChild->leaves(dest);
		nwChild->leaves(dest);
		swChild->leaves(dest);
		seChild->leaves(dest);
	}
}
