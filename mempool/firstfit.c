/**
 *
 * $CC -Wall -ggdb3 firstfit.c -o firstfit
 **/
typedef struct node
{
	int next;
	int size; 
};

struct node * firstfit (struct node *avail, int size)
{
	struct node * curr;
	struct node * prev;
	unsigned char * tmp;
	tmp = (unsigned char *)avail;
	int k = 0;
	prev = curr = avail;
	if (!curr)
		return NULL;
	prev = curr; 
	do 
	{
		if (curr->size <size)
		{ 
			prev = curr;
			curr = (struct node *)&tmp[curr->next];
			continue;
		}
		k = curr->size - size;
		if  ( k == 0)
			prev->next = curr->next;
		curr->size = k;
		break;

	}while (curr) 
}
/**
 * Name		: liberate
 * Description	: free memory from position p0 for at least N size. 
 *		  This will merge several consecutive free memory area.
 **/
void liberate (struct node * avail, struct node *p0, int N)
{
	struct node * prev = avail;
	struct node * curr = prev->next; 
	do
	{
		curr = prev->next
		if ((curr != NULL) && (curr <= p0)) 
		{
			prev = curr;
			continue;
		}
		if (((p0+N) == curr ) && (curr=!NULL))
		{
			N = N + curr->size;
			p0->next = curr->next;

		}
		else
		{
			p0->next = curr;
		}
		if ((prev + prev->size) == p0)
		{
			prev->size = prev->size + N;
			prev->next = p0->next;
		}
		else
		{
			prev->next = p0;
			p0->size = N;
		}
			

	}while (curr)
}
int main (int argc, char **argv)
{
		
}
