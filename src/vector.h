#ifndef VECTOR_H
#define VECTOR_H

typedef union vector_t {
	float elem[2];
	struct {
		float x;
		float y;
	};
} vector_t;

#ifdef __cplusplus
extern "C" {
#endif
void vector_translate(vector_t* dest, const float x, const float y);

void vector_sum(vector_t* dest, vector_t* a, vector_t* b);

void vector_scale(vector_t* dest, const float k, vector_t* v);
#ifdef __cplusplus
}
#endif

#endif /* End VECTOR_H */
