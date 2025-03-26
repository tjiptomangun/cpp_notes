#include "parserclass.h"
#include "rdxtree.h"
#include <stdio.h>
void print_reverse(PRDXTREE_FIND_CONTEXT ctx) {
	if (ctx) {
		ctx->reverse_once(ctx);
	}
	PRDXTREE_CONTEXT_ITEM curr = ctx->head;
	while (curr) {
		fprintf(stdout, " %s ", curr->item->name);
		fprintf(stdout, " <- ");
		curr = curr->next;
	}
	
}

void find_child_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding name %s  ", to_find);
	PRDXTREE_FIND_CONTEXT ctx= new_rdxtree_find_context();
	PRDXTREE_ITEM curr = rdxtreeitem_findkey(root, to_find, ctx);
	if(curr){
		print_reverse(ctx);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	} 
	ctx->delete(ctx);
	ctx = NULL;
}

void find_prefix_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "finding prefix %s  ", to_find);
	PRDXTREE_FIND_CONTEXT ctx= new_rdxtree_find_context();
	PRDXTREE_ITEM curr = rdxtreeitem_findprefix(root, to_find, ctx);
	if(curr){
		print_reverse(ctx);
		fprintf(stdout, "\n");
	}
	else {
		fprintf(stdout, "NOT FOUND\n");
	}
	ctx->delete(ctx);
	ctx = NULL;
}

void delete_and_print(PRDXTREE_ITEM root, char * to_find){
	fprintf(stdout, "delete key %s  \n", to_find);
	rdxtreeitem_deletekey(root, to_find);
	print_rdx_tree(root, 0);
	
}

int main (int argc, char **argv) {
	PRDXTREE_ITEM root;
	root = new_rdxtreeitem(NULL, 0);
	rdxtreeitem_insertkey(root, "hello");
	rdxtreeitem_insertkey(root, "world");
	rdxtreeitem_insertkey(root, "he");
	rdxtreeitem_insertkey(root, "thank");
	rdxtreeitem_insertkey(root, "busier");
	rdxtreeitem_insertkey(root, "world");
	rdxtreeitem_insertkey(root, "bustiere");
	rdxtreeitem_insertkey(root, "stop complain");
	rdxtreeitem_insertkey(root, "the");
	rdxtreeitem_insertkey(root, "thereover");
	rdxtreeitem_insertkey(root, "then");
	rdxtreeitem_insertkey(root, "this");
	rdxtreeitem_insertkey(root, "thus");
	rdxtreeitem_insertkey(root, "criminal");
	rdxtreeitem_insertkey(root, "crime");
	rdxtreeitem_insertkey(root, "crimea");
	rdxtreeitem_insertkey(root, "therefore");
	rdxtreeitem_insertkey(root, "thereof");
	rdxtreeitem_insertkey(root, "therein");
	rdxtreeitem_insertkey(root, "stop asking");
	rdxtreeitem_insertkey(root, "stop ball");
	rdxtreeitem_insertkey(root, "therein");
	rdxtreeitem_insertkey(root, "there");
	rdxtreeitem_insertkey(root, "123423433");
	rdxtreeitem_insertkey(root, "88888");
	rdxtreeitem_insertkey(root, "77777");
	rdxtreeitem_insertkey(root, "323256");
	rdxtreeitem_insertkey(root, "124");
	rdxtreeitem_insertkey(root, "324");
	
	fprintf(stdout, "----- * -----\n");
	
	find_child_and_print(root, "he");
	find_child_and_print(root, "thus");
	find_child_and_print(root, "crimean");
	find_child_and_print(root, "stop");
	find_child_and_print(root, "crimea");

	fprintf(stdout, "----- * -----\n");
	find_prefix_and_print(root, "crimson");
	find_prefix_and_print(root, "crimealand");
	find_prefix_and_print(root, "thereof");
	find_prefix_and_print(root, "theresa mae");
	find_prefix_and_print(root, "busier");
	find_prefix_and_print(root, "323");
	find_prefix_and_print(root, "12464");
	find_prefix_and_print(root, "323256323");
	find_prefix_and_print(root, "8888555");
	find_prefix_and_print(root, "7777755");
	
	fprintf(stdout, "----- * -----\n");
	print_rdx_tree(root, 0);
	
	delete_and_print(root, "crimea");
	delete_and_print(root, "stop");
	delete_and_print(root, "7777755");
	delete_and_print(root, "the");
	delete_and_print(root, "than");
	find_child_and_print(root, "the");
	char buff[1000] = {0};
	int count = 0;
	rdxtreeitem_getkeywords(root, buff, sizeof(buff), &count, ",");
	fprintf(stdout, "%s\n", buff);
	rdxtreeitem_deletenode(root);

	return 0;
	
}
