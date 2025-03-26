#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

int main()
{
    CURL *curl;
    CURLcode r;
    char url[] =
        "http://c-for-dummies.com/curl_test.txt";

    /* initialize easy curl */
    curl = curl_easy_init();
    if( curl == NULL)
    {
        fprintf(stderr,"curl init fail\n");
        exit(1);
    }

    /* set options */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl,CURLOPT_FOLLOWLOCATION,1L);

    /* curl */
    r = curl_easy_perform(curl);
    if( r!=CURLE_OK )
    {
        fprintf(stderr,"Curl fail");
        exit(1);
    }

    /* cleanup */
    curl_easy_cleanup(curl);

    return(0);
}
