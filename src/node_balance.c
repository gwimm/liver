// vim:ft=c:ts=2:sw=2:sts=2:et

#include <node>

// balancing a tree requires rotating unbalanced trees into
// a better balance factor. This function rotates two functions,
// leaving its <right ? right : left> node as parent.
//
// this diagram explains `node_rotate()`'s behaviour:
//
// | node_rotate(a, true);
// |
// |    a              b
// |   / \            / \
// | al   b    =>   a   br
// |     / \       / \
// |   bl   br   al   bl
// |-
//
struct node* node_rotate(struct node* node, bool right) {
  struct node* a = node;

  struct node* b  = a->next[right];
  a->next[right]  = b->next[!right];
  b->next[!right] = a;

  b->prev = a->prev;
  a->prev = b;

  return b;
}

// These functions are helpers, most often rotations of themselves.
//
// | node_rotate_rl(a)
// |  
// |      a    rot(b, false)    a
// |     / \                   / \
// |   al   b                al   c
// |       / \       =>          / \
// |      c   br               cl   b
// |     / \                       / \
// |   cl   cr                   cr   br
// | 
// | | node_rotate_ll(a)
// | | 
// | |     a
// | |    / \                b
// | |  al   b     =>      /   \
// | |      / \           a     c
// | |    bl   c         / \   / \
// | |        / \       al bl cr cr
// | |      cl   cr
// | |-
// |-
//
struct node* node_rotate_ll(struct node* node) {
  return node_rotate(node, false);
}

struct node* node_rotate_rr(struct node* node) {
  return node_rotate(node, true);
}

struct node* node_rotate_lr(struct node* node) {
  node->gt = node_rotate(node->gt, false);
  return node_rotate_ll(node);
}

struct node* node_rotate_rl(struct node* node) {
  node->lt = node_rotate(node->lt, true);
  return node_rotate_rr(node);
}

// # balance factor
//
// The balance factor is defined as:
//
//     node.bfac := node.right.height - node.left.height
//
//   the difference of its two sub-trees.
//
// Its sign determines the direction in which the tree is heavy.
// Positive and negative for right and left heavy respectively
// 
// Balanced would it be with a factor of zero.
//
ssize_t node_bfac(struct node* node) {
  return node ? node_height(node->gt) - node_height(node->lt) : 0;
}

// # balance current node
//
// TODO: iterative and selective implementation (`insert_balance()`)
//
// Balancing a single node isn't effective, and will not lower time complexity
// overall. This function is to be used in an insertion algorithm.
//
struct node* node_balance(struct node* node) {
  ssize_t bfac = node_bfac(node);
  return
    bfac > 1                                 // right heavy
      ? node_bfac(node->gt) > 0
        ? node_rotate_ll(node)
        : node_rotate_lr(node)
      : bfac < -1                            // left heavy
        ? node_bfac(node->lt) < 0
          ? node_rotate_rr(node)
          : node_rotate_rl(node)
        : node;                              // balanced
}

// # balance entire tree
//
// Balancing any tree requires balancing its sub-trees first.
// this function is endlessly ineffective. Its selective implementation
// meant to be used on insertion, should be used instead.
//
struct node* node_all_balance(struct node* node) {
  if (!node) return NULL;
  node->lt = node_all_balance(node->lt);
  node->gt = node_all_balance(node->gt);
  return node_balance(node);
}
