// vim:ft=c:ts=2:sw=2:sts=2:et

#include <buf>
#include <vec>

#include <string.h>
#include <stdlib.h>

//
//  +---------+      +---------+
//  | 0 * siz |      |         |
//  | 1 * siz | push |         |
//  | 2 * siz |  =>  |         |
//  | 3 * siz |      |         |
//  +---------+      | 4 * siz |
//                   +---------+
//

// (de)initialization
struct vec* vec_new(size_t siz) {
  struct vec* vec = malloc(sizeof *vec);
  *(size_t*)&vec->siz = siz;
  vec->len = 0;
  return vec;
}

void vec_del(struct vec* vec) {
  free(vec);
}

// getters
size_t vec_siz(struct vec* vec) {
  return vec ? vec->siz : 0;
}

size_t vec_len(struct vec* vec) {
  return vec ? vec->len : 0;
}

// traits
bool vec_is_empty(struct vec* vec) {
  return !vec_len(vec);
}

bool vec_is_nulled(struct vec* vec) {
  return vec ? arr_is_nulled(vec->buf, vec_siz(vec) * vec_len(vec)) : false;
}

// setters
struct vec* vec_set_len(struct vec* vec, size_t len) {
  if (vec) {
    vec = realloc(vec, offsetof(struct vec, buf) + vec->siz * len);
    if (vec->len < len) memset(vec->buf + vec->len, 0, len - vec->len);
    vec->len = len;
  }
  return vec;
}

struct vec* vec_ensure_len(struct vec* vec, size_t len) {
  return vec && vec->len < len ? vec_set_len(vec, len) : vec;
}

// accessors
void* vec_at(struct vec* vec, size_t idx) {
  return vec ? vec->buf + vec->siz * idx : NULL;
}

void* vec_tail(struct vec* vec) {
  return vec_at(vec, vec_len(vec) - 1);
}

// push / pop
struct vec* vec_push(struct vec* vec, void* val) {
  vec = vec_set_len(vec, vec_len(vec) + 1);
  memcpy(vec_tail(vec), val, vec->siz);
  return vec;
}

struct vec* vec_pop(struct vec* vec, void* val) {
  if (vec_is_empty(vec)) return NULL;
  memcpy(val, vec_tail(vec), vec->siz);
  vec = vec_set_len(vec, vec_len(vec) - 1);
  return vec;
}


void* vec_to_fp(struct vec* vec) {
  return vec ? vec->buf : NULL;
};

struct vec* fp_to_vec(void* vec) {
  return vec ? vec - offsetof(struct vec, buf) : NULL;
};


void* fp_vec_new(size_t siz) {
  return vec_to_fp(vec_new(siz));
}

void fp_vec_del(void* vec) {
  return vec_del(fp_to_vec(vec));
}

bool fp_vec_is_empty(void* vec) {
  return vec_is_empty(fp_to_vec(vec));
}

bool fp_vec_is_nulled(void* vec) {
  return vec_is_nulled(fp_to_vec(vec));
}

// getters
size_t fp_vec_siz(void* vec) {
  return vec_siz(fp_to_vec(vec));
}
size_t fp_vec_len(void* vec) {
  return vec_len(fp_to_vec(vec));
}

void* fp_vec_set_len(void* vec, size_t len) {
  return vec_to_fp(vec_set_len(fp_to_vec(vec), len));
}

void* fp_vec_ensure_len(void* vec, size_t len) {
  return vec_to_fp(vec_ensure_len(fp_to_vec(vec), len));
}

void* fp_vec_push(void* vec, void* val) {
  return vec_to_fp(fp_vec_push(fp_to_vec(vec), val));
}

void* fp_vec_pop(void* vec, void* val) {
  return vec_to_fp(fp_vec_pop(fp_to_vec(vec), val));
}
