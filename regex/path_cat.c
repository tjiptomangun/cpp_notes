#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <regex.h>
#include <sys/types.h>

char *path_cat (const char *str1, char *str2);

int main (int argc, char * argv[]) 
{
	// if no pattern then no process
	// so user should fill all the
	// flag, except for the dir path, for which
	// the default is ../log 
	struct dirent *dp;
	regex_t regex;
	int reti;
	int ret;
	FILE * fp;
	char strErr[256]={'\0'};
	char line[1024];
	const char *dir_path=argv[1];
	printf("argc %d\n",argc);
	printf("path %s\n",dir_path);
	printf("patt %s\n",argv[2]);
	ret=access(dir_path,F_OK);
	if(ret!=0)
	{
		perror(strErr);
		printf("%s\n",strErr);
		return 0;
	}
	DIR *dir = opendir(dir_path);
	printf("dir:%s\n", dir_path);
	while ((dp=readdir(dir)) != NULL) 
	{
		char *tmp;

		tmp = path_cat(dir_path, dp->d_name);
		printf("tmp:%s\n", tmp);		
		reti = regcomp(&regex,argv[2],0);
		if(reti){fprintf(stderr,"could not compile regex\n"); exit(1);}
		reti = regexec(&regex,tmp,0,NULL,0);
		if(!reti) 
		{
			puts("Match");
			fp = fopen(tmp,"r");
			if(!fp)
			{
				sprintf(stderr,"error opening file %s\n",tmp);
				exit (-1);
			}
			while(fgets (line, sizeof(line), fp )) 
			{
				fprintf (stdout,"%s", line); 
			}
			fclose(fp); 
			exit (0);
		}
		else 
			puts("No Match"); 
		free(tmp);
		tmp=NULL;
	}
	closedir(dir);
	return 0;
}
/*
 this function concatenates a directory with a filename
 adding '/' if the directory does not ended with one
*/
char *path_cat (const char *parent, char *child) {
	int i,j;
	size_t str1_len = strlen(parent);
	size_t str2_len = strlen(child);
	char *result;
	result = (char *)malloc((str1_len+str2_len+2)*sizeof (char));
	if(parent[str1_len - 1]=='/') 
		strncpy (result,parent,str1_len-1); 
	else 
	{
		strcpy (result,parent);
		str1_len+=1;
	}
	
	strcat(result,"/");
	for(i=str1_len, j=0; j<str2_len;i++, j++) 
		result[i]=child[j]; 
	result[str1_len+str2_len]='\0';
	return result;
}
