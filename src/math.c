/* vim:ft=c:ts=2:sw=2:sts=2:et
*/

#include <math>

/* 
// (a ^ b) % n
*/ 

u64 powmod(u64 a, u64 b, u64 n) {
  u64 res = 1u % n;
  while (b > 0) {
    if (b & 1)
      res = (res * a) % n;
    a = (1ull * a * a) % n;
    b >>= 1;
  }
  return res;
}


typedef uint16_t u16;

typedef int32_t  i32;
typedef uint32_t u32;
typedef float    f32;

typedef double   f64;

/*
//  the IEEE 754-2008 32-bit base-2 format, binary32,
//  specifies:
//    - sign: 1 bit
//    - exp:  8 bits
//    - frac: 23 bits
//
//  this gives 6 to 9 significant decimal digits of precision.
//
//    sign 1   exp 8      frac 23
//    =        ========   =======================
*/

u32 u32_log2_fast(const u32 n) {
  union { u32 u; f32 f; } x;
  x.f = n;
  return (x.u >> 23) - 127;
}

u64 u64_log2_fast(u64 n) {
  union { u64 u; f64 f; } x;
  x.f = n;
  return (x.u >> 52) - 1023;
}

u64 u64_log2(u64 n) {
  const u64 b[] = { 0x2, 0xC, 0xf0, 0xff00, 0xffff0000, 0xffffffff00000000 };
  const u64 S[] = { 1, 2, 4, 8, 16, 32 };
  u64 ret = 0;
  for (size_t i = 5; i >= 0; i--)
  if (n & b[i]) {
    n >>= b[i];
    ret |= S[i];
  }
  return ret;
}

/*
// #define u8_log2(n) (n ? (1 << 3) - 1 - __builtin_clz(n) : 0)
*/

u8 u8_log2(u8 n) {
  for (u8 ret = 0;; ret++, n >>= 1)
    if (n == 0)
      return ret;
}

/*
//  b ^ e mod m
*/

u64 mod_pow(u64 b, u64 e, u64 m) {
  u64 ret = 1;
  b %= m;
  for (; e > 0; e >>= 1) {
    if (e & 1) {
      ret *= b;
      ret %= m;
    }
    b *= b;
    b %= m;
  }
  return ret;
}

#ifdef __GNUC__
# define alloca(s) __builtin_alloca(s)
#endif

#define panic(...) fprintf(stderr, __VA_ARGS__), exit(1)
#define tmpalc(s) ((s) < (2 << 16) ? alloca(s) : badalc(s))
#ifdef PANIC_ON_BAD_TMP_ALC
# define badalc(n) \
   panic("%s:%i: bad stack allocation of size: %lu", \
         __FILE__, __LINE__, n)
#else
# define badalc(n) NULL
#endif

#include <string.h>

#define buf_len(x) x ## _len

#define unless(...) if (!(__VA_ARGS__))
#define CHAR_BIT 8

typedef union {
  u16 n;
  struct { u8 lo, hi; };
} split_ddigit_t;

bool buf_is_zero(
    const u8* op,
    size_t op_len
  ) {
  if (op) {
    for (size_t i = 0; i < buf_len(op); i++)
      if (op[i] != 0)
        return false;
    return true;
  } else return false;
}

u8* buf_not(
    u8* res,
    size_t buf_len(res),
    const u8* op1,
    size_t buf_len(op1)
  ) {
  for (size_t i = 0; i < min(buf_len(res), buf_len(op1)); i++) res[i] = !op1[i];
  return res;
}

u8* buf_shl(
    u8* op,
    size_t buf_len(op),
    size_t shift
  ) {
  unless (op) return NULL;
  size_t n = buf_len(op);

  size_t shift_word = shift / CHAR_BIT;
  size_t shift_bits = shift % CHAR_BIT;

  if (shift_word) {
    memmove(op + shift_word, op, buf_len(op) - shift);
    memset(op, 0, shift);
  }

  for (size_t i = 0; i < n; i++)
    op[i] = (op[i] << shift_bits)
          | (op[i - 1] >> ((CHAR_BIT * sizeof *op) - shift_bits));

  return op;
}

u8* buf_add(
    u8* res,
    size_t buf_len(res),
    const u8* op1,
    size_t buf_len(op1),
    const u8* op2,
    size_t buf_len(op2)
  ) {
  unless (res && op1 && op2) return NULL;
  size_t n = min(buf_len(res), buf_len(op1), buf_len(op2));
  split_ddigit_t x = { 0 };

  for (size_t i = 0; i < n; i++) {
    x.n    = op1[i] + op2[i] + x.hi;
    res[i] = x.lo;
  }

  return res;
}

#include <limits.h>

typedef struct arr {
  size_t len;
  size_t siz;
  void* ptr;
} arr_t ;

typedef u8 digit_t;

struct arr arr_sub(
    struct arr res,
    const struct arr op1,
    const struct arr op2
  ) {
  unless (res.ptr && op1.ptr && op2.ptr) return (struct arr){0};

  size_t n = min(res.len, op1.len, op2.len);
  union { u16 n; struct { u8 lo, hi; }; } x = { .hi = 1 };

  digit_t *a = op1.ptr, *b = op2.ptr, *c = res.ptr;

  for (size_t i = 0; i < n; i ++)
    c[i] = x.n = (split_ddigit_t){ .lo = a[i], .hi = 1}.n
               - b[i]
               - !x.hi;

  /*                                                                       |
   |  x    = [ 1 0 ]          say borrow didn't happen last iteration      |
   |                                                                       |
   |  {                                                                    |
   |    a[1] = 1              pre-emtive borrow                            |
   |    x    = a - b - !x[1]                                               |
   |  }                                                                    |
   |                                                                       */

  return res;
}

u8* buf_sub(
    u8* res,
    size_t buf_len(res),
    const u8* op1,
    size_t buf_len(op1),
    const u8* op2,
    size_t buf_len(op2)
  ) {
  return arr_sub(
      (arr_t){ .ptr = res, .len = res_len},
      (arr_t){ .ptr = (digit_t*)op1, .len = op1_len},
      (arr_t){ .ptr = (digit_t*)op2, .len = op2_len}
    ).ptr;
}

u8* buf_mul_karatsuba(
    u8* res,
    size_t buf_len(res),
    const u8* op1,
    size_t buf_len(op1),
    const u8* op2,
    size_t buf_len(op2)
  ) {
  unless (res && op1 && op2) return NULL;

  return res;
}

u8* buf_mul(
    u8* res,
    size_t buf_len(res),
    const u8* op1,
    size_t buf_len(op1),
    const u8* op2,
    size_t buf_len(op2)
  ) {
  unless (res && op1 && op2) return NULL;

  size_t n = min(buf_len(res), buf_len(op1), buf_len(op2));

  /* todo: endian independency */
  union { u16 n; struct { u8 lo, hi; }; } x = { 0 };

#if 0
  for (size_t i = 0; i < n; i++) {
    u16 carry = 0;
    for (size_t j = 0; j < n; j++) {
      x.n  += carry + op1[i] * op2[j];
      carry = x.hi;
      if (i + j < n)
        res[i + j] = x.lo;
    }
  }
#endif

#if 0
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j <= i; j++) {
      x.n += op1[j] * op2[i - j];
    }
    res[i] = x.lo;
    x.n    = x.hi;
  }
#endif

  return res;

}
