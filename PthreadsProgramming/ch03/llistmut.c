#include <pthread.h>
#include <stdlib.h>
//PThreads programming book
//linked list
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef struct llist_node {
	int index;
	void *datap;
	struct llist_node *nextp;
} llist_node_t;

typedef  struct llist {
	llist_node_t *first;
	pthread_mutex_t mutex;
} llist_t;

int llist_init(llist_t *llistp) {
	llistp->first = NULL;
	pthread_mutex_init(&(llistp->mutex), NULL);
	return 0;
}

int llist_insert_data(int index, void *datap, llist_t *llistp) {
	llist_node_t *cur, *prev, *new;
	int found =  FALSE;

	pthread_mutex_lock(&(llistp->mutex));
	for (cur = prev = llistp->first; cur != NULL; prev = cur, cur = cur->nextp) {
		if (cur->index == index) {
			free(cur->datap);
			cur->datap = datap;
			found = TRUE;
			break;
		}
		else if (cur->index > index) {
			break;
		}
	}
	if (!found) {
		new = (llist_node_t *) malloc (sizeof(llist_node_t));
		new->index = index;
		new->datap= datap;
		new->nextp = cur;
		if (cur == llistp->first) {
			llistp->first = new;
		}
		else {
			new->nextp = prev->nextp;
			prev->nextp = new;
		}
	}
	return 0;
}
int main(int argc, char **argv) {
}
