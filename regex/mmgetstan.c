#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <regex.h>
#include <sys/types.h>
/**
 * $CC -Wall -ggdb3 mmgetstan.c -o mmgetstan
 * eq. to check MM start
 *./mmgetstan . MM.*_0430 "Start"
 **/

char *path_cat (const char *str1, char *str2);

/**
 * Name		: getmmstanalloc
 * Description 	: check starting point in MM
 * Returns
 *	0	: exist, starting point is the name of the file
 *	others	: does not exists
 *	
 */
static int getmmstanalloc (char *dir_path, char *mmdd, int stan, char* starting_point)
{
	char mmfilepattern[40];
	char mmstartpattern [120];
	regex_t regex, regex2;
	char line[1024];
	int ret = 0, i=0;
	int linelen = 0;
	DIR *dir = NULL;
	struct dirent *dp;
	char *tmp = NULL;
	FILE *fp;
	int found =0; 
	sprintf (mmfilepattern, "MM.*_%s", mmdd);
	sprintf (mmstartpattern, "%X  MMAlloc Start ",stan);

	ret = access (dir_path, F_OK);
	if (ret)
		return ret;
	dir = opendir(dir_path);

	ret = regcomp (&regex, mmfilepattern, 0);
	if (ret)
		return 1;
	ret = regcomp (&regex2, mmstartpattern, 0);
	if (ret)
		return 1;
	while ((dp = readdir(dir)))
	{
		tmp = path_cat(dir_path, dp->d_name);	
		ret = regexec (&regex, tmp, 0, NULL, 0);
		if (ret) 
			continue;
		fp = fopen (tmp, "r");
		if (!fp)
			continue;		
		while (fgets (line, sizeof (line), fp))
		{ 
			ret = regexec (&regex2, line, 0, NULL, 0);
			if (ret)
				continue; 
			found = 1;
			break;

		}
		if (!found)
		{
			fclose (fp);
			free (tmp);
			continue;
		}
		linelen = strlen (line);
		for (i=36; (line[i]!=' ')&&(i<linelen);i++) 
			starting_point[i-36] = line[i];
		i-=36;
		starting_point[i++] = '_';
		starting_point[i++] = mmdd[0];
		starting_point[i++] = mmdd[1];
		starting_point[i++] = mmdd[2];
		starting_point[i++] = mmdd[3];
		starting_point[i++] = 0;
		fclose (fp);
		free (tmp);
	}
	regfree (&regex);
	regfree (&regex2); 
	return 0;
}

int main (int argc, char * argv[]) 
{
	// if no pattern then no process
	// so user should fill all the
	// flag, except for the dir path, for which
	// the default is ../log 
	int stan = 0;
	char *mmdd;

	char starting_point[180];



	const char *dir_path=argv[1];
	if (argc < 4)
	{
		fprintf (stderr, "usage: mmgetstan directory mmdd stan\n");
		exit (-1);
	}
	printf("argc %d\n",argc);
	printf("path %s\n",dir_path);
	mmdd = argv[2];
	stan = atoi(argv[3]);
	printf("mmdd %s\n", mmdd);
	printf("stan %s\n", argv[3]);
	if(!getmmstanalloc (argv[1], mmdd, stan, starting_point))
	{
		fprintf (stdout,"%s\n", starting_point);
	}
/*	
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
	*/
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
