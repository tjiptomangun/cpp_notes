static struct node
{
	char *key;
	int info;
}
static current node a[M+1];
hashinitialize ()
{
	int i;
	for (i = 0; i <= M; i++)
	{
		a[i].key = " ";
		a[i].info = -1;
	}
}
hashinsert (char *v, int info)
{
	int x = hash (v);
	int u = hash2 (v);
	while (strcmp (" ", a[x].key))
		x = (x+u) %M;
	a[x].key = v; 
	a[x].info = info;
}
