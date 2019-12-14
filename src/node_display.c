// vim:ft=c:ts=2:sw=2:sts=2:et

#include <node>

//
// TODO: write less heap-heavy implementation of this garbage.
//       `node.prev` is available, use that instead.
//

struct trunk {
	struct trunk* prev;
	char* str;
};
 
void trunk_print(struct trunk* p, FILE* out) {
	if (!p) return;
	trunk_print(p->prev, out);
	fprintf(out, "%s", p->str);
}
 
void __node_draw(
  struct node* node,
  FILE* out,
  struct trunk* prev,
  int is_gt
) {
	if (!node) return;
 
	struct trunk this_disp = { prev, "    " };
	char* prev_str = this_disp.str;
	__node_draw(node->gt, out, &this_disp, 1);
 
	if (!prev)
		this_disp.str = "---";
	else if (is_gt) {
		this_disp.str = ".--";
		prev_str = "   |";
	} else {
		this_disp.str = "`--";
		prev->str = prev_str;
	}
 
	trunk_print(&this_disp, out);
	fprintf(out, "%i\n", *(int*)node->value);
 
	if (prev) prev->str = prev_str;
	this_disp.str = "   |";
 
	__node_draw(node->lt, out, &this_disp, 0);
}

void node_draw(struct node* node, FILE* out) {
  if (!node) return;

  __node_draw(node, out, NULL, 0);
}
