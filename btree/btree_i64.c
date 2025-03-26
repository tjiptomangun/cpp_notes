#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 *gcc -m32 -Wall -ggdb3 btree.c -o btree
 */

typedef struct node
{
	int value;
	struct node *up;
	struct node *left;
	struct node *right;
} NODE, *PNODE;

PNODE  newnode (int value)
{
	PNODE res = (PNODE) calloc (1, sizeof (NODE));
	if (res)
	{
		res->value = value;
	} 
	return res;
}

PNODE insert (PNODE pto, PNODE in)
{
	if (pto == NULL)
			return in;
	if (in->value > pto->value)
	{
		if (pto->right)
			insert (pto->right, in);
		else
		{
			pto->right = in;
			in->up = pto;
		}
	}
	if (in->value <=pto->value)
	{
		if (pto->left)
			insert (pto->left, in);
		else
		{
			pto->left = in;
			in->up = pto;
		}
	}
	return in;
}

/**
 * Name		: postorder
 * Description	: get the rightmost descendant
 **/
PNODE postorder (PNODE p)
{
	if (!p->right)
		return p;
	return postorder (p->right);
}
/**
 * Name		: preorder
 * Description	: get the leftmost descendant
 **/
PNODE preorder (PNODE p)
{
	if (!p->left)
		return p;
	return preorder (p->left);
}
/**
 * Name		: inorderpre
 * Description	: get the rightmost member of left child
 **/
PNODE inorderpre (PNODE p)
{
	if(!p->left)
		return NULL;
	return postorder (p->left); 
}

/**
 * Name		: inorderpost
 * Description	: get the leftmost member of right child
 **/
PNODE inorderpost (PNODE p)
{
	if (!p->right)
		return NULL;
	return preorder (p->right);
}

/**
 * Name 	: inorder
 * Description	: get the rightmost member of left child of an item or
 *		  the leftmost member or right child
 **/ 
PNODE inorder (PNODE p)
{
	if (!p->left)
		return NULL;
	if (p->left->right)
		return postorder (p->left);	
	else if (p->right->left)
		return preorder (p->right);
	return NULL;
}
/**
 * Name		: searchval
 * Description	: search value from current node
 **/
PNODE searchval (PNODE p, int val)
{
	if (!p)
		return NULL;
	if (p->value == val)
		return p;
	else if  (val < p->value)
		return searchval (p->left, val);
	return searchval(p->right, val);
	
}
/**
 * Name		: deletenode
 * Description	: delete specified node. If no child then just delete.
 *		  if got one child the redirect parent's left/right pointer
 *		  to this child accordingly then delete node.
 *		  If there are 2 child change inorderpre value or inorderpost value
 *		  with current value, delete the inorderpre or inorderpost.
 *		  If there are neither inorderpre nor inorderpost then redirect
 *		  parent left to current left, and delete this.
 *		  
 **/
PNODE deletenode (PNODE p)
{
	PNODE cand;
	if (!(p->left || p->right))
	{
		if (p->up)
		{
			if (p->up->left == p)
				p->up->left = NULL;
			else
				p->up->right = NULL;
		}
		free (p);
		return NULL;
	}
/*
	else if ((!p->left) && p->right)
	{
		if (p->up)
		{
			if (p->up->left == p)
			{
				p->up->left = p->right;	
			}
			else
			{
				p->up->right = p->right; 
			}
			p->right->up = p->up;
			free (p);
		}
		else
		{//we re deleting root node with only right child remains
			p->value = p->right->value;
			deletenode (p->right);	
		}
		return NULL;
	}
	else if ((!p->right) && p->left)
	{
		if (p->up)
		{
			if (p->up->left == p)
			{
				p->up->left = p->left;
			}
			else
			{
				p->up->right = p->left;
			}
			p->left->up = p->up;
			free (p);
		}
		else
		{//we re deleting root node with only left child remains
			p->value = p->left->value;
			deletenode (p->left);	
		}
	}
*/
	else
	{
		if ((cand = inorderpre (p)) || (cand = inorderpost(p)))
		{
			p->value = cand->value;
			deletenode(cand);
		}
	}
	return NULL;
}

/**
 * Name		: deleteval
 * Description	: delete specified value
 *
 **/
PNODE deleteval (PNODE p, int val)
{
	PNODE ptodel = NULL;
	fprintf (stdout, "deleteval %d\n",val);
	ptodel = searchval (p,val);
	if (!ptodel) 
		return NULL;
	return deletenode (ptodel);	
	
}
void nodeprint (PNODE p , int ident)
{
	
	int i = 0;

	
	if (!p)
		return;
	nodeprint (p->left, ident+1);
	for (i = 0; i< ident; i++)
		fprintf (stdout, "-->");
	fprintf (stdout, "(%d)\n",p->value);
	nodeprint (p->right, ident+1);
}
int main (int argc, char **argv)
{
	PNODE root ;
	root = insert (NULL, newnode (7));
	insert (root, newnode(3));
	insert (root, newnode(18));

	insert (root, newnode(11));
	insert (root, newnode(5));
	insert (root, newnode(4));
	insert (root, newnode(1));
	insert (root, newnode(-1));
	insert (root, newnode(12));
	insert (root, newnode(15));
	insert (root, newnode(17));
	insert (root, newnode(10));
	insert (root, newnode(8));
	insert (root, newnode(9));
	insert (root, newnode(-9));
	insert (root, newnode(33));
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,11);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,11);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,9);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,1);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,3);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,7);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,12);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0);
	deleteval(root,15);
	fprintf (stdout, "================nodeprint================\n");
	nodeprint(root, 0); 
	nodeprint(root, 0);
	deleteval (root, 18);
	nodeprint(root, 0);

	while(root->left!=NULL || root->right!=NULL)
	{
		fprintf (stdout,"val = %d\n", root->value);
		deletenode(root); 
		nodeprint(root, 0);
	}


	
	return 1;
}
