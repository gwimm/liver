/* vim:ft=c:ts=2:sw=2:sts=2:et
*/

#include <stdlib.h>
#include <list>

/*
//                                == linked list ==
//
//                                                      complexity  | average 
//                                                    =========================
//                                                     access rand  |    n    
//                                                            end   |    1    
//                                                    =========================
//                                                        space     |    n    
//
// 
//
//    +-----------------------------------------------------+
//    |   +------+     +------+     +------+     +------+   |
//    +-> | next | >-> | next | >-> | next | >-> | next | >-+
//    +-< | prev | <-< | prev | <-< | prev | <-< | prev | <-+
//        |      |     |      |     |      |     |      |
*/

struct list {
  struct list* next;
  struct list* prev;
  void* val;
};

struct list* list_init(struct list* list) {
  return list->prev = list->next = list;
}

struct list* list_deinit(struct list* list) {
  list->next->prev = list->prev;
  list->prev->next = list->next;
  return list_init(list);
}

struct list* list_dispose(struct list* list) {
  list->prev->next = NULL;
  for (struct list* next = list; next; next = list->next, free(next));
  return list_init(list);
}

struct list* list_incr(struct list* list, size_t i) {
  while (i--) list = list->next;
  return list;
}

struct list* list_insert(struct list* list, struct list* next) {
  return list->next = ((next->next = (next->prev = list)->next)->prev = next);
}

struct list* list_head_push(struct list* list) {
  return list_insert(list->prev, malloc(sizeof *list));
}
