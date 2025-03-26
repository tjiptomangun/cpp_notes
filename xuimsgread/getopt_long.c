
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "getopt_long.h"


static int gcd(int a, int b)
{
    int c;

    c = a % b;
    while (c != 0) 
    {
	a = b;
	b = c;
	c = a % b;
    }

    return (b);
}

int optreset = 0;

static char *place = EMSG; /* option letter processing */

/* XXX: set optreset to 1 rather than these two */
static int nonopt_start = -1; /* first non option argument (for permute) */
static int nonopt_end = -1;   /* first option after non options (for permute) */

/* Error messages */
static const char recargchar[] = "option requires an argument -- %c";
static const char recargstring[] = "option requires an argument -- %s";
static const char ambig[] = "ambiguous option -- %.*s";
static const char noarg[] = "option doesn't take an argument -- %.*s";
static const char illoptchar[] = "unknown option -- %c";
static const char illoptstring[] = "unknown option -- %s";


static void permute_args( int panonopt_start, 
			  int panonopt_end, 
			  int opt_end,
			  char * const *nargv)
{
	int cstart, cyclelen, i, j, ncycle, nnonopts, nopts, pos;
	char *swap;


	nnonopts = panonopt_end - panonopt_start;
	nopts = opt_end - panonopt_end;
	ncycle = gcd(nnonopts, nopts);
	cyclelen = (opt_end - panonopt_start) / ncycle;

	for (i = 0; i < ncycle; i++) 
	{
		cstart = panonopt_end+i;
		pos = cstart;
		for (j = 0; j < cyclelen; j++) 
		{
			if (pos >= panonopt_end)
				pos -= nnonopts;
			else
				pos += nopts;
			swap = nargv[pos];

			((char **) nargv)[pos] = nargv[cstart];

			((char **)nargv)[cstart] = swap;
		}
	}
}


static int parse_long_options( char * const *nargv, 
			       const char *options,
			       const struct option *long_options, 
			       int *idx, 
			       int short_too )
{
	char *current_argv, *has_equal;
	size_t current_argv_len;
	int i, match;

	current_argv = place;
	match = -1;

	optind++;

	if ((has_equal = strchr(current_argv, '=')) != NULL) 
	{
		/* argument found (--option=arg) */
		current_argv_len = has_equal - current_argv;
		has_equal++;
	} 
	else
		current_argv_len = strlen(current_argv);

	for (i = 0; long_options[i].name; i++) 
	{
		/* find matching long option */
		if (strncmp(current_argv, long_options[i].name, current_argv_len))
			continue;

		if (strlen(long_options[i].name) == current_argv_len) 
		{
			/* exact match */
			match = i;
			break;
		}
		
		/*
		 * If this is a known short option, don't allow
		 * a partial match of a single character.
		 */
		 
		if (short_too && current_argv_len == 1)
			continue;

		if (match == -1)	/* partial match */
			match = i;
		else 
		{
		//static void warnx(int print_error, const char *fmt, ...)
			/* ambiguous abbreviation */
//			if (PRINT_ERROR)
//				warnx(ambig, (int)current_argv_len, current_argv);
				//warnx(ambig,  current_argv);
					    
					    
			optopt = 0;
			return (BADCH);
		}
	}
	
	if (match != -1) 
	{		/* option found */
		if (long_options[match].has_arg == no_argument
		    && has_equal) 
		    {
//			if (PRINT_ERROR)
//				warnx(noarg, (int)current_argv_len,  current_argv);
			if (long_options[match].flag == NULL)
				optopt = long_options[match].val;
			else
				optopt = 0;
			return (BADARG);
		}
		if (long_options[match].has_arg == required_argument ||
		    long_options[match].has_arg == optional_argument) {
			if (has_equal)
				optarg = has_equal;
			else if (long_options[match].has_arg ==   required_argument) 
			{
			    optarg = nargv[optind++];
			}
		}
		
		if ((long_options[match].has_arg == required_argument)    && (optarg == NULL)) 
		{
		
//			if (PRINT_ERROR)
//				warnx(recargstring,  current_argv);

			if (long_options[match].flag == NULL)
				optopt = long_options[match].val;
			else
				optopt = 0;
			--optind;
			return (BADARG);
		}
		
	} 
	else 
	{			/* unknown option */
		if (short_too) {
			--optind;
			return (-1);
		}
//		if (PRINT_ERROR)
//			warnx(illoptstring, current_argv);
		optopt = 0;
		return (BADCH);
	}
	if (idx)
		*idx = match;
	if (long_options[match].flag) {
		*long_options[match].flag = long_options[match].val;
		return (0);
	} else
		return (long_options[match].val);
}

static int getopt_internal( int nargc, 
			    char * const *nargv, 
			    const char *options,
			    const struct option *long_options, 
			    int *idx, 
			    int flags )
{
	char *oli;				/* option letter list index */
	int optchar, short_too;
	static int posixly_correct = -1;

	if (options == NULL)
		return (-1);


	if (posixly_correct == -1)
		posixly_correct = (getenv("POSIXLY_CORRECT") != NULL);
	if (posixly_correct || *options == '+')
		flags &= ~FLAG_PERMUTE;
	else if (*options == '-')
		flags |= FLAG_ALLARGS;
	if (*options == '+' || *options == '-')
		options++;

	if (optind == 0)
		optind = optreset = 1;

	optarg = NULL;
	if (optreset)
		nonopt_start = nonopt_end = -1;
start:
	if (optreset || !*place) 
	{		/* update scanning pointer */
		optreset = 0;
		if (optind >= nargc) 
		{          /* end of argument vector */
			place = EMSG;
			if (nonopt_end != -1) 
			{
				/* do permutation, if we have to */
				permute_args(nonopt_start, nonopt_end, optind, nargv);
				optind -= nonopt_end - nonopt_start;
			}
			else if (nonopt_start != -1) 
			{
				optind = nonopt_start;
			}
			nonopt_start = nonopt_end = -1;
			return -1;
		}
		
		if (*(place = nargv[optind]) != '-' || (place[1] == '\0' && strchr(options, '-') == NULL)) 
		    {
			place = EMSG;		/* found non-option */
			if (flags & FLAG_ALLARGS) 
			{
    			    optarg = nargv[optind++];
			    return (INORDER);
			}
			
			if (!(flags & FLAG_PERMUTE)) 
			{
			    return -1;
			}

			if (nonopt_start == -1)
				nonopt_start = optind;
			else if (nonopt_end != -1) 
			{
				permute_args(nonopt_start, nonopt_end, optind, nargv);
				nonopt_start = optind - (nonopt_end - nonopt_start);
				nonopt_end = -1;
			}
			optind++;
			/* process next argument */
			goto start;
		}
		if (nonopt_start != -1 && nonopt_end == -1)
			nonopt_end = optind;


		if (place[1] != '\0' && *++place == '-' &&   (place[1] == '\0' || long_options == NULL)) 
		    {
			optind++;
			place = EMSG;

			if (nonopt_end != -1) 
			{
				permute_args(nonopt_start, nonopt_end, optind, nargv);
				optind -= nonopt_end - nonopt_start;
			}
			nonopt_start = nonopt_end = -1;
			return -1;
		    }
	}


	if (long_options != NULL && place != nargv[optind] &&    (*place == '-' || (flags & FLAG_LONGONLY))) 
	    {
		short_too = 0;
		if (*place == '-')
			place++;		/* --foo long option */
		else if (*place != ':' && strchr(options, *place) != NULL)
			short_too = 1;		/* could be short option too */

		optchar = parse_long_options(nargv, options, long_options, idx, short_too);
		if (optchar != -1) 
		{
			place = EMSG;
			return (optchar);
		}
	}

	if ((optchar = (int)*place++) == (int)':' ||   (oli = strchr(options, optchar)) == NULL) 
	{
	    
	    if (optchar == (int)'-')
			return (-1);
		/* option letter unknown or ':' */
	    if (!*place)
			++optind;
//		if (PRINT_ERROR)
//			warnx(illoptchar, optchar);
		optopt = optchar;
		return (BADCH);
	}

	if (long_options != NULL && optchar == 'W' && oli[1] == ';') 
	{
		/* -W long-option */
		if (*place)			/* no space */
			/* NOTHING */;
		else if (++optind >= nargc) {	/* no arg */
			place = EMSG;
//			if (PRINT_ERROR)
//				warnx(recargchar, optchar);
			optopt = optchar;
			return (BADARG);
		} else				/* white space */
			place = nargv[optind];
		optchar = parse_long_options(nargv, options, long_options, idx, 0);
		place = EMSG;
		return (optchar);
	}
	
	if (*++oli != ':') 
	{			/* doesn't take argument */
		if (!*place)
			++optind;
	} 
	else 
	{				/* takes (optional) argument */
	    optarg = NULL;
	    if (*place)			/* no white space */
			optarg = place;
		/* XXX: disable test for :: if PC? (GNU doesn't) */
		else if (oli[1] != ':') 
		{	/* arg not optional */
			if (++optind >= nargc) 
			{	/* no arg */
				place = EMSG;
//				if (PRINT_ERROR)
//					warnx(recargchar, optchar);
				optopt = optchar;
				return (BADARG);
			} 
			else
			    optarg = nargv[optind];
		}
		place = EMSG;
		++optind;
	}
	/* dump back option letter */
	return (optchar);
}

//#ifdef REPLACE_GETOPT

int getopt(int nargc, char * const *nargv, const char *options)
{
    int ret =  getopt_internal(nargc, nargv, options, NULL, NULL, 0);
    return ret;
}

//#endif /* REPLACE_GETOPT */

int getopt_long( int nargc,  char * const *nargv,  const char *options,  const struct option *long_options, int *idx )
{
    int ret = getopt_internal(nargc, nargv, options, long_options, idx,  FLAG_PERMUTE);
    return ret;
}

int getopt_long_only( int nargc,
		      char * const *nargv,
		      const char *options,
		      const struct option *long_options,
		      int *idx )
{
    int ret = getopt_internal(nargc, nargv, options, long_options, idx,  FLAG_PERMUTE|FLAG_LONGONLY);
    return ret;
}
