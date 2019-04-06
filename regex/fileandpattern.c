#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <regex.h>
#include <sys/types.h>
/**
 * $CC -Wall -ggdb3 mmgetstan.c -o mmgetstan
 **/

char *path_cat (const char *str1, char *str2);

int main (int argc, char * argv[]) 
{
	// if no pattern then no process
	// so user should fill all the
	// flag, except for the dir path, for which
	// the default is ../log 
	struct dirent *dp;
	regex_t regex, regex2;
	int reti;
	int ret;
	int i = 0;
	FILE * fp;
	char strErr[256]={'\0'};
	char line[1024];
	const char *dir_path=argv[1];
	if (argc < 4)
	{
		fprintf (stderr, "usage: mmgetstan directory filepattern matchstrpattern\n");
		exit (-1);
	}
	printf("argc %d\n",argc);
	printf("path %s\n",dir_path);
	printf("patt %s\n",argv[2]);
	printf("pattt %s\n",argv[3]);
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
		printf("regex:%s\n", argv[2]);		
		printf("regex2:%s\n", argv[3]);		
		reti = regcomp(&regex,argv[2],0);
		if(reti)
		{
			fprintf(stderr,"could not compile regex\n"); 
			exit(1);
		}
		reti = regcomp(&regex2,argv[3],0);
		if(reti)
		{
			fprintf(stderr,"could not compile regex2\n"); 
			exit(1); 
		}
		reti = regexec(&regex,tmp,0,NULL,0);
		if(!reti) 
		{
			puts("Match");
			fp = fopen(tmp,"r");
			if(!fp)
			{
				fprintf(stderr,"error opening file %s\n",tmp);
				exit (-1);
			}
			while(fgets (line, sizeof(line), fp )) 
			{ 
				reti = regexec (&regex2, line, 0, NULL, 0);
				if (!reti)
				{
					fprintf (stdout,"\nmatch pattern %s", line); 
					for (i = 36; (line[i]!=' ')&& (i< strlen(line)); i++)
						fprintf (stdout,"%c", (char )line[i]);
					fprintf (stdout, "%c", '_');
					for (i=0; i<4; i++)
						fprintf (stdout, "%c", argv[2][strlen(argv[2])-(4-i)]);
				}
			}

			fprintf(stdout, "\n");
			fclose(fp); 
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
