//https://gist.github.com/p120ph37/71fb340f7a0d38fa5443

/**
 * HTTP POST with authentication using "basic" method
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
	char *memory;
	size_t size;        
};

static size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *context){
	size_t bytec = size * nmemb;
	struct MemoryStruct * mem = (struct MemoryStruct *) context;
	mem->memory = realloc(mem->memory, mem->size + bytec + 1);
	if (mem->memory == NULL) {
	    printf("not enough memory (realloc returned NULL)\n");
	    return 0;	
	}
	memcpy(&(mem->memory[mem->size]), ptr, bytec);
	mem->size += bytec;
	mem->memory[mem->size] = 0;
	return nmemb;
}

int main(void) {
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk;
	chunk.memory = malloc(1);
	chunk.size = 0; 
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://desktop_rumah.com:3001/callGw/v1/getToken");
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, (long) CURLAUTH_BASIC);
		curl_easy_setopt(curl, CURLOPT_USERNAME, "testUser");
		curl_easy_setopt(curl, CURLOPT_PASSWORD, "123123123");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Accept: text/plain");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		res = curl_easy_perform(curl);
		if (res != CURLE_OK) {
			fprintf(stderr, "not ok\n");
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}
		else {
			curl_off_t size;
			long resCode;
			curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &size); 
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); 
			printf("length: %lu\n", size);
			printf("status: %ld\n", resCode);
			printf("%s\n", chunk.memory);
		}
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		free(chunk.memory);
		curl_global_cleanup();
	}
	return 0;
}
