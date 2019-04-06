#include <stdio.h>
#include <string.h> 
#include <oci.h>
#include <stdlib.h>
#include <unistd.h> 
#include <pthread.h>
/*
 *$CC -c - I$ORACLE_HOME/rdbms/demo \
 *        -I$ORACLE_HOME/rdbms/public \
 *        -I$ORACLE_HOME/plsql/public \
 *        -I$ORACLE_HOME/network/public \
 *        -I$ORACLE_HOME/precomp/public committest.c
 *$CC -L$ORACLE_HOME/lib/ -L$ORACLE_HOME/rdbms/lib/ -o committest committest.o -lclntsh -lpthread
 *
 *do not forget to export ORACLE_HOME
 *my $ORACLE_HOME=/home/henky/app/henky/product/11.1.0/client
 */
int print_oci_error(OCIError *errhp, int ret, char *buff, int buflen, char *prefix)
{ 
	if (!errhp)
	{
		fprintf (stderr, "[%s] err_code = %d\n", prefix, ret);
		return 0;
	}
	OCIErrorGet (errhp, 1, NULL, &ret, (OraText *)buff, 
		buflen, OCI_HTYPE_ERROR);
		fprintf (stderr, "[%s] %s\n",prefix, buff); 
	return 0;
}
typedef struct struct_ociproperty
{ 
	struct struct_ociproperty *this;
	OCIEnv		*envhp;
	OCIServer	*srvhp;
	OCIError	*errhp;
	OCISvcCtx	*svchp;
	OCIStmt		*stmthp;
	int (* login)  (struct struct_ociproperty *, char*, char*, char*);
	int (* logout) (struct struct_ociproperty *);
	int (* preparestatement) (struct struct_ociproperty *, char *sqlstmt);
	int (* executestatement) (struct struct_ociproperty *);
	int (* begintrans)  (struct struct_ociproperty *);
	int (* committrans) (struct struct_ociproperty *); 
	
}STRUCT_OCIPROPERTY, *PSTRUCT_OCIPROPERTY;
int  __oci_login (struct struct_ociproperty *p, char *dbn,
		char *uid, char *pwd)
{ 
	char 		err[512];
	int		ret = 0;
	if ((ret = OCIEnvCreate (&p->envhp, OCI_DEFAULT, (dvoid *)0, 0, 
		0, 0, (size_t)0, (dvoid **)0) ) != OCI_SUCCESS ) 
		fprintf (stderr, "oci env create failed\n");
	
	if ((ret = OCIHandleAlloc ((dvoid *) p->envhp, (dvoid **) &p->errhp, 
		(ub4) OCI_HTYPE_ERROR, (size_t) 0, (dvoid **) 0)) 
		!= OCI_SUCCESS)
		print_oci_error (NULL, ret, err, 512, "err"); 

	if ((ret = OCIHandleAlloc ((dvoid *) p->envhp, (dvoid **) &p->srvhp,
		(ub4) OCI_HTYPE_SERVER, (size_t) 0, (dvoid **) 0))
		!= OCI_SUCCESS)
		print_oci_error (NULL, ret, err, 512, "srv"); 
	
	if ((ret = OCIHandleAlloc ((dvoid *) p->envhp, (dvoid **) &p->svchp, 
		(ub4) OCI_HTYPE_SVCCTX, (size_t) 0, (dvoid **) 0))
		!= OCI_SUCCESS)
		print_oci_error (NULL, ret, err, 512, "context"); 
		
	if ((ret = OCIHandleAlloc ((dvoid *) p->envhp, (dvoid **) &p->stmthp, 
		OCI_HTYPE_STMT, 0, 0)) != OCI_SUCCESS) 
		print_oci_error (NULL, ret, err, 512, "stmt"); 

	if ((ret = OCIAttrSet ((dvoid *)p->svchp, OCI_HTYPE_SVCCTX, 
		(dvoid *)p->srvhp, 0,
		OCI_ATTR_SERVER, p->errhp)) != OCI_SUCCESS)
		print_oci_error (NULL, ret, err, 512, "set attrib");

	if ((ret = OCILogon (p->envhp, p->errhp, &p->svchp, (text *)uid, 
		strlen(uid), (text *) pwd, strlen (pwd), (text *) dbn, 
		strlen (dbn))) != OCI_SUCCESS) 
		print_oci_error (p->errhp, ret, err, 512, "logon"); 

	return ret;
}

int __oci_logout (struct struct_ociproperty *p)
{
	int ret = 0;
	char 		err[512];
	if ((ret = OCILogoff (p->svchp, p->errhp) ) != OCI_SUCCESS)
		print_oci_error (p->errhp, ret, err, 512, "logout"); 

	if (p->envhp)	
		if ((ret = OCIHandleFree (p->envhp, OCI_HTYPE_ENV)) 
				!= OCI_SUCCESS)
			print_oci_error (NULL, ret, err, 512, "logout-env"); 
	if (p->errhp) 
		if ((ret = OCIHandleFree (p->errhp, OCI_HTYPE_ERROR)) 
				!= OCI_SUCCESS)
			print_oci_error (NULL, ret, err, 512, "logout-err"); 
	
	if (p->srvhp)
		if ((ret = OCIHandleFree (p->srvhp, OCI_HTYPE_SERVER)) 
				!= OCI_SUCCESS)
			print_oci_error (NULL, ret, err, 512, "logout-srv"); 

	if (p->svchp) 
		if ((ret = OCIHandleFree (p->svchp, OCI_HTYPE_SVCCTX)) 
				!= OCI_SUCCESS)
			print_oci_error (NULL, ret, err, 512, "logout-svc"); 
	return ret;
}

int __oci_preparestatement (struct struct_ociproperty *p, char *sqlstmt)
{
	int ret = 0;
	char err[512];
	if ((ret = OCIStmtPrepare (p->stmthp, p->errhp, (text *)sqlstmt, 
		strlen ((char *)sqlstmt), OCI_NTV_SYNTAX, 0)) != OCI_SUCCESS)
		print_oci_error (p->errhp, ret, err, 512, "preparestmt"); 
	return ret;
}

int __oci_executestatement (struct struct_ociproperty *p)
{
	int ret = 0;
	char err[512];
	if ((ret = OCIStmtExecute (p->svchp, p->stmthp, p->errhp, 
			1, 0, 0, 0, 0))!= OCI_SUCCESS)
		print_oci_error (p->errhp, ret, err, 512, "execstmt"); 
	return ret;
}

int __oci_committrans(struct struct_ociproperty *p)
{
	int ret = 0;
	char err[512];
	if ((ret = OCITransCommit (p->svchp, p->errhp, 
		(ub4) 0)) != OCI_SUCCESS) 
		print_oci_error (p->errhp, ret, err, 512, "commit"); 
	return ret;
}
int __oci_begintrans(struct struct_ociproperty *p)
{

	int ret = 0;
	char err[512];
	if ((ret = OCITransStart (p->svchp, p->errhp, 60, 
		(ub4) 0)) != OCI_SUCCESS)
		print_oci_error (p->errhp, ret, err, 512, "begin"); 
	return ret;
}
PSTRUCT_OCIPROPERTY new_ociproperty ()
{
	PSTRUCT_OCIPROPERTY prop = (PSTRUCT_OCIPROPERTY) calloc (1, 
		sizeof (STRUCT_OCIPROPERTY)); 
	if (prop)
	{
		prop->this	= prop;
		prop->login  = __oci_login;
		prop->logout = __oci_logout; 
		prop->preparestatement = __oci_preparestatement;
		prop->executestatement = __oci_executestatement;
		prop->begintrans = __oci_begintrans;
		prop->committrans = __oci_committrans;
	}
	return prop;
}

PSTRUCT_OCIPROPERTY new_ociproperty_login(char *dbn, char *uid, char *pwd)
{
	PSTRUCT_OCIPROPERTY prop = new_ociproperty();
	if (prop)
	{
		if (prop->login(prop, dbn, uid, pwd) != OCI_SUCCESS)
		{
			free (prop);
			prop = NULL; 
		}
			
	}
	return prop;
}

/**
 * Name 	: insertloop
 * Description 	: loop insert and request commit/continue
 * Returns	: 0 if user request to quit
 *                1 if there is a database error 
 * Notes	: we need to read from terminal after fgetc until got new line, 
 *                so that the available string do not spoil next fgetc
 * http://stackoverflow.com/questions/12063879/fgetcstdin-in-a-loop-is-producing-strange-behaviour
 */
int insertloop(PSTRUCT_OCIPROPERTY p)
{ 
	char		sqlstmt[512];
	char		sqlstmterr[512]; 
	char		sqlprep[512];
	int		i = 0;
	int		c = 0;
	int		ret = 0;
	
	sprintf ((char *)sqlstmt, "INSERT INTO xlink.xldummy (Application, "
		"UserId, Creationtimestamp, Ipcomputer, "
		"Datavalue) VALUES ('%%010d','1', "
		"Current_Date, '1','1')" );

	sprintf ((char *)sqlstmterr, "INSERT INTO xlink.xldummy (Recno, "
		"Application, UserId, Creationtimestamp, Ipcomputer, "
		"Datavalue) VALUES ('%%010d','1', '1', "
		"Current_Datet, '1', '1' )" );

	sprintf (sqlprep, sqlstmt, i++);
	ret = p->preparestatement (p, sqlprep);
	if (ret != OCI_SUCCESS) 
		return ret;
	ret = p->executestatement (p); 
	if (ret != OCI_SUCCESS) 
		return ret;
	while (1)
	{
		fprintf (stdout, "\tc : commit\n"
				"\tx : quit no commit\n"
				"\tq : commit and quit\n");
		c = fgetc(stdin);
		while (fgetc(stdin) != 0x0a)
			;
		switch (c)
		{
		case 'c':
			ret = p->committrans (p);
			if (ret != OCI_SUCCESS) 
				return ret;
			ret = p->begintrans(p);
			if (ret != OCI_SUCCESS) 
				return ret;
			break; 
		case 'x': 
			return 0; 
			break;
		case 'q':
			ret = p->committrans (p);
			if (ret != OCI_SUCCESS) 
				return ret;
			return 0;
		default:
			break;
		
		} 
	}
}

void *timeout_handler (void *lp)
{
	char err[512];
	int ret = 0; 
	PSTRUCT_OCIPROPERTY p = (PSTRUCT_OCIPROPERTY) lp;

	sleep(10); 
	printf (stdout, "timeout wakeup");

	ret = OCIBreak (p->svchp, p->errhp);
	if (ret != OCI_SUCCESS)
		print_oci_error (p->errhp, ret, err, 512, "break"); 

	ret = OCIReset (p->svchp, p->errhp);
	if (ret != OCI_SUCCESS)
		print_oci_error (p->errhp, ret, err, 512, "reset");

	return NULL;
}

int main (int argc, char **argv) 
{ 
	int		c = 0;
	int		il = 0;
	int		conti_nue = 1;
	pthread_t	thread_id = 0;
	
	PSTRUCT_OCIPROPERTY p = 0; 
	p = new_ociproperty_login ("simasc5", "x", "x1"); 
	if (!p)
	{
		fprintf (stderr, "failed to connect"); 
		exit (-1);
	}
	pthread_create (&thread_id, 0, timeout_handler, (void *)p); 
	pthread_detach (thread_id);
	while (conti_nue)
	{
		p->begintrans (p);
		il = insertloop (p); 

		fprintf (stdout, "\tc : continue\n");
		fprintf (stdout, "\th : halt\n");

		c = fgetc (stdin);

		while (fgetc (stdin) != 0x0a)
			;

		switch (c)
		{
		case 'c':
			if ( il != OCI_SUCCESS )
			{
				p->logout(p);
				free(p);
				p = new_ociproperty_login(
				"simasc5", "x","x1"); 
				if (!p)
				{
					fprintf (stderr, "failed to connect"); 
					break;
				}
			}
			break;
		case 'h':
			conti_nue = 0;
			break;
		default:
			break;
			
		}
		
	}
	//p->logout(p);
	//free(p); 
	OCITerminate (OCI_DEFAULT); 
	exit (0);
}
