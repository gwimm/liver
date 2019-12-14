// vim:ft=c:ts=2:sw=2:sts=2:et

#include <buf>

#include <stdlib.h>
#include <string.h>

struct buf* buf_new() {
  struct buf* head = malloc(sizeof *head);
  head->len = 0;
  return head;
}

ssize_t buf_len(struct buf* buf) {
  return buf ? buf->len : -1;
}

bool buf_is_nulled(struct buf* buf) {
  return buf ? arr_is_nulled(buf->buf, buf->len) : false;
}

struct buf* buf_set_len(struct buf* buf, size_t len) {
  if (buf) {
    buf = realloc(buf, offsetof(struct buf, buf) + len);
    if (len > buf->len) memset(buf->buf + buf->len, 0, len - buf->len);
    buf->len = len;
  }
  return buf;
}

struct buf* buf_ensure_len(struct buf* buf, size_t len) {
  return buf->len < len ? buf_set_len(buf, len) : buf;
}


dbuf buf_to_dbuf(struct buf* buf) {
  return buf ? buf->buf : NULL;
}
struct buf* dbuf_to_buf(dbuf buf) {
  return buf ? (struct buf*)(buf - offsetof(struct buf, buf)) : NULL;
}


dbuf dbuf_new() { return buf_to_dbuf(buf_new()); }
void dbuf_del(dbuf buf) { free(dbuf_to_buf(buf)); }

bool dbuf_is_nulled(dbuf buf) { return buf_is_nulled(dbuf_to_buf(buf)); }

// length
ssize_t dbuf_len(dbuf buf) { return buf_len(dbuf_to_buf(buf)); }

dbuf dbuf_set_len(dbuf buf, size_t len) {
  return buf_to_dbuf(buf_set_len(dbuf_to_buf(buf), len));
}
dbuf dbuf_ensure_len(dbuf buf, size_t len) {
  return buf_to_dbuf(buf_ensure_len(dbuf_to_buf(buf), len));
}
