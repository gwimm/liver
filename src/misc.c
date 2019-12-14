int max(int op1, int op2) {
  return op1 < op2 ? op2 : op1;
}

int min(int op1, int op2) {
  return op1 < op2 ? op1 : op2;
}

int int_cmp(void *lhs, void *rhs) {
  return *(int*)rhs - *(int*)lhs;
}

enum cmp cmp(int op1, int op2) {
  return op1 == op2 ? cmp_eq : op1 < op2 ? cmp_gt : cmp_lt;
}

#include <string.h>

bool arr_is_nulled(char* arr, size_t len) {
  char tmp[len];
  return !memcmp(memset(tmp, 0, sizeof tmp), arr, sizeof tmp);
}
