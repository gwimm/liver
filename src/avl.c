// vim:ft=c:ts=2:sw=2:sts=2:et

#include <avl>
#include <node>

#include <string.h>

// struct avl {
//   struct node* root;
//   cmp_fun_t cmp_fun;
// };

struct avl* avl_init(struct avl* avl, cmp_fun_t cmp_fun) {
  avl->root = NULL;
  *(cmp_fun_t*)&avl->cmp_fun = cmp_fun;
  return avl;
}

struct avl* avl_deinit(struct avl* avl) {
  node_dispose(avl->root);
  memset(avl, 0, sizeof *avl);
  return avl;
}

struct avl* avl_insert(struct avl* avl, void* val) {
  struct node* node = node_insert(avl->root, avl->cmp_fun, val);
  if (!avl->root) avl->root = node;
  // else for (size_t i = 0; i < 3; i++) avl->root = node_all_balance(avl->root);
  return avl;
}

size_t avl_height(struct avl* avl) {
  return node_height(avl->root);
}

void* avl_query(struct avl* avl, void* val) {
  struct node* node = node_query(avl->root, avl->cmp_fun, val);
  return node ? node->value : NULL;
}

void avl_fdraw(struct avl* avl, FILE* out) {
  node_draw(avl->root, out);
}

void avl_draw(struct avl* avl) {
  avl_fdraw(avl, stdout);
}
