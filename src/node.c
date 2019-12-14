// vim:ft=c:ts=2:sw=2:sts=2:et

//                       
//                       == binary search tree node ==
//
//  the worst case time complexity can be brought      cxty  | average | worst
//  from O(n) to O(log n) by balancing the tree      ==========================
//  regularily, such is the case in an AVL tree,      insert |  log n  |   n
//  an implementation is available in `avl.c`.       --------+---------+-------
//                                                    query  |  log n  |   n
//                                                   --------+---------+-------
//                                                    delete |  log n  |   n
//                                                   ==========================
//                                                    space  |    n    |   n
//

#include <stdlib.h>

#include <node>
#include <vec>
#include <misc>

struct node* node_init (
  struct node* node,
  struct node* prev,
  void* value
) {
  if (!node) return NULL;
  node->prev = prev;
  node->gt = node->lt = NULL;
  node->value = value;
  return node;
}

struct node* node_deinit(struct node* node) {
  return node;
}

static struct node** _node_query(
    struct node** node,
    cmp_fun_t cmp_fun,
    void* value
) {
  if (node) while (*node) switch (cmp(0, cmp_fun((*node)->value, value))) {
  case cmp_lt: node = &(*node)->lt;  break;
  case cmp_gt: node = &(*node)->gt; break;
  case cmp_eq: return node;
  };
  return node;
}

struct node* node_insert(
  struct node* node,
  cmp_fun_t cmp_fun,
  void* value
) {
  struct node** _node = _node_query(&node, cmp_fun, value);
  return
    _node
      ? *_node
        ? NULL // handle equal tree values;
        : (*_node = node_init(malloc(sizeof *node), node, value))
      : NULL;
}

struct node* node_query(
    struct node* node,
    cmp_fun_t cmp_fun,
    void* value
) {
  struct node** _node = _node_query(&node, cmp_fun, value);
  return _node && *_node ? *_node : NULL;
}

bool node_is_root(struct node* node) {
  return node && !!node->prev;
}

bool node_is_edge(struct node* node) {
  return node && node->prev && node->lt && node->gt;
}

bool node_is_leaf(struct node* node) {
  return node && !(node->lt || node->gt);
}

struct node* node_root(struct node* node) {
  if (node) for (; node->prev; node = node->prev);
  return node;
}

struct node* node_succ(struct node* node) {
  if (node && node->gt)
    for (node = node->gt; node->lt ; node = node->lt);
  return node;
}

size_t node_depth(struct node* node) {
  size_t ret = 0;
  for (; node; ret++, node = node->prev);
  return ret;
}

size_t node_height(struct node* node) {
  size_t height = 0;
  if (node) {
    struct vec* vec = vec_new(sizeof node);

    vec = vec_push(vec, &node);
    for (size_t count = 0; (count = vec_len(vec)); count--, height++)
    for (; count; count--) {
        vec = vec_pop(vec, &node);
        if (node->lt) vec = vec_push(vec, &node->lt);
        if (node->gt) vec = vec_push(vec, &node->gt);
    }

    vec_del(vec);
  }
  return height;
}

void* node_remove(struct node* node) {
  if (!node) return NULL;

  void* ret = node->value;
  struct node* succ = node_succ(node);

  node->value = succ->value;
  succ->prev->lt = succ->gt;
  if (succ->gt) succ->gt->prev = succ->prev;
  free(succ);

  return ret;
}

void node_dispose(struct node* node) {
  if (!node) return;

  struct vec* vec = vec_new(sizeof node);

  for (; !vec_is_empty(vec); vec = vec_pop(vec, node)) {
    if (node->lt) vec = vec_push(vec, node->lt);
    if (node->gt) vec = vec_push(vec, node->gt);
    free(node_deinit(node));
  }

  vec_del(vec);
}
