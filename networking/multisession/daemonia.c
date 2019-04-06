#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h> 
#include <errno.h>

#include "parserclass.h"
#include "filecfgparser.h"

int writer_stub()
{
	fprintf (stdout, "Hello World!");
	fprintf (stderr, "Error World!");
	return 0;
}

char logname [256] = {0};

/**
 * NAME		: PrepareService
 * DESCRIPTION	: prepare this app for service
 *		  1. load config file
 *		  2. set up env variable
 *		  3. set log file
 **/
int PrepareService ()
{
	FILE *fp;
	PMINIPARSER pfile;
	char tokenlist [] = "[]#\n=";
	PLIST root = 0;
	PLIST curr = 0;
	PPROPERTY pprop = 0;
	char newenv [258];
	int fd = -1;

	fp = fopen ("../dat/XLAdmAgent.ini", "r");
	if (!fp)
	{
		fprintf (stderr, "error opening file ../dat/XLAdmAgent.ini\n");
		exit (1);
	} 

	pfile = newminiparser (fp, tokenlist, infile_parse);
	pfile->parse (pfile, &root);
	fclose (fp); 

	curr = (PLIST) root->takename (root, "environment"); 
	if (curr)
	{
		while ((pprop = (PPROPERTY) curr->take (curr)))
		{
			if (parse_env_str (pprop->value, newenv, 258))
			{
				setenv (pprop->l_item.class.name, newenv, 1); 
			}
		} 
	} 
	curr->l_item.class.preparedelete (&curr->l_item.class);
	free (curr);

	
	curr = (PLIST) root->takename (root, "process"); 
	pprop = (PPROPERTY) curr->takename (curr, "LOG"); 
	if (pprop)
	{
		sprintf (logname, "../log/%s", pprop->value);
	}
	else
	{
		fprintf (stderr, "please define \"LOG\" in XLAdmAgent.ini\n");
		exit (1);
	} 

	pprop->l_item.class.preparedelete (&pprop->l_item.class);
	free (pprop);

	root->l_item.class.preparedelete (&root->l_item.class);
	free (root);

	fd = open (logname, O_RDWR|O_CREAT);
	if (fd == -1)
	{
		fprintf (stderr, "Unable to open %s\n", logname);
		exit (-1);
	}
	close (fd);

	return 0;
}/*PrepareService*/

int Daemonize ()
{

	pid_t	pid;
	int	i;
	long	var_sysconf = 0;
	int	fdout = -15;
	int	n_errno = 0;
	
	/* create new process */
	pid = fork ();
	if (pid == -1)
		return -1;
	else if (pid != 0)
		exit (EXIT_SUCCESS);

	/* create new session and process group */
	if (setsid () == -1)
		return -1;

	/* set the working directory to the root directory */
	//if (chdir ("/home/henky/c_c++/networking/multisession") == -1)
	//	return -1;

	/* close all open files -- NR_OPEN is overkill, but works */ 
	var_sysconf = sysconf (_SC_OPEN_MAX); 
	for (i = 0; i < var_sysconf; i++)
		close (i); 

	/* redirect fd's 0, 1, 2 to /dev/null */
	open ("/dev/null", O_RDWR);	/* stdin */
	//dup (0);		/* stdout */
	//dup (0);		/* stderr */
	sleep (25);
	fdout = open (logname, O_RDWR|O_CREAT|O_APPEND);
	if ( fdout == -1)
		n_errno = errno;

	dup (fdout);
	dup (fdout);
	close (fdout); 
	return 0;
}
int main (void)
{
	PrepareService();
	Daemonize();
	while (1)
	{
		writer_stub();	
		sleep (1);	

	}

	return 0;
	

}

