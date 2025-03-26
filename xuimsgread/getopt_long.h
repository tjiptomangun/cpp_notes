#ifndef GETOPT_LONG_H
#define GETOPT_LONG_H

#define no_argument 		0
#define required_argument 	1
#define optional_argument 	2


struct option
{
  const char *name;
  int has_arg;
  int *flag;
  int val;
};

#define PRINT_ERROR	((opterr) && (*options != ':'))

#define FLAG_PERMUTE	0x01	/* permute non-options to the end of argv */
#define FLAG_ALLARGS	0x02	/* treat non-options as args to option "-1" */
#define FLAG_LONGONLY	0x04	/* operate as getopt_long_only */

/* return values */
#define	BADCH		(int)'?'
#define	BADARG		((*options == ':') ? (int)':' : (int)'?')
#define	INORDER 	(int)1

#define	EMSG		""

int getopt_long( int nargc,  char * const *nargv,  const char *options,	 const struct option * long_options,  int *idx );
int getopt_long_only(int nargc,	char * const *nargv, const char *options, const struct option *long_options, int *idx);

#ifdef _qnx_621B_
extern char *optarg;
extern int optind, opterr,optopt;
#endif

#endif
