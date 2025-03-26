#include "aes.h"
#include "config.h"

int main(char * argv[])
{
	aes_context aes_ctx1;
	aes_context aes_ctx2;
	unsigned char key[32];
	unsigned char buf[64];
	unsigned char prv[16];
	unsigned char iv[16];
	int i, j, u, v=1, offset;
	strcpy(key,"MELODRAMATICA");
	
/*	
	for( i = 0; i < 6; i++ )
	{
		u = i >> 1;
		v = i & 1;
	

		printf( "  AES-CBC-%3d (%s): ", 128 + u * 64,
			( v == AES_DECRYPT ) ? "dec" : "enc" );
		memset( iv , 0, 16 );
		iv[0]=1;iv[1]=5;iv[2]=18;iv[3]=101;
		memset( prv, 0, 16 );
		memset( buf, 0, 16 );
		
		if( v == AES_DECRYPT )
		{
			aes_setkey_dec( &aes_ctx1, key, 128 + u * 64 );
			//for( j = 0; j < 10000; j++ )
                		aes_crypt_cbc( &aes_ctx1, v, 16, iv, buf, buf );			
			printf("\ndecrypt : %s\n",buf);
		}
        	else
        	{
			sprintf(buf,"%s","hei test");
			printf("\nencrypt : %s\n",buf);
            		aes_setkey_enc( &aes_ctx1, key, 128 + u * 64 );
            		//for( j = 0; j < 10000; j++ )
            		{					
                		unsigned char tmp[16];
                		aes_crypt_cbc( &aes_ctx1, v, 16, iv, buf, buf );
                		memcpy( tmp, prv, 16 );
                		memcpy( prv, buf, 16 );
                		memcpy( buf, tmp, 16 );
            		}
			
        	}
            	printf( "passed\n" );
	}
*/
}