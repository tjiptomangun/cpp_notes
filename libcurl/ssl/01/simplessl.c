//https://curl.se/libcurl/c/simplessl.html 
#include <stdio.h>
#include <curl/curl.h>
int main(void) {
	CURL *curl;
	CURLcode res;
	FILE *headerfile;
	const char *pPassphrase = NULL;
	static const char *pCertFile = "./tokopedia-com.pem";
	static const char *pCACertFile = "./digicert_cacert.pem";
	static const char *pHeaderFile = "dumpit";
	const char *pKeyName;
	const char *pKeyType;
	const char *pEngine;

#ifdef USE_ENGINE
	pKeyName = "rsa_test";
	pKeyType = "ENG";
	pEngine = "chil";
#else
	pKeyName = "testkey.pem";
	pKeyType = "PEM";
	pEngine = NULL; 
#endif

	headerfile = fopen(pHeaderFile, "wb");
	if (!headerfile)
		return 1;

	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "https://www.tokopedia.com");
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, headerfile); 
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127)  /* conditional expression is constant */
#endif
		do {
			if (pEngine){
				/* use crypto engine */
				if(curl_easy_setopt(curl, CURLOPT_SSLENGINE, pEngine) != CURLE_OK) {
				/* load the crypto engine */
					fprintf(stderr, "cannot set crypto engine\n");
					break;
				}
				if(curl_easy_setopt(curl, CURLOPT_SSLENGINE_DEFAULT, 1L) != CURLE_OK) {
				/* set the crypto engine as default */
				/* only needed for the first time you load 
				an engine in a curl object... */
					fprintf(stderr, "cannot set crypto engine as default\n");
					break;
				}
			}
			/* cert is stored PEM coded in file... */
			/* since PEM is default, we needn't set it for PEM */
			curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
			/* set the cert for client authentication */
			curl_easy_setopt(curl, CURLOPT_SSLCERT, pCertFile);	
			if(pPassphrase) { 
				curl_easy_setopt(curl, CURLOPT_KEYPASSWD, pPassphrase);	
			}
			//curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, pKeyType);	
			//curl_easy_setopt(curl, CURLOPT_SSLKEY, pKeyName);	
			curl_easy_setopt(curl, CURLOPT_CAINFO, pCACertFile);	
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);	

			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res)); 
		} while(0);
#ifdef _MSC_VER
#pragma warning(pop)
#endif
		/* always cleanup */
	    curl_easy_cleanup(curl);
		
	}
	curl_global_cleanup(); 
	fclose(headerfile); 
	return 0;

} 
