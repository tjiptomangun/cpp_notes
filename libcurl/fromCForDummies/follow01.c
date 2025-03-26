#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
// This code fails as the url[] string is an invalid (old) address.

int main() {
	CURL *curl;
	CURLcode r;

	char url[] = "http://c-for-dummies.com/curl_test.txt";

	curl = curl_easy_init();
	if (curl == NULL) {
		fprintf(stderr, "curl init fail\n");
		exit(1);
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);

	r = curl_easy_perform(curl);
	if (r != CURLE_OK) {
		fprintf(stderr, "Curl Fail\n");
		exit(1);
	}

	curl_easy_cleanup(curl);
	return 0;

}
