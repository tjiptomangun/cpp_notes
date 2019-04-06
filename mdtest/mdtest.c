/*
* mdtest.c
* $CC -Wall -ggdb3 mdtest.c -o mdtest -lcrypto 
*/

#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>

int main (int argc, char * argv[])
{
	EVP_MD_CTX mdctx;
	const EVP_MD *md;
	char mess1[] = "Text message\n";
	char mess2[] = "Hello World\n";
	unsigned char md_value  [EVP_MAX_MD_SIZE];
	unsigned int md_len ;
	int i;

	OpenSSL_add_all_digests ();
	if (! argv[1] )
	{
		printf ("Usage: mdtest digestname\n");
		exit (1 );
	}

	md = EVP_get_digestbyname( argv[1] );
	if ( ! md )
	{
		printf ("unknown message digest %s\n", argv[ 1]);
		exit (1);
	}

	EVP_MD_CTX_init ( &mdctx );
	EVP_DigestInit_ex (&mdctx, md, NULL );
	EVP_DigestUpdate (&mdctx, mess1, strlen (mess1) );
	EVP_DigestUpdate (&mdctx, mess2, strlen (mess2) );
	EVP_DigestFinal_ex (&mdctx, md_value, &md_len );
	EVP_MD_CTX_cleanup (&mdctx );

	printf ("Digest is : ");
	for ( i = 0; i <md_len ; i++ ) 
	{
		printf ("%02x", md_value[i]);
	}
	printf ("\n");
	
	exit ( 0);

}
